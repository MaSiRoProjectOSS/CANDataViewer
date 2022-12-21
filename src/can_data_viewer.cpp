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
#include "can_data_viewer.hpp"

MaSiRoProject::WEB::ControllerPage ctrl_page;
MaSiRoProject::CAN::CanCommunication ctrl_can;

std::vector<CanData> can_data_request_send(void)
{
    return ctrl_can.get_send_loop();
}
std::vector<CanData> can_data_request_received(void)
{
    return ctrl_can.get_received();
}
std::vector<CanData> can_data_request_resume(void)
{
    return ctrl_can.get_resume();
}
CanDeviceInfo get_can_device_info()
{
    return ctrl_can.get_device_info();
}

bool can_data_one_shot(CanData data)
{
    return ctrl_can.add_one_shot(data);
}
bool can_data_loop(CanData data)
{
    return ctrl_can.add_loop_shot(data, data.loop_interval);
}
bool can_set_mode(CAN_CTRL_STATE mode)
{
    ctrl_can.change_mode(mode);
    return true;
}
bool can_data_clear(int id)
{
    bool result = ctrl_can.request_pause();
    if (true == result) {
        result = ctrl_can.clear_loop_shot();
    }
    if (true == result) {
        result = ctrl_can.clear_resume();
    }
    ctrl_can.request_running();
    return result;
}
bool can_data_default(int id)
{
    bool result = ctrl_can.request_pause();
    if (true == result) {
        result = ctrl_can.clear_resume();
    }
    if (true == result) {
        result = ctrl_can.clear_loop_shot();
    }
    if (true == result) {
        result = ctrl_can.setup_default();
    }
    ctrl_can.request_running();

    return result;
}
bool can_data_delete(int id)
{
    bool result = ctrl_can.request_pause();
    if (true == result) {
        result = ctrl_can.delete_loop_shot(id);
    }
    if (true == result) {
        result = ctrl_can.delete_resume(id);
    }
    ctrl_can.request_running();
    return result;
}
bool CanDataViewer::set_mode(CAN_CTRL_STATE mode)
{
    return ctrl_can.change_mode(mode);
}
bool CanDataViewer::set_callback_message(MessageFunction callback)
{
    bool result = true;
    ctrl_page.set_callback_message(callback);
    ctrl_can.set_callback_message(callback);
    return result;
}
bool CanDataViewer::set_callback_changed_mode(ChangedModeFunction callback)
{
    bool result = true;
    ctrl_can.set_callback_changed_mode(callback);
    return result;
}

bool CanDataViewer::set_callback_received(GetReceivedFunction callback)
{
    bool result = true;
    ctrl_can.set_callback_received(callback);
    return result;
}

bool CanDataViewer::set_callback_setting_default(SettingDefaultFunction callback)
{
    bool result = true;
    ctrl_can.set_callback_setting_default(callback);
    return result;
}
bool CanDataViewer::set_wifi_info(std::string ssid, std::string pass, bool ap_mode)
{
    return ctrl_page.set_wifi_info(ssid, pass, ap_mode);
}

bool CanDataViewer::clear_resume(void)
{
    return ctrl_can.clear_resume();
}

bool CanDataViewer::add_one_shot(CanData data)
{
    return ctrl_can.add_one_shot(data);
}

bool CanDataViewer::set_resume(CanData data)
{
    return ctrl_can.set_resume(data);
}

bool CanDataViewer::add_loop_shot(CanData data, int interval)
{
    return ctrl_can.add_loop_shot(data, interval);
}

bool CanDataViewer::clear_loop_shot(void)
{
    return ctrl_can.clear_loop_shot();
}

CanDataViewer::CanDataViewer()
{
}

CanDataViewer::~CanDataViewer()
{
}
bool CanDataViewer::begin(std::string ssid, std::string pass, bool ap_mode)
{
    bool result = false;
    try {
        ////////////////////////////////////////////////////////
        ctrl_page.setup_callback(&can_data_request_send,
                                 &can_data_request_received,
                                 &can_data_request_resume,
                                 &get_can_device_info,
                                 &can_set_mode,
                                 &can_data_one_shot,
                                 &can_data_loop,
                                 &can_data_clear,
                                 &can_data_default,
                                 &can_data_delete);
        ctrl_page.setup();
        if ("" != ssid) {
            if ("" != pass) {
                this->set_wifi_info(ssid, pass, ap_mode);
            }
        }
        ctrl_page.begin();
        ////////////////////////////////////////////////////////
        ctrl_can.begin();
        result = true;
        ////////////////////////////////////////////////////////
    } catch (...) {
    }
    return result;
}