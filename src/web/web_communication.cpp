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
            vTaskDelay(THREAD_SEEK_INTERVAL_WIFI);
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
                        vTaskDelay(THREAD_INTERVAL_WIFI);
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
void WebCommunication::handle_network_css()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    // TODO:
    std::string css = "";
    this->get_server()->send(200, "text/css", css.c_str());
}
void WebCommunication::handle_network_js()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    // TODO:
    std::string js = "if(!JS_Network)var JS_Network={};";

    this->get_server()->send(200, "text/javascript", js.c_str());
}
void WebCommunication::handle_network_html()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    std::string body = "";

    std::string html = "<!DOCTYPE html><html lang=\"jp\"><head>"
                       "<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

    html.append("<link href='/style.css' rel='stylesheet' type='text/css' media='all'>");

    html.append("<script type='text/javascript' src='/network.js'></script>");
    html.append("<script type='text/javascript' src='/network.js'></script>");

    html.append("<title>" SETTING_SYSTEM_NAME "</title>"
                "</head><body>");
    ///////////////////////////
    html.append("<h1>" SETTING_SYSTEM_NAME "</h1>");
    html.append(body);
    ///////////////////////////
    html.append("</body></html>");

    this->get_server()->send(200, "text/html", html.c_str());
}
void WebCommunication::set_network()
{
#if DEBUG_MODE
    this->happened_message(false, "ControllerPage : set_network()");
#endif
    bool result  = false;
    String ssid  = "";
    String pass  = "";
    bool mode_ap = false;

    std::string json = "{";
    if (this->get_server()->args() > 0) {
        if (this->get_server()->hasArg("ap")) {
            int value = this->to_int(this->get_server()->arg("ap"));
            if (value == 1) {
                mode_ap = true;
            }
            if (this->get_server()->hasArg("id")) {
                ssid = this->to_int(this->get_server()->arg("id"));
                if (this->get_server()->hasArg("pa")) {
                    pass   = this->get_server()->arg("pa");
                    result = true;
                }
            }
        }
    }
    if (true == result) {
        json.append("\"result\":\"OK\"");
    } else {
        json.append("\"result\":\"NG\"");
    }
    json.append(",\"status\":{\"num\": 200, \"messages\": \"\"}");
    json.append("}");

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->send(200, "application/json", json.c_str());

    if (true == result) {
        this->request_reconnect(ssid.c_str(), pass.c_str(), mode_ap);
    }
}

bool WebCommunication::setup()
{
    bool result = true;
    try {
        this->get_server()->onNotFound(std::bind(&WebCommunication::handle_not_found, this));
        this->get_server()->on("/set/network", std::bind(&WebCommunication::set_network, this));
        this->get_server()->on("/network.css", std::bind(&WebCommunication::handle_network_css, this));
        this->get_server()->on("/network.js", std::bind(&WebCommunication::handle_network_js, this));
        this->get_server()->on("/network", std::bind(&WebCommunication::handle_network_html, this));

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
        this->task_assigned_size     = (4096 * 2);
        xTaskCreatePinnedToCore(thread_wifi, //
                                THREAD_NAME_WIFI,
                                this->task_assigned_size,
                                NULL,
                                5,
                                &this->task_handle,
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
// protected function
/////////////////////////////////
String WebCommunication::ip_to_string(IPAddress ip)
{
    String res = "";
    for (int i = 0; i < 3; i++) {
        res += String((ip >> (8 * i)) & 0xFF) + ".";
    }
    res += String(((ip >> 8 * 3)) & 0xFF);
    return res;
}
byte WebCommunication::to_byte(String data)
{
    if (true != data.isEmpty()) {
        int value = std::stoi(data.c_str());
        return (byte)(value);
    } else {
        return 0;
    }
}
unsigned long WebCommunication::to_ulong(String data)
{
    if (true != data.isEmpty()) {
        unsigned long value = std::stoul(data.c_str());
        return value;
    } else {
        return 0;
    }
}
int WebCommunication::to_int(String data)
{
    if (true != data.isEmpty()) {
        int value = std::stoi(data.c_str());
        return value;
    } else {
        return 0;
    }
}

#if DEBUG_MODE
UBaseType_t WebCommunication::get_stack_size()
{
    return this->task_assigned_size;
}
UBaseType_t WebCommunication::get_stack_high_water_mark()
{
    return uxTaskGetStackHighWaterMark(this->task_handle);
}
#endif
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
