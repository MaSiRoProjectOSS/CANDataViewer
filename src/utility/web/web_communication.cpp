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
#include "web_data.h"

namespace MaSiRoProject
{
namespace WEB
{
#pragma region thread_fuction
////////////////////////////////////////////////////////////////////////////////////////////////

#define THREAD_NAME_WIFI                       "ThreadWiFi"
#define THREAD_INTERVAL_WIFI                   (50)
#define THREAD_SEEK_INTERVAL_WIFI              (1000)
#define THREAD_RETRY_INTERVAL_WIFI             (5000)
#define THREAD_RECONNECTION_INTERVAL_WIFI      (1000)
#define WEB_PAGE_HEADER_EXPIRES                "86400"
#define WEB_PAGE_HEADER_CACHE_CONTROL_LONGTIME "max-age=604800, must-revalidate"
#define WEB_PAGE_HEADER_CACHE_CONTROL_NO_CACHE "no-cache, no-store, must-revalidate"

volatile bool flag_thread_wifi_initialized            = false;
volatile bool flag_thread_wifi_fin                    = false;
volatile unsigned long flag_thread_request_connection = 0;
WebCommunicationImpl *ctrl_web;
WebServer *ctrl_server;

std::string request_ssid;
std::string request_pass;
bool request_ap_mode;

/////////////////////////////////////////
// function
/////////////////////////////////////////

inline bool request_connection(void)
{
    bool result = false;
    if (0 != flag_thread_request_connection) {
        if (flag_thread_request_connection <= millis()) {
            result = true;
        }
    }
    return result;
}

/**
 * @brief Registration of connection change information
 *
 * @return true : Accepted
 * @return false : Did not process
 */
inline bool reconnection(void)
{
    bool result = false;
    if (true == request_connection()) {
        flag_thread_request_connection = 0;
        (void)ctrl_web->request_connection_info(request_ssid, request_pass, request_ap_mode);
        request_ssid = "";
        request_pass = "";
        result       = true;
    }

    return result;
}
/**
 * @brief WebServer dedicated loop processing
 *
 * @param args : none
 */
void thread_wifi(void *args)
{
    char buffer[255];
    log_v("<%s> - start", THREAD_NAME_WIFI);
#if SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS
    unsigned long err_begin = millis() + SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS_TIMEOUT;
#endif
    ctrl_web->setup();
    while (false == flag_thread_wifi_fin) {
        try {
            vTaskDelay(THREAD_SEEK_INTERVAL_WIFI);
            (void)reconnection();
            if (false == ctrl_web->begin()) {
                log_e("<%s> - NOT setup()", THREAD_NAME_WIFI);
#if SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS
                if (true != ctrl_web->is_ap_mode()) {
                    if (err_begin < millis()) {
                        err_begin = millis() + SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS_TIMEOUT;
                        ctrl_web->save_information(SETTING_WIFI_SSID_DEFAULT_AP, SETTING_WIFI_PASS_DEFAULT_AP, true, false, false);
                    }
                }
#endif
                vTaskDelay(THREAD_RETRY_INTERVAL_WIFI);
            } else {
                if (nullptr != ctrl_server) {
                    ctrl_server->begin();
                    while (false == flag_thread_wifi_fin) {
                        ctrl_server->handleClient();
                        if (true == request_connection()) {
                            log_v("<%s> - Change connection()", THREAD_NAME_WIFI);
                            break;
                        } else if (true != ctrl_web->is_connected()) {
                            log_v("<%s> - Lost connection()", THREAD_NAME_WIFI);
                            break;
                        }
                        vTaskDelay(THREAD_INTERVAL_WIFI);
                    }
                    ctrl_server->close();
                }
            }
        } catch (...) {
            log_e("<%s> - ERROR()", THREAD_NAME_WIFI);
        }
    }
    flag_thread_wifi_initialized = false;
}
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////
// Page setup
/////////////////////////////////////////
#pragma region page_setup
bool WebCommunication::setup()
{
    bool result = true;
    try {
        this->get_server()->onNotFound(std::bind(&WebCommunication::handle_not_found, this));
        this->get_server()->on("/favicon.ico", std::bind(&WebCommunication::handle_favicon_ico, this));
        this->get_server()->on("/ajax.js", std::bind(&WebCommunication::handle_js_ajax, this));

        this->get_server()->on("/network.css", std::bind(&WebCommunication::handle_network_css, this));
        this->get_server()->on("/network.js", std::bind(&WebCommunication::handle_network_js, this));
        this->get_server()->on("/network", std::bind(&WebCommunication::handle_network_html, this));

        this->get_server()->on("/set/network", std::bind(&WebCommunication::set_network, this));
        this->get_server()->on("/get/net_list", std::bind(&WebCommunication::get_net_list, this));
        this->get_server()->on("/get/network", std::bind(&WebCommunication::get_network, this));

        result = this->setup_server(this->get_server());

    } catch (...) {
    }
    return result;
}
#pragma region page_setup_standard
void WebCommunication::handle_favicon_ico()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_LONGTIME);
    // this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", WEB_PAGE_HEADER_EXPIRES);
    this->get_server()->send_P(200, "image/x-icon", IMAGE_FAVICON_ICO, sizeof(IMAGE_FAVICON_ICO));
}
void WebCommunication::handle_not_found()
{
    std::string html = "404 Not Found!";

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_NO_CACHE);
    this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", "0");
    this->get_server()->sendHeader("Content-Length", String(html.length()));
    this->get_server()->send(404, "text/plain", html.c_str());
}
void WebCommunication::handle_js_ajax()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_LONGTIME);
    // this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", WEB_PAGE_HEADER_EXPIRES);
    this->get_server()->sendHeader("Content-Length", String(WEB_JS_AJAX.length()));
    this->get_server()->send(200, "text/javascript", WEB_JS_AJAX.c_str());
}
#pragma endregion

#pragma region page_setup_network
void WebCommunication::handle_network_css()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_LONGTIME);
    // this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", WEB_PAGE_HEADER_EXPIRES);
    this->get_server()->sendHeader("Content-Length", String(WEB_CSS_NETWORK.length()));
    this->get_server()->send(200, "text/css", WEB_CSS_NETWORK.c_str());
}
void WebCommunication::handle_network_js()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_NO_CACHE);
    this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", WEB_PAGE_HEADER_EXPIRES);
    this->get_server()->sendHeader("Content-Length", String(WEB_JS_NETWORK.length()));
    this->get_server()->send(200, "text/javascript", WEB_JS_NETWORK.c_str());
}
void WebCommunication::handle_network_html()
{
    std::string html = "<!DOCTYPE html><html lang=\"en\"><head>"
                       "<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

    html.append("<link href='/cdv.css' rel='stylesheet' type='text/css' media='all'>");
    html.append("<link href='/network.css' rel='stylesheet' type='text/css' media='all'>");
    html.append("<script type='text/javascript' src='/ajax.js'></script>");
    html.append("<script type='text/javascript' src='/network.js'></script>");
    html.append("<title>Network mode - " SETTING_SYSTEM_NAME "</title>");
    html.append("</head><body>");
    ///////////////////////////
    html.append("<h1>Network Mode</h1>");
    html.append("<article><div class='article_header'><a href='/' target='_self'>TOP</a></div></article>");
    html.append("<article id='article_message' class='div_hide'><p>");
    html.append(this->_message);
    html.append("</p></article>");
    html.append("<article id='article_setting'>");
    html.append("<div id='network_mode_selector'>");
    html.append(
            "<input title='-' placeholder='0' type='radio' id='mode_ap' name='network_mode' value='AP mode' onclick='JS_Network.select_mode(0);'><label for='mode_ap'>&ensp;Access Point mode</label><br />");
    html.append(
            "<input title='-' placeholder='1' type='radio' id='mode_sta' name='network_mode' value='STA mode' onclick='JS_Network.select_mode(1);'><label for='mode_sta'>&ensp;&ensp;&ensp;Station mode&ensp;&ensp;&ensp;</label>");
    html.append("</div>");

    html.append("<div id='network_area'>");
    html.append("<label> LIST : <br /><select id='network_list' name='network_list' class='network_article' onchange='JS_Network.select_list(value);'></select></label>");
    html.append("<button id='network_scan' class='b_disabled' onclick='JS_Network.begin()'>Scan</button><br /><br />");
    html.append("<input title='-' class='input_text' id='network_ssid' name='ssid' length=32 onchange='JS_Network.on_change();' placeholder='SSID' value=''><br />");
    html.append("<input title='-' class='input_text' id='network_pass' name='pass' length=64 onchange='JS_Network.on_change();' placeholder='password' type='password'><br />");
    html.append("<br />");
    html.append("<button onclick='JS_Network.set_network()'>SAVE</button>");
    html.append("</div>");

    html.append("</article>");
    ///////////////////////////
    html.append("</body></html>");

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_LONGTIME);
    // this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", WEB_PAGE_HEADER_EXPIRES);
    this->get_server()->sendHeader("Content-Length", String(html.length()));
    this->get_server()->send(200, "text/html", html.c_str());
}
void WebCommunication::set_network()
{
    bool result  = false;
    String ssid  = "";
    String pass  = "";
    bool mode_ap = false;

    std::string json = "{";
    try {
        if (this->get_server()->args() > 0) {
            if (this->get_server()->hasArg("ap")) {
                int value = this->to_int(this->get_server()->arg("ap"));
                if (value == 1) {
                    mode_ap = true;
                }
                if (this->get_server()->hasArg("id")) {
                    ssid = this->get_server()->arg("id");
                    if (this->get_server()->hasArg("pa")) {
                        pass   = this->get_server()->arg("pa");
                        result = true;
                    }
                }
            }
        }
    } catch (...) {
        result = false;
    }
    if (true == result) {
        json.append("\"result\":\"OK\"");
    } else {
        json.append("\"result\":\"NG\"");
    }
    json.append(",\"status\":{\"num\": 200, \"messages\": \"\"}");
    json.append("}");

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_NO_CACHE);
    this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", "0");
    this->get_server()->sendHeader("Content-Length", String(json.length()));
    this->get_server()->send(200, "application/json", json.c_str());

    if (true == result) {
        this->request_reconnect(ssid.c_str(), pass.c_str(), mode_ap, false);
    }
}
void WebCommunication::get_network()
{
    bool result      = false;
    bool ap_mode     = ctrl_web->is_ap_mode();
    std::string json = "{";
    json.append("\"result\":\"OK\"");
    json.append(",\"status\":{\"num\": 200, \"messages\": \"\"");

    json.append(", \"data\":{");
    json.append("\"default\": \"");
    json.append(ctrl_web->get_ssid_ap_default());
    json.append("\", \"name\": \"");
    json.append(ctrl_web->get_ssid());
    json.append("\", \"ap_mode\":");
    json.append((true == ap_mode) ? "1" : "0");
    json.append("}");

    json.append("}}");

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_NO_CACHE);
    this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", "0");
    this->get_server()->sendHeader("Content-Length", String(json.length()));
    this->get_server()->send(200, "application/json", json.c_str());
}
void WebCommunication::get_net_list()
{
    bool flag_start = true;
    char buffer[255];
    std::vector<NetworkList> items = ctrl_web->get_wifi_list();

    std::string json = "{";
    json.append("\"result\":\"OK\"");
    json.append(",\"status\":{\"num\": 200, \"messages\": \"\", \"data\" : [");

    for (NetworkList item : items) {
        if (false == flag_start) {
            json.append(",");
        }
        flag_start = false;
        sprintf(buffer, "{\"name\":\"%s\" , \"power\":%d}", item.name.c_str(), item.rssi);
        json.append(buffer);
    }

    json.append("]}}");

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_NO_CACHE);
    this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", "0");
    this->get_server()->sendHeader("Content-Length", String(json.length()));
    this->get_server()->send(200, "application/json", json.c_str());
}
#pragma endregion

#pragma endregion

#pragma region fuction
/////////////////////////////////
// value conversion
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

/////////////////////////////////////////
// virtual function
/////////////////////////////////////////
bool WebCommunication::setup_server(WebServer *server)
{
    bool result = true;
    return result;
}

/////////////////////////////////////////
// Set callback
/////////////////////////////////////////

/////////////////////////////////////////
// get  member valuable
/////////////////////////////////////////

WebServer *WebCommunication::get_server()
{
    return ctrl_server;
}
IPAddress WebCommunication::get_ip()
{
    return ctrl_web->get_ip();
}
const char *WebCommunication::get_ssid()
{
    return ctrl_web->get_ssid();
}
bool WebCommunication::is_ap_mode()
{
    return ctrl_web->is_ap_mode();
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
#pragma endregion

/////////////////////////////////////////
// setup function
/////////////////////////////////////////
#pragma region setup_function
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

void WebCommunication::set_message(std::string message)
{
    this->_message = message;
}

std::vector<NetworkList> get_wifi_list()
{
    std::vector<NetworkList> result_data;
    std::vector<NetworkList> list = ctrl_web->get_wifi_list();
    copy(list.begin(), list.end(), back_inserter(result_data));
    return result_data;
}
void WebCommunication::request_reconnect(std::string ssid, std::string pass, bool ap_mode, bool force)
{
    flag_thread_request_connection = millis() + ((true == force) ? -1 : THREAD_RECONNECTION_INTERVAL_WIFI);
    request_ssid                   = ssid;
    request_pass                   = pass;
    request_ap_mode                = ap_mode;
}
void WebCommunication::set_config_address_ap(IPAddress ip, IPAddress subnet, IPAddress gateway)
{
    ctrl_web->set_config_address_ap(ip, subnet, gateway);
}
void WebCommunication::set_config_address_sta(IPAddress ip, IPAddress subnet, IPAddress gateway)
{
    ctrl_web->set_config_address_sta(ip, subnet, gateway);
}
#pragma endregion

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
