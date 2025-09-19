#include <Arduino.h>
#include <M5Atom.h>
#include <can_data_viewer.hpp>

#if LIB_CAN_DRIVER == 0
// ESP32CAN should be Pin set.
CanDataViewer can_data_viewer(CAN_COMMUNICATION_PIN_ESP32CAN_TX, CAN_COMMUNICATION_PIN_ESP32CAN_RX);
#else
// MCP2515 should be Pin set.
#ifdef CONTROLLERS_M5STACK
CanDataViewer can_data_viewer(G15, G12);
#else
// CONTROLLERS_ATOM_LITE
CanDataViewer can_data_viewer(G25, G19);
#endif
#endif

void setup()
{
    M5.begin(true, true, true);
    M5.dis.begin();
    M5.dis.fillpix(CRGB::Yellow);
    delay(3000);
    M5.dis.fillpix(CRGB::Green);
    Serial.println("Start CANDataViewer");

    bool result = can_data_viewer.begin();
    if (false == result) {
        M5.dis.fillpix(CRGB::Red);
        Serial.println("CANDataViewer begin failed");
        while (1) {
            delay(1000);
        }
    } else {
        M5.dis.fillpix(CRGB::Blue);
        Serial.println("CANDataViewer begin success");
    }
}

void loop()
{
    (void)M5.update();
    if (true == M5.Btn.wasPressed()) {
        UBaseType_t stack_cushy_server = can_data_viewer.get_stack_high_water_mark_server();
        UBaseType_t max_cushy_server   = can_data_viewer.get_stack_size_server();
        UBaseType_t stack_cushy_can    = can_data_viewer.get_stack_high_water_mark_can();
        UBaseType_t max_cushy_can      = can_data_viewer.get_stack_size_can();
        UBaseType_t stack_cushy_wifi   = can_data_viewer.get_stack_high_water_mark_wifi();
        UBaseType_t max_cushy_wifi     = can_data_viewer.get_stack_size_wifi();

        char msg_buffer[512];
        sprintf(msg_buffer,
                "STACK SIZE : Server[%d/%d] WiFi[%d/%d] CAN[%d/%d]", //
                (int)(max_cushy_server - stack_cushy_server),
                (int)max_cushy_server,
                (int)(max_cushy_wifi - stack_cushy_wifi),
                (int)max_cushy_wifi,
                (int)(max_cushy_can - stack_cushy_can),
                (int)max_cushy_can);
        log_i("%s", msg_buffer);
    }
    (void)delay(1);
}
