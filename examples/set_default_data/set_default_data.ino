/**
 * @file set_default_data.ino
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2
 * @date 2022-12-22
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include <M5Atom.h>
#include <can_data_viewer.hpp>

#ifndef SETTING_SYSTEM_NAME
#define SETTING_SYSTEM_NAME "CAN Data Viewer"
#endif
#ifndef SETTING_SYSTEM_VERSION_MAJOR
#define SETTING_SYSTEM_VERSION_MAJOR 1
#endif
#ifndef SETTING_SYSTEM_VERSION_MINER
#define SETTING_SYSTEM_VERSION_MINER 0
#endif
#ifndef SETTING_SETUP_SLEEP_ON_STARTUP
#define SETTING_SETUP_SLEEP_ON_STARTUP 1000
#endif
#ifndef SETTING_LOOP_TIME_SLEEP_DETECT
#define SETTING_LOOP_TIME_SLEEP_DETECT 100
#endif

void setup_m5()
{
    char buffer[255];
    bool enable_serial  = true;
    bool enable_i2c     = false;
    bool enable_display = true;
    M5.begin(enable_serial, enable_i2c, enable_display);
#if DEBUG_MODE
    delay(SETTING_SETUP_SLEEP_ON_STARTUP);
#endif
    M5.dis.begin();
    M5.dis.fillpix(CRGB::White);
    Serial.println("---------------------");
    sprintf(buffer, "  App : %s Ver. %d.%d", SETTING_SYSTEM_NAME, SETTING_SYSTEM_VERSION_MAJOR, SETTING_SYSTEM_VERSION_MINER);
    Serial.println(buffer);
    sprintf(buffer, "   Loop interval : %d ms", SETTING_LOOP_TIME_SLEEP_DETECT);
    Serial.println(buffer);
    Serial.println("---------------------");
}

void output_message(bool message_type_is_error, const char *message, bool add_header = true)
{
    if (true == add_header) {
        char buffer[300];
        unsigned long tm    = millis();
        unsigned long tm_s  = tm / 1000;
        unsigned long tm_ms = tm % 1000;

        if (true == message_type_is_error) {
            sprintf(buffer, "[Error] [%7ld.%03ld] : %s", tm_s, tm_ms, message);
        } else {
            sprintf(buffer, "[     ] [%7ld.%03ld] : %s", tm_s, tm_ms, message);
        }
        Serial.println(buffer);
    } else {
        Serial.println(message);
    }
}
///////////////////////////////////////////////////////////////////////////////////

CanDataViewer ctr;
void change_can_mode(CAN_CTRL_STATE mode, const char *text)
{
    char buffer[255];
    switch (mode) {
        case CAN_CTRL_STATE::MODE_RUNNING:
            M5.dis.fillpix(CRGB::Green);
            break;
        case CAN_CTRL_STATE::MODE_READY:
            M5.dis.fillpix(CRGB::GreenYellow);
            break;
        case CAN_CTRL_STATE::MODE_INACTIVE:
        case CAN_CTRL_STATE::MODE_FINISHED:
            M5.dis.fillpix(CRGB::Black);
            break;
        case CAN_CTRL_STATE::MODE_NOT_INITIALIZE:
            M5.dis.fillpix(CRGB::Yellow);
            break;
        case CAN_CTRL_STATE::MODE_ABORT:
        default:
            M5.dis.fillpix(CRGB::Red);
            break;
    }
    sprintf(buffer, "CAN MODE [%s]", text);
    output_message(false, buffer);
}

void received(CanData data)
{
}

bool setting_default(void)
{
    bool result = true;
    ///////////////////////////////////////////////////////////////////
    CanData data;
    data.Id            = 0x20;
    data.Length        = 0x08;
    data.Data[0]       = 0x10 + random(1, 9);
    data.Data[1]       = 0x20 + random(1, 9);
    data.Data[2]       = 0x30 + random(1, 9);
    data.Data[3]       = 0x40 + random(1, 9);
    data.Data[4]       = 0x50 + random(1, 9);
    data.Data[5]       = 0x60 + random(1, 9);
    data.Data[6]       = 0x70 + random(1, 9);
    data.loop_interval = 1000;
    ///////////////////////////////////////////////////////////////////
    ctr.clear_loop_shot();
    ///////////////////////////////////////////////////////////////////
    data.Id            = 0x20;
    data.Data[0]       = data.Id + random(1, 9);
    data.loop_interval = 1000;
    ctr.add_loop_shot(data, data.loop_interval);
    ///////////////////////////////////////////////////////////////////
    data.Id            = 0x30;
    data.Data[0]       = data.Id + random(1, 9);
    data.loop_interval = 0;
    ctr.add_one_shot(data);
    ///////////////////////////////////////////////////////////////////
    data.Id            = 0x40;
    data.Data[0]       = data.Id + random(1, 9);
    data.loop_interval = 0;
    ctr.add_resume(data);
    ///////////////////////////////////////////////////////////////////
    return result;
}

///////////////////////////////////////////////////////////////////////////////////
void setup_can_data_viewer()
{
    ctr.set_callback_message(&output_message);
    ctr.set_callback_changed_mode(&change_can_mode);
    ctr.set_callback_received(&received);
    ctr.set_callback_setting_default(&setting_default);
    ctr.begin();
}

void setup()
{
    setup_m5();
    setup_can_data_viewer();
}

void loop()
{
    (void)M5.update();
    ////////////////////////////////////////////////////////
    if (M5.Btn.wasPressed()) {
        ctr.set_mode();
    }
    ////////////////////////////////////////////////////////
    (void)delay(SETTING_LOOP_TIME_SLEEP_DETECT);
}