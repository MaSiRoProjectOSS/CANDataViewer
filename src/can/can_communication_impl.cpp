/**
 * @file can_communication.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-27
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "can_communication_impl.hpp"

#include "can_config.h"
#include "can_data_viewer_info.hpp"

namespace MaSiRoProject
{
namespace CAN
{

/////////////////////////////////
// Send
/////////////////////////////////
#pragma region Send
bool CanCommunicationImpl::clear_loop_shot()
{
    bool result = true;
    this->send_loop_list.clear();
    return result;
}
bool CanCommunicationImpl::delete_loop_shot(unsigned long Id)
{
    bool result           = true;
    bool flag_duplication = false;
    int index             = 0;
    for (CanData item : this->send_loop_list) {
        if (item.Id == Id) {
            flag_duplication = true;
            break;
        }
        index++;
    }
    if (true == flag_duplication) {
        this->send_loop_list.erase(std::begin(this->send_loop_list) + index);
    }
    return result;
}
bool CanCommunicationImpl::clear_resume()
{
    bool result = true;
    this->send_resume.clear();
    return result;
}

bool CanCommunicationImpl::delete_resume(unsigned long Id)
{
    bool result           = true;
    bool flag_duplication = false;
    int index             = 0;
    for (CanData item : this->send_resume) {
        if (item.Id == Id) {
            flag_duplication = true;
            break;
        }
        index++;
    }
    if (true == flag_duplication) {
        this->send_resume.erase(std::begin(this->send_resume) + index);
    }
    return result;
}
bool CanCommunicationImpl::add_one_shot(CanData data)
{
    bool result        = true;
    data.loop_interval = 0;
    this->send_one_shot_list.push_back(data);
    return result;
}
bool CanCommunicationImpl::add_loop_shot(CanData data, int interval)
{
    bool result = true;

    bool flag_duplication = false;
    int index             = 0;
    data.loop_interval    = interval;
    for (CanData item : this->send_loop_list) {
        if (item.Id == data.Id) {
            flag_duplication = true;
            break;
        }
        index++;
    }
    if (true == flag_duplication) {
        this->send_loop_list.erase(std::begin(this->send_loop_list) + index);
    }
    this->send_loop_list.push_back(data);

    return result;
}

std::vector<CanData> CanCommunicationImpl::get_loop_shot()
{
    std::vector<CanData> result_data;
    copy(this->send_loop_list.begin(), this->send_loop_list.end(), back_inserter(result_data));
    return result_data;
}
std::vector<CanData> CanCommunicationImpl::get_received()
{
    std::vector<CanData> result_data;
    copy(this->received_list.begin(), this->received_list.end(), back_inserter(result_data));
    return result_data;
}

std::vector<CanData> CanCommunicationImpl::get_resume()
{
    std::vector<CanData> result_data;
    copy(this->send_resume.begin(), this->send_resume.end(), back_inserter(result_data));
    return result_data;
}

CanDeviceInfo CanCommunicationImpl::get_device_info()
{
    CanDeviceInfo buffer        = this->can->get_device_info();
    this->device_info.can_type  = buffer.can_type;
    this->device_info.can_speed = buffer.can_speed;
    sprintf(this->device_info.can_speed_txt, buffer.can_speed_txt);
    return this->device_info;
}

bool CanCommunicationImpl::send_one_shot()
{
    bool result           = false;
    int index             = 0;
    unsigned long current = millis();

    for (CanData item : this->send_one_shot_list) {
        if (item.time < current) {
            if (true == this->send(item)) {
                result = true;
            }
            break;
        }
        index++;
    }
    if (true == result) {
        this->send_one_shot_list.erase(std::begin(this->send_one_shot_list) + index);
    }
    return result;
}

bool CanCommunicationImpl::send_loop_shot()
{
    bool result           = false;
    int index             = 0;
    unsigned long current = millis();
    CanData data;
    char buffer[255];

    for (CanData item : this->send_loop_list) {
        if (item.time < current) {
            data = item;
            if (true == this->send(data)) {
                result = true;
            }
            break;
        }
        index++;
    }
    if (true == result) {
        this->send_loop_list.erase(std::begin(this->send_loop_list) + index);
        do {
            data.time += data.loop_interval;
        } while (data.time <= current);

        this->send_loop_list.push_back(data);
    }
    return result;
}

bool CanCommunicationImpl::data_sendable(CAN_CTRL_STATE state)
{
    bool result = true;
    if (nullptr != this->callback_sendable) {
        CanData data;
        result = this->callback_sendable(state, &data);
        if (0 != data.Id) {
            data.loop_interval = 0;
            (void)this->send(data);
        }
    }
    return result;
}

bool CanCommunicationImpl::send(CanData data)
{
    bool result = this->can->send(data);
    if (true == result) {
        this->set_resume(data);
    }
#if OUTPUT_MESSAGE_FOR_SERIAL
    char buffer[255];
    sprintf(buffer,
            "SEND   "
            " : id = 0x%02lX / %02d / "
            "0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
            data.Id,
            data.Length,
            data.Data[0],
            data.Data[1],
            data.Data[2],
            data.Data[3],
            data.Data[4],
            data.Data[5],
            data.Data[6],
            data.Data[7]);

    happened_message(!result, buffer);
#endif

    return result;
}

bool CanCommunicationImpl::set_resume(CanData data)
{
    bool result           = true;
    bool flag_duplication = false;
    int index             = 0;
    data.time             = millis();
    for (CanData item : this->send_resume) {
        if (item.Id == data.Id) {
            flag_duplication = true;
            item.time        = millis();
            break;
        }
        index++;
    }
    if (true == flag_duplication) {
        this->send_resume.erase(std::begin(this->send_resume) + index);
    }
    this->send_resume.push_back(data);
    return result;
}
#pragma endregion

/////////////////////////////////
// Receive
/////////////////////////////////
#pragma region Receive
bool CanCommunicationImpl::interrupt()
{
    return this->can->interrupt();
}
#pragma endregion

/////////////////////////////////
// Request
/////////////////////////////////
#pragma region Request
void CanCommunicationImpl::request_start()
{
    this->mode_request = CAN_CTRL_STATE::MODE_READY;
}
void CanCommunicationImpl::request_run()
{
    this->mode_request = CAN_CTRL_STATE::MODE_RUNNING;
}
void CanCommunicationImpl::request_resume()
{
    if (true == this->flag_request_pause) {
        this->flag_request_pause = false;
    } else {
        this->mode_request = CAN_CTRL_STATE::MODE_READY;
    }
}
void CanCommunicationImpl::request_suspend()
{
    this->mode_request = CAN_CTRL_STATE::MODE_STOPPING;
}
void CanCommunicationImpl::request_sleep()
{
    this->mode_request = CAN_CTRL_STATE::MODE_INACTIVE;
}
void CanCommunicationImpl::request_stop()
{
    this->mode_request = CAN_CTRL_STATE::MODE_FINISHED;
}
void CanCommunicationImpl::request_pause()
{
    this->flag_request_pause = true;
}
#pragma endregion

/////////////////////////////////
// Callback
/////////////////////////////////
#pragma region Callback
bool CanCommunicationImpl::set_callback_sendable(SendEventFunction callback)
{
    bool result = false;
    try {
        this->callback_sendable = callback;
        result                  = true;
    } catch (...) {
    }
    return result;
}
bool CanCommunicationImpl::set_callback_message(MessageFunction callback)
{
    bool result = false;
    try {
        this->callback_message = callback;
        result                 = true;
    } catch (...) {
    }
    return result;
}
bool CanCommunicationImpl::set_callback_changed_mode(ChangedModeFunction callback)
{
    bool result = false;
    try {
        this->callback_changed_mode = callback;
        result                      = true;
    } catch (...) {
    }
    return result;
}

bool CanCommunicationImpl::set_callback_received(GetReceivedFunction callback)
{
    bool result = false;
    try {
        this->callback_received = callback;
        result                  = true;
    } catch (...) {
    }
    return result;
}

#pragma endregion

/////////////////////////////////
// Happened
/////////////////////////////////
#pragma region Happened
void CanCommunicationImpl::happened_received(CanData data)
{
    bool flag_insert = true;
    int index        = 0;
    for (CanData item : this->received_list) {
        if (item.Id == data.Id) {
            flag_insert = false;
            break;
        }
        index++;
    }
    if (false == flag_insert) {
        this->received_list.erase(std::begin(this->received_list) + index);
    }
    this->received_list.push_back(data);

    if (nullptr != this->callback_received) {
        this->callback_received(data);
    }
#if OUTPUT_MESSAGE_FOR_SERIAL
    if (0 != data.Id) {
        char buffer[255];
        sprintf(buffer,
                "RECEIVE"
                " : id = 0x%02lX / %02d / "
                "0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                data.Id,
                data.Length,
                data.Data[0],
                data.Data[1],
                data.Data[2],
                data.Data[3],
                data.Data[4],
                data.Data[5],
                data.Data[6],
                data.Data[7]);
        output_message(false, buffer, true);
    }
#endif
}
void CanCommunicationImpl::happened_message(bool is_error, const char *message)
{
    if (nullptr != this->callback_message) {
        this->callback_message(is_error, message, true);
    }
}
void CanCommunicationImpl::happened_changed_mode(CAN_CTRL_STATE mode)
{
    char buffer[255];
    switch (this->mode_current) {
        case CAN_CTRL_STATE::MODE_NOT_INITIALIZE:
            sprintf(buffer, "NOT INITIALIZE");
            break;
        case CAN_CTRL_STATE::MODE_READY:
            sprintf(buffer, "READY");
            break;
        case CAN_CTRL_STATE::MODE_RUNNING:
            sprintf(buffer, "RUNNING");
            break;
        case CAN_CTRL_STATE::MODE_INACTIVE:
            sprintf(buffer, "inactive");
            break;
        case CAN_CTRL_STATE::MODE_ABORT:
            sprintf(buffer, "abort");
            break;
        case CAN_CTRL_STATE::MODE_FINISHED:
            sprintf(buffer, "finished");
            break;
        case CAN_CTRL_STATE::MODE_STOPPING:
            sprintf(buffer, "stopping");
            break;
        case CAN_CTRL_STATE::MODE_PAUSE:
            sprintf(buffer, "pause");
            break;
        default:
            sprintf(buffer, "--");
            break;
    }
    this->device_info.mode = this->mode_current;
    sprintf(this->device_info.mode_text, buffer);
    if (nullptr != this->callback_changed_mode) {
        this->callback_changed_mode(mode, buffer);
    }
}
#pragma endregion

/////////////////////////////////
// Public
/////////////////////////////////
#pragma region Public
CAN_CTRL_STATE CanCommunicationImpl::get_mode()
{
    return this->mode_current;
}
bool CanCommunicationImpl::begin()
{
    bool result = false;
    result      = this->can->begin();
    if (true == result) {
#if DEBUG_MODE
        happened_message(false, "CanCommunication : MCP2515 Initialized Successfully!");
#endif
        initialized = true;
    } else {
        happened_message(true, "CanCommunication : Initializing MCP2515...");
    }
    this->happened_changed_mode(this->mode_current);
    return result;
}
bool CanCommunicationImpl::loop()
{
    bool result = true;
    if (true == flag_request_pause) {
    } else {
        bool flag_send = true;
        if (this->mode_current != this->mode_request) {
            this->mode_current = this->mode_request;
            this->happened_changed_mode(this->mode_current);
        }
        if (true == this->interrupt()) {
#if DEBUG_MODE
            happened_message(false, "CanCommunication : loop-receive");
#endif
        }
        switch (this->mode_current) {
            case CAN_CTRL_STATE::MODE_READY:
                if (true == this->data_sendable(CAN_CTRL_STATE::MODE_READY)) {
                    this->request_run();
                }
                break;
            case CAN_CTRL_STATE::MODE_RUNNING:
                if (false == this->send_one_shot()) {
                    if (false == this->send_loop_shot()) {
                        this->data_sendable(CAN_CTRL_STATE::MODE_RUNNING);
                    }
                }
                break;
            case CAN_CTRL_STATE::MODE_STOPPING:
                if (true == this->data_sendable(CAN_CTRL_STATE::MODE_STOPPING)) {
                    this->request_sleep();
                }
                break;
            case CAN_CTRL_STATE::MODE_NOT_INITIALIZE:
            case CAN_CTRL_STATE::MODE_INACTIVE:
            case CAN_CTRL_STATE::MODE_ABORT:
            case CAN_CTRL_STATE::MODE_FINISHED:
            default:
                flag_send = false;
                break;
        }
#if DEBUG_MODE
        if (true == flag_send) {
            if (true == result) {
                // do nothing
            } else {
                this->can->output_error();
            }
        }
#endif
        ////////////////////////////////////////////////////////
    }
    return result;
}
#pragma endregion

/////////////////////////////////
// Constructor
/////////////////////////////////
#pragma region Constructor
CanCommunicationImpl::CanCommunicationImpl()
        : mode_request(CAN_CTRL_STATE::MODE_NOT_INITIALIZE), mode_current(CAN_CTRL_STATE::MODE_NOT_INITIALIZE), initialized(false), flag_request_pause(false)
{
#if LIB_ESP32CAN
    this->can = new DriverEsp32can();
#else
    this->can = new DriverMcp2515();
#endif
    this->can->set_callback_get_received(std::bind(&CanCommunicationImpl::happened_received, this, std::placeholders::_1));
}
CanCommunicationImpl::~CanCommunicationImpl()
{
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////
} // namespace CAN
} // namespace MaSiRoProject