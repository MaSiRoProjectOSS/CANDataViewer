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
#define CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS      (100)
#define CONTROLLER_PAGE_STARTUP_STA_TIMEOUT_MS    (50 * CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS)
#define CONTROLLER_PAGE_CONNECTION_CHECK_INTERVAL (5000)

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
    bool skip         = false;
    bool formatOnFail = false;
#if STORAGE_SPI_FS_FORMAT
    formatOnFail = true;
#endif
    if (SPIFFS.begin(formatOnFail)) {
        result = SPIFFS.exists(SETTING_WIFI_INFOMATION);
        SPIFFS.end();
    } else {
        this->happened_message(true, "SPIFFS Failed to Begin. You need to Run SPIFFS.format(),");
        skip = true;
    }
    if (false == skip) {
        this->_open_fs = true;
#if STORAGE_SPI_FS_INITIALIZE
        result = false;
#endif
        if (false == result) {
            result = this->save_information(this->_ssid, this->_pass, this->_mode_ap, false, true);
        } else {
            result = this->load_information();
        }
    }

#endif
    return result;
}
bool WebCommunicationImpl::save_information(std::string ssid, std::string pass, bool ap_mode, bool reconnecting, bool is_save)
{
    bool result      = false;
    bool force_write = false;
    if (true == reconnecting) {
        this->disconnect();
        if (true == this->connect(ssid, pass, ap_mode)) {
            result = this->reconnect();
        }
    } else {
        this->_mode_ap = ap_mode;
        this->_ssid    = ssid;
        this->_pass    = pass;
        result         = true;
    }
    if (true == result) {
        result = false;
        if (true == is_save) {
#if STORAGE_SPI_FS
            if (true == this->_open_fs) {
#if DEBUG_MODE
                this->happened_message(false, "SPIFFS save begin.");
#endif
                if (SPIFFS.begin()) {
                    if (true == SPIFFS.exists(SETTING_WIFI_INFOMATION)) {
                        this->_load_information();
                    } else {
                        force_write = true;
                    }
                    if ((true == force_write) || (this->_ssid != ssid) || (this->_pass != pass) || (this->_mode_ap != ap_mode)) {
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
        } else {
            result = true;
        }
    }
    return result;
}
bool WebCommunicationImpl::_load_information()
{
    bool result        = false;
    int totalBytes     = 0;
    int type           = 0;
    bool previous_type = 0;
    std::string words;
    File dataFile = SPIFFS.open(SETTING_WIFI_INFOMATION, FILE_READ);
    if (!dataFile) {
        result = false;
    } else {
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
    return result;
}
bool WebCommunicationImpl::load_information()
{
    bool result = false;
#if STORAGE_SPI_FS
    if (true == this->_open_fs) {
#if DEBUG_MODE
        this->happened_message(false, "SPIFFS load begin.");
#endif
        static int BUF_SIZE   = 255;
        char buffer[BUF_SIZE] = { 0 };
        if (SPIFFS.begin()) {
            result = this->_load_information();
            SPIFFS.end();
        }
#if DEBUG_MODE
        if (true == result) {
            this->happened_message(false, "Finished load data from SPIFFS");
        } else {
            this->happened_message(false, "Finished SPIFFS");
        }
#endif
    }
#endif
    return result;
}

bool WebCommunicationImpl::is_ap_mode()
{
    return this->_mode_ap;
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

bool WebCommunicationImpl::is_connected(bool force)
{
    static unsigned long next_time = 0;
    static bool result             = false;

    if ((true == force) || (next_time < millis())) {
        next_time          = millis() + CONTROLLER_PAGE_CONNECTION_CHECK_INTERVAL;
        wl_status_t status = WiFi.status();
        switch (status) {
            case wl_status_t::WL_CONNECTED:
            case wl_status_t::WL_SCAN_COMPLETED:
            case wl_status_t::WL_IDLE_STATUS:
                result = true;
                break;
            case wl_status_t::WL_CONNECTION_LOST:
            case wl_status_t::WL_CONNECT_FAILED:
            case wl_status_t::WL_NO_SSID_AVAIL:
            case wl_status_t::WL_NO_SHIELD:
            case wl_status_t::WL_DISCONNECTED:
            default:
                result = false;
                break;
        }
    }
    return result;
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
    if (true != this->is_connected(true)) {
        sprintf(buffer, //
                "WebCommunication : is NOT Connected : SSID [%s] / IP [%s]",
                WiFi.SSID().c_str(),
                this->get_ip().toString().c_str());
        this->happened_message(true, buffer);
        result = false;
    } else {
        sprintf(buffer,
                "WebCommunication : SSID [%s] / IP [%s]", //
                WiFi.SSID().c_str(),
                this->get_ip().toString().c_str());
        this->happened_message(false, buffer);
    }
    return result;
}

bool WebCommunicationImpl::begin()
{
    return this->connect(this->_ssid, this->_pass, this->_mode_ap);
}

bool WebCommunicationImpl::connect(std::string ssid, std::string pass, bool ap_mode)
{
    bool result = false;
    ///////////////////////////
    this->disconnect();
    if (true == ap_mode) {
        result = WiFi.softAP(ssid.c_str(), pass.c_str());
    } else {
        WiFi.begin(ssid.c_str(), pass.c_str());
        result = reconnect();
    }
    if (true == result) {
        this->_mode_ap = ap_mode;
        this->_ssid    = ssid;
        this->_pass    = pass;
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
bool WebCommunicationImpl::disconnect()
{
    bool result = true;
    if (true == this->_mode_ap) {
        WiFi.softAPdisconnect();
    } else {
        if (true == WiFi.isConnected()) {
            WiFi.disconnect();
        }
    }
    return result;
}
////////////////////////////////////////////////

WebCommunicationImpl::WebCommunicationImpl() : _open_fs(false)
{
    if (true == SETTING_WIFI_MODE_AP) {
        this->_mode_ap = true;
        this->_ssid    = SETTING_WIFI_SSID_AP;
        this->_pass    = SETTING_WIFI_PASS_AP;
    } else {
        this->_mode_ap = false;
        this->_ssid    = SETTING_WIFI_SSID_STA;
        this->_pass    = SETTING_WIFI_PASS_STA;
    }
}
WebCommunicationImpl::~WebCommunicationImpl()
{
}
////////////////////////////////////////////////

} // namespace WEB
} // namespace MaSiRoProject