/**
 * @file can_communication.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "can_communication.hpp"

#include "can_communication_impl.hpp"
#include "can_data_viewer_conf.hpp"
#include "driver/can_config.h"

namespace MaSiRoProject
{
namespace CAN
{

///////////////////////////////////////////////////////////////////
#define THREAD_NAME_CAN "ThreadCAN"
CanCommunicationImpl *can;
///////////////////////////////////////////////////////////////////
volatile bool flag_thread_can_fin          = false;
volatile bool flag_thread_can_on_interrupt = false;
volatile bool flag_thread_can_initialized  = false;
volatile bool flag_thread_can_change_mode  = false;
volatile CAN_CTRL_STATE request_mode       = CAN_CTRL_STATE::MODE_NOT_INITIALIZE;
volatile CAN_CTRL_STATE current_mode       = CAN_CTRL_STATE::MODE_NOT_INITIALIZE;
///////////////////////////////////////////////////////////////////
ChangedModeFunction callback_changed_mode = nullptr;
MessageFunction callback_mess;
///////////////////////////////////////////////////////////////////

void change_can_mode(CAN_CTRL_STATE mode, const char *text)
{
    current_mode = mode;
    if (nullptr != callback_changed_mode) {
        callback_changed_mode(mode, text);
    }
}

///////////////////////////////////////////////////////////////////

void IRAM_ATTR thread_can_on_interrupt()
{
    flag_thread_can_on_interrupt = true;
}

void thread_can(void *args)
{
    flag_thread_can_initialized = true;
    char buffer[255];
#if DEBUG_MODE
    sprintf(buffer, "<%s> - start", THREAD_NAME_CAN);
    if (nullptr != callback_mess) {
        callback_mess(false, buffer, true);
    }
#endif
    while (false == flag_thread_can_fin) {
        try {
            if (false == can->begin()) {
                sprintf(buffer, "<%s> - NOT begin()", THREAD_NAME_CAN);
                if (nullptr != callback_mess) {
                    callback_mess(true, buffer, true);
                }
            } else {
                can->request_start();
                while (false == flag_thread_can_fin) {
                    if (true == flag_thread_can_on_interrupt) {
                        flag_thread_can_on_interrupt = false;
                        can->interrupt();
                    }
                    if (true == flag_thread_can_change_mode) {
                        flag_thread_can_change_mode = false;
                        switch (request_mode) {
                            case CAN_CTRL_STATE::MODE_RUNNING:
                                can->request_resume();
                                break;
                            case CAN_CTRL_STATE::MODE_INACTIVE:
                            case CAN_CTRL_STATE::MODE_ABORT:
                                can->request_suspend();
                                break;
                            case CAN_CTRL_STATE::MODE_PAUSE:
                                can->request_pause();
                                break;
                            case CAN_CTRL_STATE::MODE_STOPPING:
                            case CAN_CTRL_STATE::MODE_NOT_INITIALIZE:
                            case CAN_CTRL_STATE::MODE_READY:
                            case CAN_CTRL_STATE::MODE_FINISHED:
                            default:
                                break;
                        }
                    }
                    can->loop();
                    vTaskDelay(CAN_THREAD_INTERVAL);
                }
                can->request_suspend();
            }
        } catch (...) {
            sprintf(buffer, "<%s> - ERROR()", THREAD_NAME_CAN);
            if (nullptr != callback_mess) {
                callback_mess(true, buffer, true);
            }
        }
    }
    flag_thread_can_initialized = false;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool CanCommunication::change_mode(CAN_CTRL_STATE mode = CAN_CTRL_STATE::MODE_UNKNOW)
{
    bool result = true;
    if (CAN_CTRL_STATE::MODE_UNKNOW == mode) {
        switch (current_mode) {
            case CAN_CTRL_STATE::MODE_READY:
            case CAN_CTRL_STATE::MODE_INACTIVE:
            case CAN_CTRL_STATE::MODE_ABORT:
            case CAN_CTRL_STATE::MODE_STOPPING:
                request_mode = CAN_CTRL_STATE::MODE_RUNNING;
                break;
            case CAN_CTRL_STATE::MODE_RUNNING:
                request_mode = CAN_CTRL_STATE::MODE_INACTIVE;
                break;
            case CAN_CTRL_STATE::MODE_PAUSE:
                request_mode = CAN_CTRL_STATE::MODE_PAUSE;
                break;
            case CAN_CTRL_STATE::MODE_FINISHED:
            case CAN_CTRL_STATE::MODE_NOT_INITIALIZE:
            default:
                break;
        }
    } else {
        request_mode = mode;
    }
    flag_thread_can_change_mode = true;
    if (CAN_CTRL_STATE::MODE_PAUSE == request_mode) {
        delay(CAN_THREAD_INTERVAL);
    }

    return result;
}
bool CanCommunication::request_pause()
{
    return this->change_mode(CAN_CTRL_STATE::MODE_PAUSE);
}
bool CanCommunication::request_running()
{
    return this->change_mode(CAN_CTRL_STATE::MODE_PAUSE);
}
////////////////////////////////////////////////////////////////////////////////////////////

bool CanCommunication::sendable(CAN_CTRL_STATE state, CanData *data)
{
    return true;
}

CanDeviceInfo CanCommunication::get_device_info()
{
    return can->get_device_info();
}

std::vector<CanData> CanCommunication::get_send_loop()
{
    return can->get_loop_shot();
}

std::vector<CanData> CanCommunication::get_received()
{
    return can->get_received();
}
std::vector<CanData> CanCommunication::get_resume()
{
    return can->get_resume();
}

bool CanCommunication::setup_callback(void)
{
    return true;
}
bool CanCommunication::add_resume(CanData data)
{
    data.time = 0;
    return can->add_resume(data);
}
bool CanCommunication::setup_default(void)
{
    bool result = true;
    if (nullptr != this->callback_setting_default) {
        result = this->callback_setting_default();
    }
    return result;
}
bool CanCommunication::add_one_shot(CanData data)
{
    return can->add_one_shot(data);
}
bool CanCommunication::add_loop_shot(CanData data, int interval)
{
    return can->add_loop_shot(data, interval);
}

bool CanCommunication::clear_loop_shot()
{
    return can->clear_loop_shot();
}
bool CanCommunication::clear_resume()
{
    return can->clear_resume();
}
bool CanCommunication::delete_loop_shot(unsigned long Id)
{
    return can->delete_loop_shot(Id);
}
bool CanCommunication::delete_resume(unsigned long Id)
{
    return can->delete_resume(Id);
}

bool CanCommunication::set_callback_message(MessageFunction callback)
{
    bool result   = true;
    callback_mess = callback;
    can->set_callback_message(callback);
    return result;
}
bool CanCommunication::set_callback_changed_mode(ChangedModeFunction callback)
{
    bool result = false;
    try {
        callback_changed_mode = callback;
        can->set_callback_changed_mode(&change_can_mode);
        result = true;
    } catch (...) {
    }
    return result;
}

bool CanCommunication::set_callback_received(GetReceivedFunction callback)
{
    bool result = false;
    try {
        can->set_callback_received(callback);
        result = true;
    } catch (...) {
    }
    return result;
}

bool CanCommunication::set_callback_setting_default(SettingDefaultFunction callback)
{
    bool result = false;
    try {
        this->callback_setting_default = callback;
        result                         = true;
    } catch (...) {
    }
    return result;
}

bool CanCommunication::begin()
{
    bool result = true;
    if (false == flag_thread_can_initialized) {
        flag_thread_can_initialized == true;
        this->setup_callback();
        this->setup_default();
        if (-1 != interrupt) {
            pinMode(interrupt, INPUT);
            attachInterrupt(interrupt, thread_can_on_interrupt, FALLING);
        }
        this->task_assigned_size = 4096;
        xTaskCreatePinnedToCore(thread_can, //
                                THREAD_NAME_CAN,
                                this->task_assigned_size,
                                NULL,
                                3,
                                &this->task_handle,
                                THREAD_CORE_CAN);
    }
    return result;
}

#if DEBUG_MODE
UBaseType_t CanCommunication::get_stack_size()
{
    return this->task_assigned_size;
}
UBaseType_t CanCommunication::get_stack_high_water_mark()
{
    return uxTaskGetStackHighWaterMark(this->task_handle);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////
CanCommunication::CanCommunication() : interrupt(CAN_COMMUNICATION_PIN_INTERRUPT)
{
    can = new CanCommunicationImpl();
    can->set_callback_sendable(std::bind(&CanCommunication::sendable, this, std::placeholders::_1, std::placeholders::_2));
}

CanCommunication::~CanCommunication()
{
    flag_thread_can_fin = true;
}
////////////////////////////////////////////////////////////////////////////////////////////
} // namespace CAN
} // namespace MaSiRoProject
