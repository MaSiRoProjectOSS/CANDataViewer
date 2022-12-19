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
#include <web/web_communication_impl.hpp>

namespace MaSiRoProject
{
namespace ToyBox
{
namespace WEB
{
#define CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS   (100)
#define CONTROLLER_PAGE_STARTUP_STA_TIMEOUT_MS (50 * CONTROLLER_PAGE_STARTUP_STA_SLEEP_MS)

////////////////////////////////////////////////////////////////////////////////////////////////
bool WebCommunicationImpl::set_callback_message(ToyBoxMessageFunction callback)
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
    return result;
}

IPAddress WebCommunicationImpl::get_ip()
{
    if (true == this->mode_ap) {
        return WiFi.softAPIP();
    } else {
        return WiFi.localIP();
    }
}
char *WebCommunicationImpl::get_ssid()
{
    return this->ssid;
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
                this->ssid,
                this->get_ip().toString().c_str());
        this->happened_message(true, buffer);
        result = false;
    } else {
        sprintf(buffer,
                "WebCommunication : SSID [%s] / IP [%s]", //
                this->ssid,
                this->get_ip().toString().c_str());
        this->happened_message(false, buffer);
    }
    return result;
}

bool WebCommunicationImpl::begin()
{
    bool result = false;
    ///////////////////////////
    if (true == this->mode_ap) {
        WiFi.softAP(this->ssid, this->pass);
    } else {
        WiFi.begin(this->ssid, this->pass);
    }
    result = reconnect();

    return result;
}
bool WebCommunicationImpl::loop()
{
    bool result = true;
    //////////////////////////////////
    if (true == this->mode_ap) {
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
    if (true == this->mode_ap) {
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
        ssid(SETTING_WIFI_SSID)
        , pass(SETTING_WIFI_PASS)
{
    mode_ap = SETTING_WIFI_MODE_AP;
}
WebCommunicationImpl::~WebCommunicationImpl()
{
}
////////////////////////////////////////////////

} // namespace WEB
} // namespace ToyBox
} // namespace MaSiRoProject
