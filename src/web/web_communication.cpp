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
#include "web/web_communication.hpp"

#include "common/common_function_def.hpp"
#include "web/web_communication_impl.hpp"

#include <WiFi.h>
#include <common/common_function.hpp>
#include <conf/pin_setting.h>
#include <conf/setting.h>

namespace MaSiRoProject
{
namespace ToyBox
{
namespace WEB
{
////////////////////////////////////////////////////////////////////////////////////////////////
#define THREAD_NAME_WIFI     "ThreadWiFi"
#define THREAD_INTERVAL_WIFI (50)
#define THREAD_CORE_WIFI     (CORE_NUM_01)
volatile bool flag_thread_wifi_initialized = false;
volatile bool flag_thread_wifi_fin         = false;
WebCommunicationImpl *ctrl_web;
WebServer *ctrl_server;
ToyBoxMessageFunction callback_mess;

void thread_wifi(void *args)
{
    char buffer[255];
#if DEBUG_MODE
    sprintf(buffer, "<%s> - start", THREAD_NAME_WIFI);
    if (nullptr != callback_mess) {
        callback_mess(false, buffer, true);
    }
#endif
    while (false == flag_thread_wifi_fin) {
        try {
            delay(1000);
            if (false == ctrl_web->begin()) {
                sprintf(buffer, "<%s> - NOT setup()", THREAD_NAME_WIFI);
                if (nullptr != callback_mess) {
                    callback_mess(true, buffer, true);
                }
            } else {
                ctrl_server->begin();
                while (false == flag_thread_wifi_fin) {
                    ctrl_web->loop();
                    if (nullptr != ctrl_server) {
                        ctrl_server->handleClient();
                    }
                    delay(THREAD_INTERVAL_WIFI);
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
bool WebCommunication::set_callback_message(ToyBoxMessageFunction callback)
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
} // namespace ToyBox
} // namespace MaSiRoProject
