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
#define CONTROLLER_PAGE_STARTUP_STA_TIMEOUT_MS    (5 * 1000)
#define CONTROLLER_PAGE_CONNECTION_CHECK_INTERVAL (5 * 1000)

////////////////////////////////////////////////////////////////////////////////////////////////

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
        result = SPIFFS.exists(SETTING_WIFI_SETTING_FILE);
        SPIFFS.end();
    } else {
        log_e("SPIFFS Failed to Begin. You need to Run SPIFFS.format().");
        skip = true;
    }
    if (false == skip) {
        this->_open_fs = true;
#if STORAGE_SPI_FS_INITIALIZE
        result = false;
#endif
        if (false == result) {
            result = this->_save_information(this->_ssid, this->_pass, this->_mode_ap);
        } else {
            result = this->load_information();
        }
    }

#endif
    return result;
}
bool WebCommunicationImpl::_save_information(std::string ssid, std::string pass, bool ap_mode)
{
    bool result = false;
#if STORAGE_SPI_FS
    bool force_write = false;
    if (true == this->_open_fs) {
        if (SPIFFS.begin()) {
            if (true == SPIFFS.exists(SETTING_WIFI_SETTING_FILE)) {
                this->_load_information_for_spiffs();
            } else {
                force_write = true;
            }
            if ((true == force_write) || (this->_ssid != ssid) || (this->_pass != pass) || (this->_mode_ap != ap_mode)) {
                File dataFile = SPIFFS.open(SETTING_WIFI_SETTING_FILE, FILE_WRITE);
                dataFile.println((true == ap_mode) ? "A" : "S");
                dataFile.println(ssid.c_str());
                dataFile.println(pass.c_str());
                dataFile.close();
                result = true;
            }
            SPIFFS.end();
        }
    }
#endif
    return result;
}
void WebCommunicationImpl::request_connection_info(std::string ssid, std::string pass, bool ap_mode)
{
    this->_mode_ap = ap_mode;
    this->_ssid    = ssid;
    this->_pass    = pass;
}
bool WebCommunicationImpl::_load_information_for_spiffs()
{
    bool result        = false;
    int totalBytes     = 0;
    int type           = 0;
    bool previous_type = 0;
    std::string words;
    result = SPIFFS.exists(SETTING_WIFI_SETTING_FILE);
    if (true == result) {
        File dataFile = SPIFFS.open(SETTING_WIFI_SETTING_FILE, FILE_READ);
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
    }
    return result;
}
bool WebCommunicationImpl::load_information()
{
    bool result = false;
#if STORAGE_SPI_FS
    if (true == this->_open_fs) {
        static int BUF_SIZE   = 255;
        char buffer[BUF_SIZE] = { 0 };
        if (SPIFFS.begin()) {
            result = this->_load_information_for_spiffs();
            SPIFFS.end();
        }
    }
#endif
    return result;
}

bool WebCommunicationImpl::is_ap_mode()
{
    return this->_mode_ap_current;
}

IPAddress WebCommunicationImpl::get_ip()
{
    if (true == this->_mode_ap_current) {
        return WiFi.softAPIP();
    } else {
        return WiFi.localIP();
    }
}
const char *WebCommunicationImpl::get_ssid()
{
    if (true == this->_mode_ap_current) {
        return WiFi.softAPSSID().c_str();
    } else {
        return WiFi.SSID().c_str();
    }
}
const char *WebCommunicationImpl::get_ssid_ap_default()
{
    return this->_ssid_ap.c_str();
}

int WebCommunicationImpl::_get_rssi_as_quality(int rssi)
{
    int quality = 0;

    if (rssi <= -100) {
        quality = 0;
    } else if (rssi >= -50) {
        quality = 100;
    } else {
        quality = 2 * (rssi + 100);
    }
    return quality;
}

std::vector<NetworkList> WebCommunicationImpl::get_wifi_list()
{
    std::vector<NetworkList> list;
    int num = WiFi.scanNetworks();

    if (num == 0) {
    } else {
        for (int i = 0; i < num; i++) {
            NetworkList item;
            String name = WiFi.SSID(i);

            if (0 != name.length()) {
                item.name       = name;
                item.encryption = WiFi.encryptionType(i);
                item.rssi       = WiFi.RSSI(i);
                item.rssi       = this->_get_rssi_as_quality(item.rssi);
                list.push_back(item);
            }
        }
        std::sort(list.begin(), list.end(), NetworkList::compar_rssi);
    }
    return list;
}
void WebCommunicationImpl::set_config_address_ap(IPAddress ip, IPAddress subnet, IPAddress gateway)
{
    if (INADDR_NONE != ip) {
        if (INADDR_NONE != subnet) {
            this->_config_ap.flag_set = true;
            this->_config_ap.local_ip = ip;
            this->_config_ap.gateway  = gateway;
            this->_config_ap.subnet   = subnet;
        }
    }
}
void WebCommunicationImpl::set_config_address_sta(IPAddress ip, IPAddress subnet, IPAddress gateway)
{
    if (INADDR_NONE != ip) {
        if (INADDR_NONE != subnet) {
            this->_config_sta.flag_set = true;
            this->_config_sta.local_ip = ip;
            this->_config_sta.gateway  = gateway;
            this->_config_sta.subnet   = subnet;
        }
    }
}

bool WebCommunicationImpl::is_connected(bool force)
{
    static unsigned long next_time = 0;
    static bool result             = false;
    if (true == this->_mode_ap_current) {
        return true;
    } else {
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
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool WebCommunicationImpl::check_connection()
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
    return result;
}

bool WebCommunicationImpl::begin()
{
    return this->connect(this->_ssid, this->_pass, this->_mode_ap);
}

bool WebCommunicationImpl::connect(std::string ssid, std::string pass, bool ap_mode)
{
    char buffer[255];
    bool result = true;
    ///////////////////////////
    (void)this->disconnect();
    if (true == ap_mode) {
        if (true == this->_config_ap.flag_set) {
            result = WiFi.softAPConfig(this->_config_ap.local_ip, this->_config_ap.gateway, this->_config_ap.subnet);
        }
        if (true == result) {
            result = WiFi.softAP(ssid.c_str(), pass.c_str());
        }
    } else {
        if (true == this->_config_sta.flag_set) {
            result = WiFi.config(this->_config_sta.local_ip, this->_config_sta.gateway, this->_config_sta.subnet);
        }
        if (true == result) {
            WiFi.begin(ssid.c_str(), pass.c_str());
            result = check_connection();
        }
    }
    if (true == result) {
        this->_running = true;
        (void)this->is_connected(true);
        this->_mode_ap_current = this->_mode_ap;
        this->_mode_ap         = ap_mode;
        this->_ssid            = ssid;
        this->_pass            = pass;
        this->_save_information(this->_ssid, this->_pass, this->_mode_ap);
        sprintf(buffer,
                "WebCommunication : %s / SSID [%s] / IP [%s]", //
                ((true == this->_mode_ap_current) ? "AP " : "STA"),
                this->get_ssid(),
                this->get_ip().toString().c_str());
    } else {
        sprintf(buffer, //
                "WebCommunication : is NOT Connected : %s / SSID [%s] / IP [%s]",
                ((true == this->_mode_ap_current) ? "AP " : "STA"),
                this->_ssid,
                this->get_ip().toString().c_str());
    }
    return result;
}
bool WebCommunicationImpl::disconnect()
{
    bool result = true;
    if (true == this->_running) {
        if (true == this->_mode_ap_current) {
            result = WiFi.softAPdisconnect();
        } else {
            if (true == WiFi.isConnected()) {
                result = WiFi.disconnect();
            }
        }

        if (false == result) {
            int countdown = CONTROLLER_PAGE_STARTUP_STA_TIMEOUT_MS;
            while (true == this->is_connected(true)) {
                delay(CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS);
                countdown -= CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS;
                if (0 >= countdown) {
                    result = false;
                    break;
                }
            }
        }
        (void)this->is_connected(true);
    }

    return result;
}
////////////////////////////////////////////////

WebCommunicationImpl::WebCommunicationImpl() : _open_fs(false), _running(false)
{
    this->_ssid_ap = SETTING_WIFI_SSID_DEFAULT_AP;
    if (true == SETTING_WIFI_MODE_AP) {
        this->request_connection_info(SETTING_WIFI_SSID_DEFAULT_AP, SETTING_WIFI_PASS_DEFAULT_AP, true);
    } else {
        this->request_connection_info(SETTING_WIFI_SSID_DEFAULT_STA, SETTING_WIFI_PASS_DEFAULT_STA, false);
    }
}
WebCommunicationImpl::~WebCommunicationImpl()
{
}
////////////////////////////////////////////////

} // namespace WEB
} // namespace MaSiRoProject
