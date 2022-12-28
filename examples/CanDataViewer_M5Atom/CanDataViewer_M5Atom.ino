/**
 * @file CanDataViewer_M5Atom.ino
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2
 * @date 2022-12-28
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include <Arduino.h>
#include <M5Atom.h>
#include <can_data_viewer.hpp>

#define SETTING_LOOP_TIME_SLEEP_DETECT 100

CanDataViewer can_data_viewer(G25, G19);

void setup_m5()
{
    bool enable_serial  = true;
    bool enable_i2c     = false;
    bool enable_display = true;
    M5.begin(enable_serial, enable_i2c, enable_display);
    M5.dis.begin();
    delay(1000);
    M5.dis.fillpix(CRGB::White);
    Serial.println("---------------------");
    Serial.printf("  App : %s\n", "CanDataViewer");
    Serial.printf("    Loop interval : %d ms\n", SETTING_LOOP_TIME_SLEEP_DETECT);
    Serial.println("---------------------");
}
void output_message(OUTPUT_LOG_LEVEL level, const char *message, const char *function_name, const char *file_name, int line)
{
    char buffer[300];
    unsigned long tm    = millis();
    unsigned long tm_s  = tm / 1000;
    unsigned long tm_ms = tm % 1000;

    if (level >= OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_WARN) {
        sprintf(buffer, "[Error] [%7ld.%03ld][%s:%s:%d] : %s", tm_s, tm_ms, file_name, function_name, line, message);
    } else {
        sprintf(buffer, "[     ] [%7ld.%03ld] : %s", tm_s, tm_ms, message);
    }
    Serial.println(buffer);
}
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
    output_message(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_INFO, buffer, __func__, __FILENAME__, __LINE__);
}

void setup()
{
    setup_m5();
    can_data_viewer.set_callback_message(&output_message);
    can_data_viewer.set_callback_changed_mode(&change_can_mode);
    can_data_viewer.begin();
}

void loop()
{
    (void)M5.update();
    if (M5.Btn.wasPressed()) {
        // Press the button to change the CAN output mode.
        can_data_viewer.set_mode();
    }
    (void)delay(SETTING_LOOP_TIME_SLEEP_DETECT);
}
