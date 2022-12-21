/**
 * @file web_communication_impl.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "web_communication_impl.hpp"

#if STORAGE_SPI_FS
#include <SPIFFS.h>
#endif
namespace MaSiRoProject
{
namespace WEB
{
#define CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS   (100)
#define CONTROLLER_PAGE_STARTUP_STA_TIMEOUT_MS (50 * CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS)

////////////////////////////////////////////////////////////////////////////////////////////////
bool WebCommunicationImpl::set_callback_message(MessageFunction callback)
{
    bool result = false;
    try {
        this->callback_message = callback;
        result                 = true;
    } catch (...) {
    }
    return result;
}
void WebCommunicationImpl::happened_message(bool is_error, const char *message)
{
    if (nullptr != this->callback_message) {
        this->callback_message(is_error, message, true);
    }
}

bool WebCommunicationImpl::setup()
{
    bool result = true;
#if STORAGE_SPI_FS
    bool skip = false;
    if (SPIFFS.begin()) {
        result = SPIFFS.exists(SETTING_WIFI_INFOMATION);
        SPIFFS.end();
    } else {
#if STORAGE_SPI_FS_FORMAT
        this->happened_message(false, "SPIFFS format start...");
        SPIFFS.format();
        this->happened_message(false, "SPIFFS format finish");
        result = false;
#else
        this->happened_message(true, "SPIFFS Failed to Begin. You need to Run SPIFFS_Add.ino first");
        skip = true;
#endif
    }
    if (false == skip) {
        this->open_fs = true;
        if (false == result) {
            result = this->save_information(this->_ssid, this->_pass, this->_mode_ap, false);
        } else {
#if STORAGE_SPI_FS_INITIALIZE
            result = this->save_information(this->_ssid, this->_pass, this->_mode_ap, false);
#else
            result = this->load_information();
#endif
        }
    }

#endif
    return result;
}
bool WebCommunicationImpl::save_information(std::string ssid, std::string pass, bool ap_mode, bool reconnecting = true)
{
    bool result = false;
#if STORAGE_SPI_FS
    if (true == this->open_fs) {
#if DEBUG_MODE
        this->happened_message(false, "SPIFFS Begin.");
#endif
        if (SPIFFS.begin()) {
            if (true == SPIFFS.exists(SETTING_WIFI_INFOMATION)) {
                this->load_information();
            }
            if ((this->_ssid != ssid) || (this->_pass != pass) || (this->_mode_ap != ap_mode)) {
                File dataFile = SPIFFS.open(SETTING_WIFI_INFOMATION, FILE_WRITE);
                dataFile.println((true == ap_mode) ? "A" : "S");
                dataFile.println(ssid.c_str());
                dataFile.println(pass.c_str());
                dataFile.close();
                result = true;
            }
            SPIFFS.end();
        }
#if DEBUG_MODE
        if (true == result) {
            this->happened_message(false, "Finished Writing data to SPIFFS");
        } else {
            this->happened_message(false, "Finished SPIFFS");
        }
#endif
    }

    result = true;
#endif
    this->_mode_ap = ap_mode;
    this->_ssid    = ssid;
    this->_pass    = pass;
    if (true == reconnecting) {
        WiFi.reconnect();
        result = this->reconnect();
    }
    return result;
}
bool WebCommunicationImpl::load_information()
{
    bool result = false;
#if STORAGE_SPI_FS
    if (true == this->open_fs) {
#if DEBUG_MODE
        this->happened_message(false, "SPIFFS Begin.");
#endif
        static int BUF_SIZE   = 255;
        char buffer[BUF_SIZE] = { 0 };
        int totalBytes        = 0;
        int ne                = 0;
        int type              = 0;
        bool previous_type    = 0;
        std::string words;
        if (SPIFFS.begin()) {
            ne            = 1;
            File dataFile = SPIFFS.open(SETTING_WIFI_INFOMATION, FILE_READ);
            if (!dataFile) {
                ne     = 2;
                result = false;
            } else {
                ne         = 3;
                result     = true;
                totalBytes = dataFile.size();
                words.clear();
                while (dataFile.available()) {
                    String word = dataFile.readStringUntil('\r');
                    String nr   = dataFile.readStringUntil('\n');
                    type++;
                    if (3 < type) {
                        break;
                    }
                    if (previous_type != type) {
                        switch (type) {
                            case 1:
                                if (true == word.equals("S")) {
                                    this->_mode_ap = false;
                                } else {
                                    this->_mode_ap = true;
                                }
                                break;
                            case 2:
                                this->_ssid = word.c_str();
                                break;
                            case 3:
                                this->_pass = word.c_str();
                                break;

                            default:
                                break;
                        }
                        previous_type = type;
                    }
                }
                dataFile.close();
            }
            SPIFFS.end();
        }
#if DEBUG_MODE
        this->happened_message(false, "Finished Writing data to SPIFFS");
#endif
    }
#endif
    return result;
}

IPAddress WebCommunicationImpl::get_ip()
{
    if (true == this->_mode_ap) {
        return WiFi.softAPIP();
    } else {
        return WiFi.localIP();
    }
}
const char *WebCommunicationImpl::get_ssid()
{
    return this->_ssid.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool WebCommunicationImpl::reconnect()
{
    bool result = true;
    ///////////////////////////
    int countdown = CONTROLLER_PAGE_STARTUP_STA_TIMEOUT_MS;
    while (WiFi.status() != WL_CONNECTED) {
        delay(CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS);
        countdown -= CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS;
        if (0 >= countdown) {
            result = false;
            break;
        }
    }
    ///////////////////////////
    char buffer[255];
    if (WiFi.status() != WL_CONNECTED) {
        sprintf(buffer, //
                "WebCommunication : is NOT Connected : SSID [%s] / IP [%s]",
                this->_ssid.c_str(),
                this->get_ip().toString().c_str());
        this->happened_message(true, buffer);
        result = false;
    } else {
        sprintf(buffer,
                "WebCommunication : SSID [%s] / IP [%s]", //
                this->_ssid.c_str(),
                this->get_ip().toString().c_str());
        this->happened_message(false, buffer);
    }
    return result;
}

bool WebCommunicationImpl::begin()
{
    bool result = true;
    ///////////////////////////
    if (true == this->_mode_ap) {
        result = WiFi.softAP(this->_ssid.c_str(), this->_pass.c_str());
    } else {
        WiFi.begin(this->_ssid.c_str(), this->_pass.c_str());
        result = reconnect();
    }
    return result;
}
bool WebCommunicationImpl::loop()
{
    bool result = true;
    //////////////////////////////////
    if (true == this->_mode_ap) {
        // do nothing
    } else {
        if (WiFi.status() != WL_CONNECTED) {
            char buffer[255];
            sprintf(buffer, "ControllerPage : request reconnect");
            this->happened_message(true, buffer);
            WiFi.reconnect();
            result = reconnect();
        } else {
            result = false;
        }
    }
    return result;
}
bool WebCommunicationImpl::end()
{
    bool result = true;
    if (true == this->_mode_ap) {
        // do nothing
    } else {
        if (true == WiFi.isConnected()) {
            WiFi.disconnect();
        }
    }
    return result;
}
////////////////////////////////////////////////

WebCommunicationImpl::WebCommunicationImpl()
        : //
        _ssid(SETTING_WIFI_SSID)
        , _pass(SETTING_WIFI_PASS)
        , _mode_ap(SETTING_WIFI_MODE_AP)
#if STORAGE_SPI_FS
        , open_fs(false)
#endif
{
}
WebCommunicationImpl::~WebCommunicationImpl()
{
}
////////////////////////////////////////////////

} // namespace WEB
} // namespace MaSiRoProject