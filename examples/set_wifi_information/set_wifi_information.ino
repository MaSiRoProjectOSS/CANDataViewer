/**
 * @file set_wifi_information.ino
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2
 * @date 2022-12-22
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#define SETTING_WIFI_MODE_AP false
#define SETTING_WIFI_SSID    "(wifi ssid)"
#define SETTING_WIFI_PASS    "(wifi password)"

#include <Arduino.h>
#include <M5Atom.h>
#include <SPIFFS.h>
#include <can_data_viewer.hpp>
#include <web/web_communication_impl.hpp>

MaSiRoProject::WEB::WebCommunicationImpl web_comm;

///////////////////////////////////////////////////////////////////////////////////
void output_message(bool message_type_is_error, const char *message, bool add_header = true)
{
    Serial.println(message);
}
void readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.println("DATA: ");
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
    Serial.println("");
}
void deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file: %s\n", path);
    if (true == SPIFFS.exists(path)) {
        if (fs.remove(path)) {
            Serial.println("File deleted");
        } else {
            Serial.println("Delete failed");
        }
    } else {
        Serial.println("Not found deleted file");
    }
}
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        Serial.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("(");
            Serial.print(file.path());
            Serial.print(")");
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
///////////////////////////////////////////////////////////////////////////////////

void setup()
{
    // --------------------------------------
    std::string ssid  = SETTING_WIFI_SSID;
    std::string pass  = SETTING_WIFI_PASS;
    bool ap_mode      = SETTING_WIFI_MODE_AP;
    bool is_save      = true;
    bool reconnecting = false;
    // --------------------------------------
    char buffer[255];
    bool enable_serial  = true;
    bool enable_i2c     = false;
    bool enable_display = true;
    M5.begin(enable_serial, enable_i2c, enable_display);

    M5.dis.begin();
    M5.dis.fillpix(CRGB::White);

    web_comm.set_callback_message(&output_message);

    delay(2500);
    Serial.println("======================================");
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        M5.dis.fillpix(CRGB::Red);
    } else {
#if 0
        deleteFile(SPIFFS, SETTING_WIFI_INFOMATION);
#endif
        /////////////////////////////////////////
        listDir(SPIFFS, "/", 0);
        SPIFFS.end();
        /////////////////////////////////////////
        web_comm.setup();
        if (true != web_comm.save_information(ssid, pass, ap_mode, reconnecting, is_save)) {
            Serial.println("[ERROR] web_comm.save_information");
            M5.dis.fillpix(CRGB::Yellow);
        } else {
            Serial.println("[     ] save information");
            M5.dis.fillpix(CRGB::Green);
        }
    }
    Serial.println("======================================");
}

void loop()
{
    (void)M5.update();
    if (M5.Btn.wasPressed()) {
        Serial.println("======================================");
        if (!SPIFFS.begin()) {
            Serial.println("SPIFFS Mount Failed");
            M5.dis.fillpix(CRGB::Red);
        } else {
            listDir(SPIFFS, "/", 0);
            if (true == SPIFFS.exists(SETTING_WIFI_INFOMATION)) {
                Serial.println("--------------------------------------");
                readFile(SPIFFS, SETTING_WIFI_INFOMATION);
                M5.dis.fillpix(CRGB::Green);
            } else {
                Serial.println("[ERROR] Not found file.");
                M5.dis.fillpix(CRGB::Yellow);
            }
            SPIFFS.end();
        }
    }
    (void)delay(100);
}
