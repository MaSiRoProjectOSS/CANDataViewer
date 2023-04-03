/**
 * @file controller_page.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-26
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "controller_page.hpp"

#include "../can_data_viewer_conf.hpp"
#include "web_data.h"

namespace MaSiRoProject
{

namespace WEB
{
#define WEB_HEADER_CACHE_CONTROL_SHORT_TIME "max-age=100, immutable"
#define WEB_HEADER_CACHE_CONTROL_LONGTIME   "max-age=31536000, immutable"
#define WEB_HEADER_CACHE_CONTROL_NO_CACHE   "no-cache"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ControllerPage::setup_server(AsyncWebServer *server)
{
    bool result = true;
    if (nullptr != server) {
        server->on("/custom.css", std::bind(&ControllerPage::handle_css, this, std::placeholders::_1));
        server->on("/can_controller.js", std::bind(&ControllerPage::handle_js_can_controller, this, std::placeholders::_1));
        server->on("/table_view.js", std::bind(&ControllerPage::handle_js_table_view, this, std::placeholders::_1));
        server->on("/", std::bind(&ControllerPage::handle_root, this, std::placeholders::_1));

        server->on("/get/can_data", std::bind(&ControllerPage::get_can_data, this, std::placeholders::_1));
        server->on("/set/can_data", std::bind(&ControllerPage::set_can_data, this, std::placeholders::_1));
        server->on("/set/btn_on", std::bind(&ControllerPage::set_mode_on, this, std::placeholders::_1));
        server->on("/set/btn_off", std::bind(&ControllerPage::set_mode_off, this, std::placeholders::_1));
        server->on("/set/change_mode", std::bind(&ControllerPage::set_change_mode, this, std::placeholders::_1));

        server->on("/set/clear", std::bind(&ControllerPage::set_clear, this, std::placeholders::_1));
        server->on("/set/default", std::bind(&ControllerPage::set_default, this, std::placeholders::_1));
        server->on("/set/delete", std::bind(&ControllerPage::set_delete, this, std::placeholders::_1));

        result = true;
        log_d("ControllerPage : setup()");

    } else {
        log_e("ControllerPage : NOT setup()");
    }
    return result;
}

/////////////////////////////////////////////////

void ControllerPage::handle_css(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css; charset=utf-8", WEB_CSS_COMMON);
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_LONGTIME);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}
void ControllerPage::handle_js_can_controller(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/javascript; charset=utf-8", WEB_JS_CAN_CONTROLLER);
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_LONGTIME);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}
void ControllerPage::handle_js_table_view(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/javascript; charset=utf-8", WEB_JS_TABLE);
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_LONGTIME);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}
void ControllerPage::handle_root(AsyncWebServerRequest *request)
{
    std::string html = this->page_html(this->page_body().c_str());

    AsyncWebServerResponse *response = request->beginResponse(200, "text/html; charset=utf-8", html.c_str());
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_SHORT_TIME);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}

std::string ControllerPage::page_body()
{
    CanDeviceInfo device_info;
    if (nullptr != callback_device_info) {
        device_info = this->callback_device_info();
    }

    std::string html = "<article><div class='article_header'>";
    html.append("<a href='/network' target='_self'>Network Config</a><br>");
    html.append("<a href='/update' target='_self'>OTA</a></div>");
    html.append("</article>");

    html.append("<article><p><span>CanType : <b>");
    switch (device_info.can_type) {
        case 0:
            html.append("Standard");
            break;
        case 1:
            html.append("Extended");
            break;
        case 2:
            html.append("Standard-Extended");
            break;
        default:
            html.append("Any");
            break;
    }
    html.append("</b></span>");
    html.append("<span>&nbsp; / BPS : <b>");
    if (0 != device_info.can_speed) {
        html.append(device_info.can_speed_txt);
    } else {
        html.append("UNKNOW");
    }
    html.append("</b></span>");
    html.append("<span>&nbsp; / TIME : <b id='current_time'>--</b></span></p>");
    ///////////////////////////
    html.append("<div><p>"
                "<span><input title='-' placeholder='0' type='button' value='On/Off' onclick='JS_CCtrl.change_mode();' />&nbsp;&nbsp;&nbsp;State : <b id='can_mode'>--</b></span>"
                "</p></div></article><br />");
    ///////////////////////////

    html.append("<article><details open><summary>send parameter table</summary><table>");
    html.append("<caption><span>&nbsp;&nbsp;parameter</span></caption>");
    html.append("<tbody>");
    html.append("<tr><th rowspan='2'>type</th><th rowspan='2'>Loop<br>INTERVAL</th><th rowspan='2'>ID</th><th rowspan='2'>Len</th><th colspan='8'>Data</th></tr>");
    html.append("<tr><th>[0]</th><th>[1]</th><th>[2]</th><th>[3]</th><th>[4]</th><th>[5]</th><th>[6]</th><th>[7]</th></tr>");
    html.append(
            "<tr><td class='td_view td_type'><span id='send_type'>Loop</span></td><td id='send_time' class='td_view'>0.500</td><td id='send_id' class='td_view'>0x64</td><td id='send_len' class='td_view'>8</td><td id='send_data_0' class='td_view'>0x00</td><td id='send_data_1' class='td_view'>0x00</td><td id='send_data_2' class='td_view'>0x00</td><td id='send_data_3' class='td_view'>0x00</td><td id='send_data_4' class='td_view'>0x00</td><td id='send_data_5' class='td_view'>0x00</td><td id='send_data_6' class='td_view'>0x00</td><td id='send_data_7' class='td_view'>0x00</td></tr>");
    html.append(
            "<tr><td><input title='-' placeholder='0' type='button' value='Send' onclick='JS_CCtrl.send();' /></td><td><p><input title='-' placeholder='0' id='b_send_time_s' type='number' value='0' min='0' max='120' onchange='JS_Table.change();' /><span>.</span><input title='-' placeholder='0' id='b_send_time_ms' type='number' value='500' min='0' max='1000' step='100' onchange='JS_Table.change();' /></p></td><td><input title='-' placeholder='0' id='b_send_id' type='number' value='100' min='0' max='255' onchange='JS_Table.change();' /></td><td><input title='-' placeholder='0' id='b_send_len' type='number' value='8' min='1' max='8' onchange='JS_Table.change();' /></td><td><input title='-' placeholder='0' id='b_send_data_0' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input title='-' placeholder='0' id='b_send_data_1' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input title='-' placeholder='0' id='b_send_data_2' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input title='-' placeholder='0' id='b_send_data_3' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input title='-' placeholder='0' id='b_send_data_4' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input title='-' placeholder='0' id='b_send_data_5' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input title='-' placeholder='0' id='b_send_data_6' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input title='-' placeholder='0' id='b_send_data_7' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td></tr>");
    html.append("</tbody></table></details></article>");

    html.append("<br />");

    html.append("<article><details open><summary>send (one shot)</summary><table>");
    html.append(
            "<caption><span>send(one shot) : size[<b id='send_one_shot_size'>0</b>]</span><span>&nbsp;&nbsp;</span><input title='-' placeholder='0' type='button' value='CLEAR' onclick='JS_CCtrl.clear(0);' /></caption>");
    html.append("<tbody id='send_one_shot'>");
    html.append("<tr><th rowspan='2' colspan='2'></th><th rowspan='2'>TIME</th><th rowspan='2'>ID</th><th rowspan='2'>Len</th><th colspan='8'>Data</th><th rowspan='2'></th></tr>");
    html.append("<tr><th>[0]</th><th>[1]</th><th>[2]</th><th>[3]</th><th>[4]</th><th>[5]</th><th>[6]</th><th>[7]</th></tr>");
    html.append("</tbody></table></details></article>");

    html.append("<br />");

    html.append("<article><details open><summary>send (loop)</summary><table>");
    html.append(
            "<caption><span>send(loop) : size[<b id='send_loop_size'>0</b>]</span><span>&nbsp;&nbsp;</span><input title='-' placeholder='0' type='button' value='DEFAULT' onclick='JS_CCtrl.default(1);' /><span>&nbsp;&nbsp;</span><input title='-' placeholder='0' type='button' value='CLEAR' onclick='JS_CCtrl.clear(1);' /></caption>");
    html.append("<tbody id='send_loop'>");
    html.append(
            "<tr><th rowspan='2' colspan='2'></th><th rowspan='2'>TIME</th><th rowspan='2'>LOOP<br>INTERVAL</th><th rowspan='2'>ID</th><th rowspan='2'>Len</th><th colspan='8'>Data</th></tr>");
    html.append("<tr><th>[0]</th><th>[1]</th><th>[2]</th><th>[3]</th><th>[4]</th><th>[5]</th><th>[6]</th><th>[7]</th></tr>");
    html.append("</tbody></table></details></article>");

    html.append("<br />");

    html.append("<article><table>");
    html.append("<caption>received : size[<b id='received_size'>0</b>]</caption>");
    html.append("<tbody id='received_table'>");
    html.append("<tr><th rowspan='2'>Time</th><th rowspan='2'>ID</th><th rowspan='2'>Len</th><th colspan='8'>Data</th></tr>");
    html.append("<tr><th>[0]</th><th>[1]</th><th>[2]</th><th>[3]</th><th>[4]</th><th>[5]</th><th>[6]</th><th>[7]</th></tr>");
    html.append("</tbody></table></article>");

    return html.c_str();
}
std::string ControllerPage::page_html(const std::string body)
{
    std::string ip_text = this->get_ip().toString().c_str();

    std::string html = "<!DOCTYPE html><html lang=\"en\"><head>"
                       "<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

    html.append("<link href='/custom.css' rel='stylesheet' type='text/css' media='all'>");

    html.append("<script type='text/javascript' src='/can_controller.js'></script>");
    html.append("<script type='text/javascript' src='/ajax.js'></script>");
    html.append("<script type='text/javascript' src='/table_view.js'></script>");

    html.append("<title>" SETTING_SYSTEM_NAME "</title>"
                "</head><body>");
    ///////////////////////////
    html.append("<h1>" SETTING_SYSTEM_NAME "</h1>");
    html.append(body);
    ///////////////////////////
    html.append("</body></html>");
    return html;
}
/////////////////////////////////////////////////
void ControllerPage::set_clear(AsyncWebServerRequest *request)
{
    log_v("ControllerPage : set_clear()");

    bool result = this->callback_data_clear(0);

    ////////////////////////////////////////////////////////////////////////////////////
    std::string json = this->template_json_result(result);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json; charset=utf-8", json.c_str());
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_NO_CACHE);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}

void ControllerPage::set_default(AsyncWebServerRequest *request)
{
    log_v("ControllerPage : set_default()");

    bool result = false;
    result      = this->callback_data_default(0);
    ////////////////////////////////////////////////////////////////////////////////////
    std::string json = this->template_json_result(result);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json; charset=utf-8", json.c_str());
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_NO_CACHE);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}

void ControllerPage::set_delete(AsyncWebServerRequest *request)
{
    log_v("ControllerPage : set_delete()");

    bool result = false;
    if (request->args() > 0) {
        if (request->hasArg("id")) {
            int value = this->to_int(request->arg("id"));
            if (nullptr != callback_data_delete) {
                result = this->callback_data_delete(value);
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////
    std::string json = this->template_json_result(result);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json; charset=utf-8", json.c_str());
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_NO_CACHE);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}

void ControllerPage::set_mode_on(AsyncWebServerRequest *request)
{
    log_v("ControllerPage : set_mode_on()");
    bool result = false;
    if (nullptr != callback_can_mode) {
        result = this->callback_can_mode(CAN_CTRL_STATE::MODE_RUNNING);
    }

    std::string json = this->template_json_result(result);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json; charset=utf-8", json.c_str());
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_NO_CACHE);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}

void ControllerPage::set_mode_off(AsyncWebServerRequest *request)
{
    log_v("ControllerPage : set_mode_off()");
    bool result = false;
    if (nullptr != callback_can_mode) {
        result = this->callback_can_mode(CAN_CTRL_STATE::MODE_STOPPING);
    }
    std::string json = this->template_json_result(result);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json; charset=utf-8", json.c_str());
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_NO_CACHE);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}

void ControllerPage::get_can_data(AsyncWebServerRequest *request)
{
    log_v("ControllerPage : get_can_data()");

    CanDeviceInfo device_info;
    if (nullptr != callback_device_info) {
        device_info = this->callback_device_info();
    }
    std::string json = "";
    char buffer[255];
    sprintf(buffer, //
            "{\"time\": %lu, \"mode\": \"%s\",",
            millis(),
            device_info.mode_text);
    json.append(buffer);
    json.append("\"resume\": [");
    json.append(get_can_data_text(this->callback_data_request_resume));
    json.append("],");
    json.append("\"received\": [");
    json.append(get_can_data_text(this->callback_data_request_received));
    json.append("],");
    json.append("\"send\": [");
    json.append(get_can_data_text(this->callback_data_request_send));
    json.append("]}");
    ////////////////////////////////////////////////////////////////////////////////////
    std::string send_data = this->template_json_result(true, json.c_str());

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json; charset=utf-8", send_data.c_str());
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_NO_CACHE);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}
void ControllerPage::set_change_mode(AsyncWebServerRequest *request)
{
    log_v("ControllerPage : set_change_mode()");

    CAN_CTRL_STATE mode = CAN_CTRL_STATE::MODE_UNKNOW;
    if (request->args() > 0) {
        if (request->hasArg("mode")) {
            int value = this->to_int(request->arg("mode"));
            if ((0 <= value) && (value <= CAN_CTRL_STATE::MODE_FINISHED)) {
                mode = (CAN_CTRL_STATE)value;
            }
        }
    }
    if (nullptr != callback_can_mode) {
        this->callback_can_mode(mode);
    }
    delay(10);
    CanDeviceInfo device_info;
    if (nullptr != callback_device_info) {
        device_info = this->callback_device_info();
    }
    char buffer[255];
    sprintf(buffer, //
            "{\"mode\": \"%s\"}",
            device_info.mode_text);
    ////////////////////////////////////////////////////////////////////////////////////
    std::string json = this->template_json_result(true, buffer);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json; charset=utf-8", json.c_str());
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_NO_CACHE);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}

void ControllerPage::set_can_data(AsyncWebServerRequest *request)
{
    log_v("ControllerPage : set_can_data()");

    bool result = false;
    ////////////////////////////////////////////////////////////////////////////////////
    bool flag_loop = false;
    CanData data;
    try {
        if (request->args() > 0) {
            if (request->hasArg("id")) {
                data.Id = this->to_ulong(request->arg("id"));
                if (request->hasArg("loop")) {
                    int value = this->to_int(request->arg("loop"));
                    if (value > 0) {
                        flag_loop          = true;
                        data.loop_interval = value;
                    }
                }
                if (request->hasArg("len")) {
                    data.Length = this->to_byte(request->arg("len"));
                }
                if (request->hasArg("d0")) {
                    data.Data[0] = this->to_byte(request->arg("d0"));
                }
                if (request->hasArg("d1")) {
                    data.Data[1] = this->to_byte(request->arg("d1"));
                }
                if (request->hasArg("d2")) {
                    data.Data[2] = this->to_byte(request->arg("d2"));
                }
                if (request->hasArg("d3")) {
                    data.Data[3] = this->to_byte(request->arg("d3"));
                }
                if (request->hasArg("d4")) {
                    data.Data[4] = this->to_byte(request->arg("d4"));
                }
                if (request->hasArg("d5")) {
                    data.Data[5] = this->to_byte(request->arg("d5"));
                }
                if (request->hasArg("d6")) {
                    data.Data[6] = this->to_byte(request->arg("d6"));
                }
                if (request->hasArg("d7")) {
                    data.Data[7] = this->to_byte(request->arg("d7"));
                }
            }
        }

        if (0 < data.Length) {
            this->callback_data_delete(data.Id);
            if (true == flag_loop) {
                if (nullptr != this->callback_loop) {
                    result = this->callback_loop(data);
                }
            } else {
                if (nullptr != this->callback_on_shot) {
                    result = this->callback_on_shot(data);
                }
            }
            log_v("set_can_data / interval [%d]"
                  " : id = 0x%02lX[%02d] /  %02d / "
                  "0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                  request->args(),
                  data.Id,
                  data.loop_interval,
                  data.Length,
                  data.Data[0],
                  data.Data[1],
                  data.Data[2],
                  data.Data[3],
                  data.Data[4],
                  data.Data[5],
                  data.Data[6],
                  data.Data[7]);
        }
    } catch (...) {
    }
    ////////////////////////////////////////////////////////////////////////////////////
    std::string json = this->template_json_result(result);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json; charset=utf-8", json.c_str());
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_NO_CACHE);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}

void ControllerPage::handle_favicon_ico(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", WEB_IMAGE_FAVICON_ICO, WEB_IMAGE_FAVICON_ICO_LEN);
    response->addHeader("Location", String("http://") + this->get_ip().toString());
    response->addHeader("Cache-Control", WEB_HEADER_CACHE_CONTROL_LONGTIME);
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ControllerPage::setup_callback(RequestCanDataFunction data_request_send,
                                    RequestCanDataFunction data_request_received,
                                    RequestCanDataFunction data_request_resume,
                                    GetCanInfoFunction device_info,
                                    SetCanModeFunction can_mode,
                                    SetCanDataFunction on_shot,
                                    SetCanDataFunction loop,
                                    SetAnyFunction data_clear,
                                    SetAnyFunction data_default,
                                    SetAnyFunction data_delete)
{
    bool result = true;
    try {
        if (nullptr != data_request_send) {
            this->callback_data_request_send = data_request_send;
        }
        if (nullptr != data_request_received) {
            this->callback_data_request_received = data_request_received;
        }
        if (nullptr != data_request_resume) {
            this->callback_data_request_resume = data_request_resume;
        }
        if (nullptr != on_shot) {
            this->callback_on_shot = on_shot;
        }
        if (nullptr != loop) {
            this->callback_loop = loop;
        }
        if (nullptr != device_info) {
            this->callback_device_info = device_info;
        }
        if (nullptr != can_mode) {
            this->callback_can_mode = can_mode;
        }
        if (nullptr != data_clear) {
            this->callback_data_clear = data_clear;
        }
        if (nullptr != data_default) {
            this->callback_data_default = data_default;
        }
        if (nullptr != data_delete) {
            this->callback_data_delete = data_delete;
        }
    } catch (...) {
    }
    return result;
}

std::string ControllerPage::get_can_data_text(RequestCanDataFunction callback)
{
    char buffer[255];
    bool flag_first  = true;
    std::string json = "";
    std::vector<CanData> received_list;
    if (nullptr != callback) {
        received_list = callback();
        std::sort(received_list.begin(), received_list.end(), CanData::compar_Id);
    }
    for (CanData item : received_list) {
        if (true != flag_first) {
            json.append(",");
        } else {
            flag_first = false;
        }
        sprintf(buffer,
                "{\"ID\": %ld,\"LEN\": %d,\"DATA\": [%d,%d,%d,%d,%d,%d,%d,%d],\"TIME\": %ld,\"INTERVAL\": %ld}",
                item.Id,
                item.Length,
                item.Data[0],
                item.Data[1],
                item.Data[2],
                item.Data[3],
                item.Data[4],
                item.Data[5],
                item.Data[6],
                item.Data[7],
                item.time,
                item.loop_interval);
        json.append(buffer);
    }
    return json;
}

ControllerPage::ControllerPage() : CushyWebServer()
{
}
ControllerPage::~ControllerPage()
{
}
/////////////////////////////////////////////////
} // namespace WEB
} // namespace MaSiRoProject
