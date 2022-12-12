/**
 * @file main.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-27
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "controller_can.hpp"
#include "controller_page.hpp"

#include <M5Atom.h>
#include <common/common_function.hpp>
#include <conf/pin_setting.h>
#include <conf/setting.h>
using namespace MaSiRoProject::ToyBox;

///////////////////////////////////////////////////////////////////
void output_message(bool message_type_is_error, const char *message, bool add_header = true)
{
    CommonFunction::output_message(message_type_is_error, message, add_header);
}
///////////////////////////////////////////////////////////////////
ControllerPage ctrl_page;
///////////////////////////////////////////////////////////////////
ControllerCan ctrl_can;
void change_can_mode(CAN::CAN_CTRL_STATE mode, const char *text)
{
    char buffer[255];
    switch (mode) {
        case CAN::CAN_CTRL_STATE::MODE_RUNNING:
            M5.dis.fillpix(CRGB::Green);
            break;
        case CAN::CAN_CTRL_STATE::MODE_READY:
            M5.dis.fillpix(CRGB::GreenYellow);
            break;
        case CAN::CAN_CTRL_STATE::MODE_INACTIVE:
        case CAN::CAN_CTRL_STATE::MODE_FINISHED:
            M5.dis.fillpix(CRGB::Black);
            break;
        case CAN::CAN_CTRL_STATE::MODE_NOT_INITIALIZE:
            M5.dis.fillpix(CRGB::Yellow);
            break;
        case CAN::CAN_CTRL_STATE::MODE_ABORT:
        default:
            M5.dis.fillpix(CRGB::Red);
            break;
    }
    sprintf(buffer, "CAN MODE [%s]", text);
    output_message(false, buffer);
}

std::vector<CAN::CanData> can_data_request_send(void)
{
    return ctrl_can.get_send_loop();
}
std::vector<CAN::CanData> can_data_request_received(void)
{
    return ctrl_can.get_received();
}
std::vector<CAN::CanData> can_data_request_resume(void)
{
    return ctrl_can.get_resume();
}
CAN::CanDeviceInfo get_can_device_info()
{
    return ctrl_can.get_device_info();
}

bool can_data_one_shot(CAN::CanData data)
{
    return ctrl_can.add_one_shot(data);
}
bool can_data_loop(CAN::CanData data)
{
    return ctrl_can.add_loop_shot(data, data.loop_interval);
}
bool can_set_mode(CAN::CAN_CTRL_STATE mode)
{
    ctrl_can.push(mode);
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

///////////////////////////////////////////////////////////////////
void setup()
{
    char buffer[255];
    ///////////
    bool enable_serial  = true;
    bool enable_i2c     = false;
    bool enable_display = true;
    M5.begin(enable_serial, enable_i2c, enable_display);
#if DEBUG_MODE
    delay(SETTING_SETUP_SLEEP_ON_STARTUP);
#endif
    ////////////////////////////////////////////////////////
    M5.dis.begin();
    M5.dis.fillpix(CRGB::White);
    ////////////////////////////////////////////////////////
    Serial.println("---------------------");
    sprintf(buffer, "  App : %s Ver. %d.%d", SETTING_SYSTEM_NAME, SETTING_SYSTEM_VERSION_MAJOR, SETTING_SYSTEM_VERSION_MINER);
    Serial.println(buffer);
    sprintf(buffer, "   Loop interval : %d ms", SETTING_LOOP_TIME_SLEEP_DETECT);
    Serial.println(buffer);
    Serial.println("---------------------");
    bool flag_setup = false;
    while (false == flag_setup) {
        try {
            ////////////////////////////////////////////////////////
            ctrl_page.set_callback_message(&output_message);
            ctrl_page.setup_callback(&output_message,
                                     &can_data_request_send,
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
            ctrl_page.begin();
            ////////////////////////////////////////////////////////
            ctrl_can.setup(&output_message, &change_can_mode);
            ctrl_can.begin();
            flag_setup = true;
            ////////////////////////////////////////////////////////
        } catch (...) {
            Serial.println("ERROR");
        }
        if (false == flag_setup) {
            Serial.println("Setup retry");
            delay(SETTING_SETUP_SLEEP_ON_STARTUP);
        }
    }
}

void loop()
{
    (void)M5.update();
    ////////////////////////////////////////////////////////
    if (M5.Btn.wasPressed()) {
        can_set_mode(MaSiRoProject::ToyBox::CAN::CAN_CTRL_STATE::MODE_UNKNOW);
    }
    ////////////////////////////////////////////////////////
    (void)delay(SETTING_LOOP_TIME_SLEEP_DETECT);
}
