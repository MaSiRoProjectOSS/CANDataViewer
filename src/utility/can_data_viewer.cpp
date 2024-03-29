/**
 * @file can_data_viewer.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-20
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "can/can_communication.hpp"
#include "web/controller_page.hpp"

#include <can_data_viewer.hpp>

namespace MaSiRoProject
{
MaSiRoProject::WEB::ControllerPage *ctrl_page;
MaSiRoProject::CAN::CanCommunication *ctrl_can;
} // namespace MaSiRoProject
using namespace MaSiRoProject;

////////////////////////////////////////////////
// setup function
////////////////////////////////////////////////
#pragma region setup_function
bool CanDataViewer::set_callback_changed_mode(ChangedModeFunction callback)
{
    bool result = true;
    ctrl_can->set_callback_changed_mode(callback);
    return result;
}

bool CanDataViewer::set_callback_received(GetReceivedFunction callback)
{
    return ctrl_can->set_callback_received(callback);
}
bool CanDataViewer::set_callback_sendable_interrupt(SendEventFunction callback)
{
    return ctrl_can->set_callback_sendable(callback);
}

bool CanDataViewer::set_callback_setting_default(SettingDefaultFunction callback)
{
    bool result = true;
    ctrl_can->set_callback_setting_default(callback);
    return result;
}
#pragma endregion

////////////////////////////////////////////////
// control function
////////////////////////////////////////////////
#pragma region control_function
bool CanDataViewer::set_mode(CAN_CTRL_STATE mode)
{
    return ctrl_can->change_mode(mode);
}

std::vector<CanData> can_data_request_send(void)
{
    return ctrl_can->get_send_loop();
}
std::vector<CanData> can_data_request_received(void)
{
    return ctrl_can->get_received();
}
std::vector<CanData> can_data_request_resume(void)
{
    return ctrl_can->get_resume();
}
CanDeviceInfo get_can_device_info()
{
    return ctrl_can->get_device_info();
}

bool can_data_one_shot(CanData data)
{
    return ctrl_can->add_one_shot(data);
}
bool can_data_loop(CanData data)
{
    return ctrl_can->add_loop_shot(data, data.loop_interval);
}
bool can_set_mode(CAN_CTRL_STATE mode)
{
    ctrl_can->change_mode(mode);
    return true;
}
bool can_data_clear(int id)
{
    bool result = ctrl_can->request_pause();
    if (true == result) {
        result = ctrl_can->clear_loop_shot();
    }
    if (true == result) {
        result = ctrl_can->clear_resume();
    }
    ctrl_can->request_running();
    return result;
}
bool can_data_default(int id)
{
    bool result = ctrl_can->request_pause();
    if (true == result) {
        result = ctrl_can->clear_resume();
    }
    if (true == result) {
        result = ctrl_can->clear_loop_shot();
    }
    if (true == result) {
        result = ctrl_can->setup_default();
    }
    ctrl_can->request_running();

    return result;
}
bool can_data_delete(int id)
{
    bool result = ctrl_can->request_pause();
    if (true == result) {
        result = ctrl_can->delete_loop_shot(id);
    }
    if (true == result) {
        result = ctrl_can->delete_resume(id);
    }
    ctrl_can->request_running();
    return result;
}
bool CanDataViewer::clear_resume(void)
{
    return ctrl_can->clear_resume();
}

bool CanDataViewer::add_one_shot(CanData data)
{
    return ctrl_can->add_one_shot(data);
}

bool CanDataViewer::add_resume(CanData data)
{
    return ctrl_can->add_resume(data);
}

bool CanDataViewer::add_loop_shot(CanData data, int interval)
{
    return ctrl_can->add_loop_shot(data, interval);
}

bool CanDataViewer::clear_loop_shot(void)
{
    return ctrl_can->clear_loop_shot();
}
#pragma endregion

////////////////////////////////////////////////
// debug function
////////////////////////////////////////////////
#pragma region debug_function
#if DEBUG_MODE
UBaseType_t CanDataViewer::get_stack_high_water_mark_can()
{
    return ctrl_can->get_stack_high_water_mark();
}
UBaseType_t CanDataViewer::get_stack_high_water_mark_server()
{
    return ctrl_page->get_stack_high_water_mark();
}
UBaseType_t CanDataViewer::get_stack_size_can()
{
    return ctrl_can->get_stack_size();
}
UBaseType_t CanDataViewer::get_stack_size_server()
{
    return ctrl_page->get_stack_size();
}
#endif
#pragma endregion

////////////////////////////////////////////////
// standard function
////////////////////////////////////////////////
#pragma region standard_function

CanDataViewer::CanDataViewer(const uint8_t interrupt, const uint8_t cs)
{
    ctrl_page = new MaSiRoProject::WEB::ControllerPage();
    ctrl_can  = new MaSiRoProject::CAN::CanCommunication(interrupt, cs);
}

CanDataViewer::~CanDataViewer()
{
}
bool CanDataViewer::begin()
{
    bool result = false;
    try {
        ////////////////////////////////////////////////////////
        ctrl_page->setup_callback(&can_data_request_send,
                                  &can_data_request_received,
                                  &can_data_request_resume,
                                  &get_can_device_info,
                                  &can_set_mode,
                                  &can_data_one_shot,
                                  &can_data_loop,
                                  &can_data_clear,
                                  &can_data_default,
                                  &can_data_delete);
        ctrl_page->begin();
        ////////////////////////////////////////////////////////
        ctrl_can->begin();
        result = true;
        ////////////////////////////////////////////////////////
    } catch (...) {
    }
    return result;
}
#pragma endregion
