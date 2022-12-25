/**
 * @file main.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief main function on CanDataViewer_PlatformIO
 * @version 0.2
 * @date 2022-12-25
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include <Arduino.h>
#include <M5Atom.h>
#include <can_data_viewer.hpp>

#define SETTING_WIFI_MODE_AP false
#define SETTING_WIFI_SSID    "(wifi ssid)"
#define SETTING_WIFI_PASS    "(wifi password)"

#define SETTING_LOOP_TIME_SLEEP_DETECT 100

CanDataViewer can_data_viewer;

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

    if (level <= OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_WARN) {
        sprintf(buffer, "[Error] [%7ld.%03ld][%s:%s:%d] : %s", tm_s, tm_ms, file_name, function_name, line, message);
    } else {
        sprintf(buffer, "[     ] [%7ld.%03ld][%s:%s:%d] : %s", tm_s, tm_ms, file_name, function_name, line, message);
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

/**
 * @brief Set default settings.
 *
 * @return true     setting is finished
 * @return false    did not set.
 */
bool setting_default(void)
{
    bool result = true;
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
    // Clears CAN data that is sent at regular intervals.
    can_data_viewer.clear_loop_shot();
    ///////////////////////////////////////////////////////////////////
    // Clears the list of CanData displayed in the browser.
    can_data_viewer.clear_resume();
    ///////////////////////////////////////////////////////////////////
    // Add CAN data to be sent at regular intervals.
    data.Id            = 0x20;
    data.Data[0]       = data.Id + random(1, 9);
    data.loop_interval = 1000;
    can_data_viewer.add_loop_shot(data, data.loop_interval);
    ///////////////////////////////////////////////////////////////////
    // Add CAN data to be sent only once.
    data.Id            = 0x30;
    data.Data[0]       = data.Id + random(1, 9);
    data.loop_interval = 0;
    can_data_viewer.add_one_shot(data);
    ///////////////////////////////////////////////////////////////////
    // Add to the list displayed on the browser.
    data.Id            = 0x40;
    data.Data[0]       = data.Id + random(1, 9);
    data.loop_interval = 0;
    can_data_viewer.add_resume(data);
    ///////////////////////////////////////////////////////////////////
    return result;
}

/**
 * @brief Receive received data
 *
 * @param data
 */
void received(CanData data)
{
}

/**
 * @brief   Notification of when transmission interrupts are possible
 *
 * @param mode      Current mode
 * @param data      Data to be sent at this timing
 * @return true     Set data
 * @return false    Not set
 */
bool sendable(CAN_CTRL_STATE mode, CanData *data)
{
    bool result                   = false;
    static int SPAN               = (1 * 1000);
    static unsigned long next_run = millis();
    unsigned long current_time    = millis();
    if (next_run <= current_time) {
        do {
            next_run += SPAN;
        } while (next_run < current_time);
        switch (mode) {
            case CAN_CTRL_STATE::MODE_RUNNING:
                ////////////////////////////////////////////
                data->Id            = 0xA0;
                data->Length        = 0x08;
                data->Data[0]       = 0x10 + random(1, 9);
                data->Data[1]       = 0x20 + random(1, 9);
                data->Data[2]       = 0x30 + random(1, 9);
                data->Data[3]       = 0x40 + random(1, 9);
                data->Data[4]       = 0x50 + random(1, 9);
                data->Data[5]       = 0x60 + random(1, 9);
                data->Data[6]       = 0x70 + random(1, 9);
                data->loop_interval = 0;
                ////////////////////////////////////////////
                result = true;
                break;

            default:
                break;
        }
    }
    return result;
}

void setup()
{
    // setup on M5 ATOM
    setup_m5();

    // setup callback function
    can_data_viewer.set_callback_setting_default(&setting_default);
    can_data_viewer.set_callback_message(&output_message);
    can_data_viewer.set_callback_changed_mode(&change_can_mode);
    can_data_viewer.set_callback_received(&received);
    can_data_viewer.set_callback_sendable_interrupt(&sendable);

    // Change IP configuration settings
    if (true == SETTING_WIFI_MODE_AP) {
        can_data_viewer.config_address_ap();
    } else {
        can_data_viewer.config_address_sta();
    }

    // Start Can-Data-Viewer
    can_data_viewer.begin(SETTING_WIFI_SSID, SETTING_WIFI_PASS, SETTING_WIFI_MODE_AP);

    // Switch connection information programmatically
    can_data_viewer.set_wifi_info(SETTING_WIFI_SSID, SETTING_WIFI_PASS, SETTING_WIFI_MODE_AP);
}

void loop()
{
    (void)M5.update();
    if (M5.Btn.wasPressed()) {
        // Press the button to change the CAN output mode.
        can_data_viewer.set_mode();
    }

#if DEBUG_MODE
    static int SPAN               = (3 * 1000);
    static unsigned long next_run = millis();
    unsigned long current_time    = millis();
    if (next_run <= current_time) {
        do {
            next_run += SPAN;
        } while (next_run < current_time);
        UBaseType_t stack_can    = can_data_viewer.get_stack_high_water_mark_can();
        UBaseType_t stack_server = can_data_viewer.get_stack_high_water_mark_server();
        UBaseType_t max_can      = can_data_viewer.get_stack_size_can();
        UBaseType_t max_server   = can_data_viewer.get_stack_size_server();

        char msg_buffer[512];
        sprintf(msg_buffer,
                "STACK SIZE : can[%d/%d] server[%d/%d]", //
                (max_can - stack_can),
                max_can,
                (max_server - stack_server),
                max_server);
        output_message(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_INFO, msg_buffer, __func__, __FILENAME__, __LINE__);
    }
#endif
    (void)delay(SETTING_LOOP_TIME_SLEEP_DETECT);
}
