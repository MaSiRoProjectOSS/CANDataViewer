/**
 * @file app_start.ino
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
    return true;
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
    static int SPAN               = (3 * 1000);
    static unsigned long next_run = millis();
    unsigned long current_time    = millis();
    (void)M5.update();
    ////////////////////////////////////////////////////////
    if (M5.Btn.wasPressed()) {
        ctr.set_mode();
    }
    if (next_run <= current_time) {
        do {
            next_run += SPAN;
        } while (next_run < current_time);

#if DEBUG_MODE
        UBaseType_t stack_can    = ctr.get_stack_high_water_mark_can();
        UBaseType_t stack_server = ctr.get_stack_high_water_mark_server();
        UBaseType_t max_can      = ctr.get_stack_size_can();
        UBaseType_t max_server   = ctr.get_stack_size_server();

        char msg_buffer[512];
        sprintf(msg_buffer,
                "STACK SIZE : can[%d/%d] server[%d/%d]", //
                (max_can - stack_can),
                max_can,
                (max_server - stack_server),
                max_server);
        output_message(false, msg_buffer);
#endif
    }
    ////////////////////////////////////////////////////////
    (void)delay(SETTING_LOOP_TIME_SLEEP_DETECT);
}
