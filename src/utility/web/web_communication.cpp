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

#include "favicon.h"
#include "web_communication_impl.hpp"

namespace MaSiRoProject
{
namespace WEB
{
#pragma region thread_fuction
////////////////////////////////////////////////////////////////////////////////////////////////

#define THREAD_NAME_WIFI                       "ThreadWiFi"
#define THREAD_INTERVAL_WIFI                   (50)
#define THREAD_SEEK_INTERVAL_WIFI              (1000)
#define THREAD_RECONNECTION_INTERVAL_WIFI      (1000)
#define WEB_PAGE_HEADER_EXPIRES                "86400"
#define WEB_PAGE_HEADER_CACHE_CONTROL_LONGTIME "max-age=604800, must-revalidate"
#define WEB_PAGE_HEADER_CACHE_CONTROL_NO_CACHE "no-cache, no-store, must-revalidate"

volatile bool flag_thread_wifi_initialized            = false;
volatile bool flag_thread_wifi_fin                    = false;
volatile unsigned long flag_thread_request_connection = 0;
WebCommunicationImpl *ctrl_web;
WebServer *ctrl_server;
MessageFunction callback_mess;

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
#if DEBUG_MODE
    sprintf(buffer, "<%s> - start", THREAD_NAME_WIFI);
    if (nullptr != callback_mess) {
        callback_mess(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_TRACE, buffer, __func__, __FILENAME__, __LINE__);
    }
#endif
#if SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS
    unsigned long err_begin = millis() + SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS_TIMEOUT;
#endif
    ctrl_web->setup();
    while (false == flag_thread_wifi_fin) {
        try {
            vTaskDelay(THREAD_SEEK_INTERVAL_WIFI);
            (void)reconnection();
            if (false == ctrl_web->begin()) {
                sprintf(buffer, "<%s> - NOT setup()", THREAD_NAME_WIFI);
                if (nullptr != callback_mess) {
                    callback_mess(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_ERROR, buffer, __func__, __FILENAME__, __LINE__);
                }
#if SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS
                if (true != ctrl_web->is_ap_mode()) {
                    if (err_begin < millis()) {
                        err_begin = millis() + SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS_TIMEOUT;
                        ctrl_web->save_information(SETTING_WIFI_SSID_DEFAULT_AP, SETTING_WIFI_PASS_DEFAULT_AP, true, false, false);
                    }
                }
#endif
            } else {
                if (nullptr != ctrl_server) {
                    ctrl_server->begin();
                    while (false == flag_thread_wifi_fin) {
                        ctrl_server->handleClient();
                        if (true == request_connection()) {
#if DEBUG_MODE
                            sprintf(buffer, "<%s> - Change connection()", THREAD_NAME_WIFI);
                            if (nullptr != callback_mess) {
                                callback_mess(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_TRACE, buffer, __func__, __FILENAME__, __LINE__);
                            }
#endif
                            break;
                        } else if (true != ctrl_web->is_connected()) {
#if DEBUG_MODE
                            sprintf(buffer, "<%s> - Lost connection()", THREAD_NAME_WIFI);
                            if (nullptr != callback_mess) {
                                callback_mess(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_TRACE, buffer, __func__, __FILENAME__, __LINE__);
                            }
#endif
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
                callback_mess(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_FATAL, buffer, __func__, __FILENAME__, __LINE__);
            }
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
#if DEBUG_MODE
        this->happened_message(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_TRACE, "WebCommunication : setup()", __func__, __FILENAME__, __LINE__);
#endif
    } catch (...) {
        this->happened_message(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_FATAL, "WebCommunication : NOT setup()", __func__, __FILENAME__, __LINE__);
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
    std::string js
            = "if(!JS_AJAX)var JS_AJAX={post:function(url){return JS_AJAX.send('post',url)},get:function(url){return JS_AJAX.send('get',url)},send:function(method,url){return new Promise((resolve,reject)=>{'post'!=method&&'get'!=method&&(method=get);const request=new XMLHttpRequest;request.ontimeout=()=>{reject({subject:url,result:'NG',status:{num:503,messages:'Timeout'}})},request.onload=function(event){4===request.readyState&&200===request.status?(request.responseText||reject({subject:url,result:'NG',status:{num:500,messages:'Internal Server Error'}}),resolve(JSON.parse(request.responseText))):reject({subject:url,result:'NG',status:{num:request.status,messages:request.statusText}})},request.onerror=function(event){reject({subject:url,result:'NG',status:{num:404,messages:'Not found'}})},request.timeout=1e4,request.open(method,url,!0),request.send(null)})}};";

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_LONGTIME);
    // this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", WEB_PAGE_HEADER_EXPIRES);
    this->get_server()->sendHeader("Content-Length", String(js.length()));
    this->get_server()->send(200, "text/javascript", js.c_str());
}
#pragma endregion

#pragma region page_setup_network
void WebCommunication::handle_network_css()
{
    std::string css
            = "body{text-align:center}div,input{padding:5px;font-size:1em}option{text-align:left}#network_area{text-align:left;display:inline-block;min-width:100px}#network_mode_selector{margin-bottom:1em}#network_list{margin-bottom:1em;width:100%;height:2em;text-align:left}.wifi_article{text-align:center;width:95%;padding:5px;font-size:1em}button{border:0;border-radius:.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%}.b_disabled{background-color:#d3d3d3}.div_hide{display:none}#article_message{border:ridge}.input_text{text-align:center;width:95%}";

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_LONGTIME);
    // this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", WEB_PAGE_HEADER_EXPIRES);
    this->get_server()->sendHeader("Content-Length", String(css.length()));
    this->get_server()->send(200, "text/css", css.c_str());
}
void WebCommunication::handle_network_js()
{
    std::string js
            = "if(!JS_Network)var JS_Network={default_name_ap:'',default_name_sta:'',timeoutID:null,network_list:Array(),timerInterval:3e3,on_change:function(){null!=JS_Network.timeoutID&&(clearTimeout(JS_Network.timeoutID),JS_Network.timeoutID=null)},reception_message:function(data){null!=data&&'OK'==data.result&&(document.getElementById('article_setting').classList.add('div_hide'),document.getElementById('article_message').classList.remove('div_hide'))},set_network:function(){let mode=!0===document.getElementById('mode_ap').checked?1:0;JS_AJAX.post('/set/network?id='+document.getElementById('network_ssid').value+'&pa='+document.getElementById('network_pass').value+'&ap='+mode).then(ok=>JS_Network.reception_message(ok),error=>console.error(error.status.messages))},select_mode:function(sta){JS_Network.on_change();var elem_list=document.getElementById('network_list'),elem_scan=document.getElementById('network_scan'),elem_ssid=document.getElementById('network_ssid');0===sta?(elem_list.disabled=!0,elem_scan.classList.add('b_disabled'),elem_ssid.value=JS_Network.default_name_ap):(elem_list.disabled=!1,elem_scan.classList.remove('b_disabled'),elem_ssid.value=JS_Network.default_name_sta)},select_list:function(id){JS_Network.on_change();for(var set_name='',i=0;i<JS_Network.network_list.length;i++)if(id==JS_Network.network_list[i].index){set_name=JS_Network.network_list[i].name;break}document.getElementById('network_ssid').value=set_name,JS_Network.default_name_sta=set_name},make_option:function(index,name,power){let opt=document.createElement('option');return opt.value=index,opt.innerHTML=(0<power?'  ['+JS_Network.padding(power,3,'&ensp;')+'%]  ':'')+name,opt},set_list:function(data){if(null!=data&&'OK'==data.result){var elm=document.getElementById('network_list');JS_Network.network_list=[],elm.innerHTML='';for(var i=0;i<data.status.data.length;i++){var item=data.status.data[i];let opt;JS_Network.network_list.push({index:i,name:item.name,power:item.power}),document.createElement('option').value=i,elm.appendChild(JS_Network.make_option(i,item.name,item.power))}text='non-public ...';var opt_public=JS_Network.make_option(JS_Network.network_list.length,JS_Network.padding(text,7+text.length,'&ensp;'),-1);opt_public.style.color='darkgrey',elm.appendChild(opt_public)}},padding:function(value,len,pad){for(var buf=''+value,i=buf.length;i<len;i++)buf=pad+buf;return buf},scan:function(){JS_AJAX.get('/get/net_list').then(ok=>JS_Network.set_list(ok),error=>console.error(error.status.messages))},retry_begin:function(data,time_ms){JS_Network.timeoutID=setTimeout(JS_Network.begin,time_ms)},set_info:function(data){var result=!1;if(null!=data&&'OK'==data.result){var mode='mode_sta';1==data.status.data.ap_mode&&(mode='mode_ap'),document.getElementById(mode).checked=!0,JS_Network.default_name_ap=data.status.data.default,JS_Network.default_name_sta=data.status.data.name,document.getElementById('network_ssid').value=data.status.data.name,JS_Network.select_mode(),JS_Network.scan(),result=!0}!1===result&&JS_Network.retry_begin(data,JS_Network.timerInterval)},begin:function(){JS_Network.on_change(),JS_AJAX.get('/get/network').then(ok=>JS_Network.set_info(ok),error=>JS_Network.retry_begin(error,JS_Network.timerInterval))}};window.onload=function(){JS_Network.begin()};";

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->sendHeader("Cache-Control", WEB_PAGE_HEADER_CACHE_CONTROL_NO_CACHE);
    this->get_server()->sendHeader("Pragma", "no-cache");
    this->get_server()->sendHeader("Expires", WEB_PAGE_HEADER_EXPIRES);
    this->get_server()->sendHeader("Content-Length", String(js.length()));
    this->get_server()->send(200, "text/javascript", js.c_str());
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
#if DEBUG_MODE
    this->happened_message(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_TRACE, "ControllerPage : set_network()", __func__, __FILENAME__, __LINE__);
#endif
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

void WebCommunication::happened_message(OUTPUT_LOG_LEVEL level, const char *message, const char *function_name, const char *file_name, int line)
{
    if (nullptr != this->callback_message) {
        this->callback_message(level, message, function_name, file_name, line);
    }
}

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
void WebCommunication::set_config_address_ap(IPAddress ip, IPAddress gateway, IPAddress subnet)
{
    ctrl_web->set_config_address_ap(ip, gateway, subnet);
}
void WebCommunication::set_config_address_sta(IPAddress ip, IPAddress gateway, IPAddress subnet)
{
    ctrl_web->set_config_address_sta(ip, gateway, subnet);
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
