/**
 * @file CanDataViewer_basic.ino
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2
 * @date 2022-12-25
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include <Arduino.h>
#include <M5Stack.h>
#include <can_data_viewer.hpp>

#define SETTING_LOOP_TIME_SLEEP_DETECT 100

CanDataViewer can_data_viewer(G15, G12);

void setup_m5()
{
    M5.begin();
    M5.Power.begin();
    delay(1000);
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
            break;
        case CAN_CTRL_STATE::MODE_READY:
            break;
        case CAN_CTRL_STATE::MODE_INACTIVE:
        case CAN_CTRL_STATE::MODE_FINISHED:
            break;
        case CAN_CTRL_STATE::MODE_NOT_INITIALIZE:
            break;
        case CAN_CTRL_STATE::MODE_ABORT:
        default:
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
    if (M5.BtnA.wasPressed()) {
        // Press the button to change the CAN output mode.
        can_data_viewer.set_mode();
    }
    (void)delay(SETTING_LOOP_TIME_SLEEP_DETECT);
}
