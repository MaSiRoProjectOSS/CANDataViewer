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
//#define CONTROLLERS_M5STACK
//#define CONTROLLERS_ATOM_LITE

#include <Arduino.h>
#ifdef CONTROLLERS_M5STACK
#include <M5Stack.h>
#else
// CONTROLLERS_ATOM_LITE
#include <M5Atom.h>
#endif
#include <can_data_viewer.hpp>

#define SETTING_WIFI_MODE_AP false
#define SETTING_WIFI_SSID    "(wifi ssid)"
#define SETTING_WIFI_PASS    "(wifi password)"

#define SETTING_LOOP_TIME_SLEEP_DETECT 100

#if LIB_CAN_DRIVER == 0
// ESP32CAN does not require Interrupt Pin and Chip select Pin
CanDataViewer can_data_viewer;
#else
// MCP2515 should be Pin set.
#ifdef CONTROLLERS_M5STACK
CanDataViewer can_data_viewer(G15, G12);
#else
// CONTROLLERS_ATOM_LITE
CanDataViewer can_data_viewer(G25, G19);
#endif
#endif

void setup_m5()
{
    M5.begin();
#ifdef CONTROLLERS_M5STACK
    M5.Power.begin();
#else
    // CONTROLLERS_ATOM_LITE
    M5.dis.begin();
    delay(1000);
    M5.dis.fillpix(CRGB::White);
#endif
    Serial.println("---------------------");
    Serial.printf("  App : %s\n", "CanDataViewer");
    Serial.printf("    Loop interval : %d ms\n", SETTING_LOOP_TIME_SLEEP_DETECT);
    Serial.println("---------------------");
}

void change_can_mode(CAN_CTRL_STATE mode, const char *text)
{
    char buffer[255];
#ifdef CONTROLLERS_M5STACK
#else
    // CONTROLLERS_ATOM_LITE
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
#endif
    log_i("CAN MODE [%s]", text);
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
    can_data_viewer.set_callback_changed_mode(&change_can_mode);
    can_data_viewer.set_callback_received(&received);
    can_data_viewer.set_callback_sendable_interrupt(&sendable);

    // Start CANDataViewer
    can_data_viewer.begin();
}

void loop()
{
    (void)M5.update();
#ifdef CONTROLLERS_M5STACK
    if (M5.BtnA.wasPressed()) {
        // Press the button to change the CAN output mode.
        can_data_viewer.set_mode();
    }
#else
    if (M5.Btn.wasPressed()) {
        // Press the button to change the CAN output mode.
        can_data_viewer.set_mode();
    }
#endif

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

        log_i("STACK SIZE : can[%d/%d] server[%d/%d]", //
              (max_can - stack_can),
              max_can,
              (max_server - stack_server),
              max_server);
    }
#endif
    (void)delay(SETTING_LOOP_TIME_SLEEP_DETECT);
}
