#include <Arduino.h>
#include <M5AtomS3.h>
#include <can_data_viewer.hpp>

#if LIB_CAN_DRIVER == 0
// ESP32CAN should be Pin set.
CanDataViewer can_data_viewer(CAN_COMMUNICATION_PIN_ESP32CAN_TX, CAN_COMMUNICATION_PIN_ESP32CAN_RX);
#else
CanDataViewer can_data_viewer(G1, G2);
#endif

const int UPDATE_DISPLAY_INTERVAL = 1000;
volatile int flag_update_display  = UPDATE_DISPLAY_INTERVAL;

void update_display()
{
    CushyWebServer::WEB_VIEWER_MODE display_mode = can_data_viewer.get_web_instance()->get_mode();
    int h                                        = AtomS3.Display.height();
    int w                                        = AtomS3.Display.width();
    int padding                                  = 2;
    AtomS3.Display.setTextSize(1.2);
    int text_height     = AtomS3.Display.fontHeight();
    int text_box_height = text_height + padding * 2;
    int pos             = 0;
    switch (display_mode) {
        case CushyWebServer::WEB_VIEWER_MODE::CONNECTED_STA:
            log_i("CONNECTED_STA");
            AtomS3.Display.setTextColor(BLACK);
            AtomS3.Display.fillRect(0, pos, w, text_box_height, DARKGREY);
            AtomS3.Display.setCursor(padding, padding);
            AtomS3.Display.println("A:0.0.0.0");
            pos += text_box_height;
            AtomS3.Display.fillRect(0, pos, w, text_box_height, GREEN);
            AtomS3.Display.setCursor(padding, pos + padding);
            AtomS3.Display.println("S:" + can_data_viewer.get_web_instance()->get_ip_address(CushyWebServer::NW_IF_WIFI_STA).toString());
            break;
        case CushyWebServer::WEB_VIEWER_MODE::CONNECTED_AP:
            log_i("CONNECTED_AP");
            AtomS3.Display.setTextColor(BLACK);
            AtomS3.Display.fillRect(0, pos, w, text_box_height, DARKGREY);
            AtomS3.Display.setCursor(padding, padding);
            AtomS3.Display.println("A:" + can_data_viewer.get_web_instance()->get_ip_address(CushyWebServer::NW_IF_WIFI_AP).toString());
            pos += text_box_height;
            AtomS3.Display.fillRect(0, pos, w, text_box_height, GREEN);
            AtomS3.Display.setCursor(padding, pos + padding);
            AtomS3.Display.println("S:0.0.0.0");
            break;
        case CushyWebServer::WEB_VIEWER_MODE::CONNECTED_AP_AND_STA:
            AtomS3.Display.setTextColor(BLACK);
            AtomS3.Display.fillRect(0, pos, w, text_box_height, DARKGREY);
            AtomS3.Display.setCursor(padding, padding);
            AtomS3.Display.println("A:" + can_data_viewer.get_web_instance()->get_ip_address(CushyWebServer::NW_IF_WIFI_AP).toString());
            pos += text_box_height;
            AtomS3.Display.fillRect(0, pos, w, text_box_height, GREEN);
            AtomS3.Display.setCursor(padding, pos + padding);
            AtomS3.Display.println("S:" + can_data_viewer.get_web_instance()->get_ip_address(CushyWebServer::NW_IF_WIFI_STA).toString());
            break;
        default:
            break;
    }
}
void notify_mode(CushyWebServer::WEB_VIEWER_MODE mode)
{
    flag_update_display = 0;
}

void setup()
{
    auto cfg          = M5.config();
    cfg.internal_imu  = false;
    cfg.clear_display = true;
    AtomS3.begin(cfg);
    AtomS3.dis.begin();
    delay(3000);

    AtomS3.Display.clear();
    AtomS3.Display.fillScreen(BLACK);
    Serial.println("Start CANDataViewer");

    can_data_viewer.get_web_instance()->set_callback_mode(&notify_mode);
    bool result = can_data_viewer.begin();

    if (false == result) {
        Serial.println("CANDataViewer begin failed");
        while (1) {
            delay(1000);
        }
    } else {
        Serial.println("CANDataViewer begin success");
    }
}

void loop()
{
    (void)AtomS3.update();
    if (true == AtomS3.BtnA.wasPressed()) {
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
        flag_update_display = UPDATE_DISPLAY_INTERVAL;
    }
    if (flag_update_display >= UPDATE_DISPLAY_INTERVAL) {
        flag_update_display = -1;
        update_display();
        log_i("Display update");
    } else if (flag_update_display >= 0) {
        flag_update_display++;
    } else {
    }

    (void)delay(1);
}
