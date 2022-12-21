/**
 * @file web_communication.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "web_communication.hpp"

#include "web_communication_impl.hpp"

namespace MaSiRoProject
{
namespace WEB
{
////////////////////////////////////////////////////////////////////////////////////////////////
#define THREAD_NAME_WIFI                  "ThreadWiFi"
#define THREAD_INTERVAL_WIFI              (50)
#define THREAD_SEEK_INTERVAL_WIFI         (1000)
#define THREAD_RECONNECTION_INTERVAL_WIFI (1000)
volatile bool flag_thread_wifi_initialized            = false;
volatile bool flag_thread_wifi_fin                    = false;
volatile unsigned long flag_thread_request_connection = 0;
WebCommunicationImpl *ctrl_web;
WebServer *ctrl_server;
MessageFunction callback_mess;
std::string request_ssid;
std::string request_pass;
bool request_ap_mode;
inline bool reconnection(void)
{
    bool result = true;
    if (0 != flag_thread_request_connection) {
        if (flag_thread_request_connection <= millis()) {
            flag_thread_request_connection = 0;
            result                         = ctrl_web->save_information(request_ssid, request_pass, request_ap_mode, true, true);
            request_ssid                   = "";
            request_pass                   = "";
        }
    }
    return result;
}
void thread_wifi(void *args)
{
    char buffer[255];
#if DEBUG_MODE
    sprintf(buffer, "<%s> - start", THREAD_NAME_WIFI);
    if (nullptr != callback_mess) {
        callback_mess(false, buffer, true);
    }
#endif
#if SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS
    unsigned long err_begin = millis() + SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS_TIMEOUT;
#endif
    ctrl_web->setup();
    while (false == flag_thread_wifi_fin) {
        try {
            delay(THREAD_SEEK_INTERVAL_WIFI);
            reconnection();
            if (false == ctrl_web->begin()) {
                sprintf(buffer, "<%s> - NOT setup()", THREAD_NAME_WIFI);
                if (nullptr != callback_mess) {
                    callback_mess(true, buffer, true);
                }
#if SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS
                if (true != ctrl_web->is_ap_mode()) {
                    if (err_begin < millis()) {
                        err_begin = millis() + SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS_TIMEOUT;
                        ctrl_web->save_information(SETTING_WIFI_SSID_AP, SETTING_WIFI_PASS_AP, true, false, false);
                    }
                }
#endif
            } else {
                if (nullptr != ctrl_server) {
                    ctrl_server->begin();
                    while (false == flag_thread_wifi_fin) {
                        ctrl_web->loop();
                        ctrl_server->handleClient();
                        if (false == reconnection()) {
                            break;
                        } else if (true != ctrl_web->is_connected()) {
                            break;
                        }
                        delay(THREAD_INTERVAL_WIFI);
                    }
                    ctrl_server->close();
                }
            }
        } catch (...) {
            sprintf(buffer, "<%s> - ERROR()", THREAD_NAME_WIFI);
            if (nullptr != callback_mess) {
                callback_mess(true, buffer, true);
            }
        }
    }
    flag_thread_wifi_initialized = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool WebCommunication::set_callback_message(MessageFunction callback)
{
    bool result = false;
    try {
        callback_mess          = callback;
        this->callback_message = callback;
        ctrl_web->set_callback_message(callback);
        result = true;
    } catch (...) {
    }
    return result;
}

void WebCommunication::happened_message(bool is_error, const char *message)
{
    if (nullptr != this->callback_message) {
        this->callback_message(is_error, message, true);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
WebServer *WebCommunication::get_server()
{
    return ctrl_server;
}

bool WebCommunication::setup_server(WebServer *server)
{
    bool result = true;
    return result;
}

void WebCommunication::handle_not_found()
{
    this->get_server()->send(404, "text/plain", "404 Not Found!");
}

bool WebCommunication::setup()
{
    bool result = true;
    try {
        this->get_server()->onNotFound(std::bind(&WebCommunication::handle_not_found, this));
        result = this->setup_server(this->get_server());
#if DEBUG_MODE
        this->happened_message(false, "WebCommunication : setup()");
#endif
    } catch (...) {
        this->happened_message(true, "WebCommunication : NOT setup()");
    }
    return result;
}
bool WebCommunication::begin()
{
    bool result = true;
    if (false == flag_thread_wifi_initialized) {
        flag_thread_wifi_initialized = true;
        xTaskCreatePinnedToCore(thread_wifi, //
                                THREAD_NAME_WIFI,
                                sizeof(WebCommunicationImpl) + 4096,
                                NULL,
                                5,
                                NULL,
                                THREAD_CORE_WIFI);
    }
    return result;
}

IPAddress WebCommunication::get_ip()
{
    return ctrl_web->get_ip();
}
bool WebCommunication::set_wifi_info(std::string ssid, std::string pass, bool ap_mode)
{
    return ctrl_web->save_information(ssid, pass, ap_mode, true, true);
}
void WebCommunication::request_reconnect(std::string ssid, std::string pass, bool ap_mode)
{
    flag_thread_request_connection = millis() + THREAD_RECONNECTION_INTERVAL_WIFI;
    request_ssid                   = ssid;
    request_pass                   = pass;
    request_ap_mode                = ap_mode;
}
/////////////////////////////////
// Constructor
/////////////////////////////////
#pragma region Constructor
WebCommunication::WebCommunication() : initialized(false)
{
    ctrl_web    = new WebCommunicationImpl();
    ctrl_server = new WebServer(SETTING_WIFI_PORT);
}

WebCommunication::~WebCommunication()
{
    if (nullptr != ctrl_server) {
        ctrl_server->close();
    }
    flag_thread_wifi_fin = false;
}
#pragma endregion

} // namespace WEB
} // namespace MaSiRoProject
