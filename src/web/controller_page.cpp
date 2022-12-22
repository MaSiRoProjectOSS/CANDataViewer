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

#include <iostream>
#include <string>

namespace MaSiRoProject
{

namespace WEB
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ControllerPage::setup_server(WebServer *server)
{
    bool result = true;
    if (nullptr != server) {
        server->on("/style.css", std::bind(&ControllerPage::handle_css, this));
        server->on("/can_controller.js", std::bind(&ControllerPage::handle_js_can_controller, this));
        server->on("/ajax.js", std::bind(&ControllerPage::handle_js_ajax, this));
        server->on("/table_view.js", std::bind(&ControllerPage::handle_js_table_view, this));
        server->on("/", std::bind(&ControllerPage::handle_root, this));

        server->on("/get/can_data", std::bind(&ControllerPage::get_can_data, this));
        server->on("/set/can_data", std::bind(&ControllerPage::set_can_data, this));
        server->on("/set/btn_on", std::bind(&ControllerPage::set_mode_on, this));
        server->on("/set/btn_off", std::bind(&ControllerPage::set_mode_off, this));
        server->on("/set/change_mode", std::bind(&ControllerPage::set_change_mode, this));

        server->on("/set/clear", std::bind(&ControllerPage::set_clear, this));
        server->on("/set/default", std::bind(&ControllerPage::set_default, this));
        server->on("/set/delete", std::bind(&ControllerPage::set_delete, this));

        result = true;
#if DEBUG_MODE
        this->happened_message(false, "ControllerPage : setup()");
#endif
    } else {
        this->happened_message(true, "ControllerPage : NOT setup()");
    }
    return result;
}

/////////////////////////////////////////////////

void ControllerPage::handle_css()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    std::string css
            = "td{text-align:center}th{background-color:#d3d3d3}table{border-collapse:collapse;border:outset}td,th{border:outset;text-align:center;padding:5px}input{text-align:center}.td_left{text-align:left}.td_view{background-color:ivory}.td_disable{background-color:#a9a9a9}.td_type{width:6em}";
    this->get_server()->send(200, "text/css", css.c_str());
}
void ControllerPage::handle_js_can_controller()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    std::string js
            = "if(!JS_CCtrl)var JS_CCtrl={can_data:null,timerId:null,timerInterval:1e3,none:function(data){},send_can:function(id,len,loop,data0,data1,data2,data3,data4,data5,data6,data7){let num_loop=1e3*Number(loop);JS_AJAX.get('/set/can_data?id='+id+'&len='+len+(0==num_loop?'':'&loop='+num_loop)+'&d0='+data0+'&d1='+data1+'&d2='+data2+'&d3='+data3+'&d4='+data4+'&d5='+data5+'&d6='+data6+'&d7='+data7).then(ok=>JS_CCtrl.none(ok),error=>console.log(error.status.messages))},send:function(){JS_CCtrl.send_can(document.getElementById('b_send_id').value,document.getElementById('b_send_len').value,document.getElementById('send_time').innerText,document.getElementById('b_send_data_0').value,document.getElementById('b_send_data_1').value,document.getElementById('b_send_data_2').value,document.getElementById('b_send_data_3').value,document.getElementById('b_send_data_4').value,document.getElementById('b_send_data_5').value,document.getElementById('b_send_data_6').value,document.getElementById('b_send_data_7').value)},one_more:function(table_type,id){for(var i=0;i<JS_CCtrl.can_data.resume.length;i++)if(id==JS_CCtrl.can_data.resume[i].ID){JS_CCtrl.send_can(JS_CCtrl.can_data.resume[i].ID,JS_CCtrl.can_data.resume[i].LEN,JS_CCtrl.can_data.resume[i].INTERVAL,JS_CCtrl.can_data.resume[i].DATA[0],JS_CCtrl.can_data.resume[i].DATA[1],JS_CCtrl.can_data.resume[i].DATA[2],JS_CCtrl.can_data.resume[i].DATA[3],JS_CCtrl.can_data.resume[i].DATA[4],JS_CCtrl.can_data.resume[i].DATA[5],JS_CCtrl.can_data.resume[i].DATA[6],JS_CCtrl.can_data.resume[i].DATA[7]);break}},clear:function(table_type){JS_AJAX.get('/set/clear').then(ok=>JS_CCtrl.none(ok),error=>console.log(error.status.messages))},default:function(table_type){JS_AJAX.get('/set/default').then(ok=>JS_CCtrl.none(ok),error=>console.log(error.status.messages))},delete_row:function(table_type,id){JS_AJAX.get('/set/delete?id='+id).then(ok=>JS_CCtrl.none(ok),error=>console.log(error.status.messages))},change_mode:function(){JS_AJAX.get('/set/change_mode').then(ok=>JS_CCtrl.none(ok),error=>JS_CCtrl.none(error))},make:function(data){JS_CCtrl.can_data=data,JS_Table.add_table(data.resume,'send_one_shot','send_one_shot_size',0),JS_Table.add_table(data.send,'send_loop','send_loop_size',1),JS_Table.add_table(data.received,'received_table','received_size',2);let txt_mode=document.getElementById('can_mode');txt_mode.innerHTML!=data.status.mode&&(txt_mode.innerHTML=data.status.mode);let txt_time=document.getElementById('current_time');0!=data.time?txt_time.innerHTML=(Number(data.time)/1e3).toFixed(3):txt_time.innerHTML='--'},interval:function(){JS_AJAX.get('/get/can_data').then(ok=>JS_CCtrl.make(ok),error=>console.log(error.status.messages))}};window.onload=function(){JS_CCtrl.timerId=setInterval(JS_CCtrl.interval,JS_CCtrl.timerInterval)},window.onunload=function(){null!=JS_CCtrl.timerId&&clearInterval(JS_CCtrl.timerId)};";
    this->get_server()->send(200, "text/javascript", js.c_str());
}
void ControllerPage::handle_js_ajax()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    const char js[]
            = "if(!JS_AJAX)var JS_AJAX={post:function(url){return JS_AJAX.send('post',url)},get:function(url){return JS_AJAX.send('get',url)},send:function(method,url){return new Promise((resolve,reject)=>{'post'!=method&&'get'!=method&&(method=get);const request=new XMLHttpRequest;request.ontimeout=()=>{reject({subject:url,result:'NG',status:{num:503,messages:'Timeout'}})},request.onload=function(event){4===request.readyState&&200===request.status?(request.responseText||reject({subject:url,result:'NG',status:{num:500,messages:'Internal Server Error'}}),resolve(JSON.parse(request.responseText))):reject({subject:url,result:'NG',status:{num:request.status,messages:request.statusText}})},request.onerror=function(event){reject({subject:url,result:'NG',status:{num:404,messages:'Not found'}})},request.timeout=3e3,request.open(method,url,!0),request.send(null)})}};";
    this->get_server()->send(200, "text/javascript", js);
}
void ControllerPage::handle_js_table_view()
{
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    std::string js
            = "if(!JS_Table)var JS_Table={del_head:function(elm){let tbody=document.getElementById(elm);for(;2<tbody.rows.length;)tbody.deleteRow(tbody.rows.length-1)},change:function(){0==document.getElementById('b_send_time_s').value&&0==document.getElementById('b_send_time_ms').value?(document.getElementById('send_type').innerHTML='One shot',document.getElementById('send_time').innerHTML='0.000'):(document.getElementById('send_type').innerHTML='Loop',1e3==document.getElementById('b_send_time_ms').value?document.getElementById('send_time').innerHTML=Number(document.getElementById('b_send_time_s').value)+1+'.000':document.getElementById('send_time').innerHTML=document.getElementById('b_send_time_s').value+'.'+document.getElementById('b_send_time_ms').value.toString().padStart(3,'0')),document.getElementById('send_id').innerHTML='0x'+Number(document.getElementById('b_send_id').value).toString(16),document.getElementById('send_len').innerHTML=document.getElementById('b_send_len').value,document.getElementById('send_data_0').innerHTML='0x'+Number(document.getElementById('b_send_data_0').value).toString(16),document.getElementById('send_data_1').innerHTML='0x'+Number(document.getElementById('b_send_data_1').value).toString(16),document.getElementById('send_data_2').innerHTML='0x'+Number(document.getElementById('b_send_data_2').value).toString(16),document.getElementById('send_data_3').innerHTML='0x'+Number(document.getElementById('b_send_data_3').value).toString(16),document.getElementById('send_data_4').innerHTML='0x'+Number(document.getElementById('b_send_data_4').value).toString(16),document.getElementById('send_data_5').innerHTML='0x'+Number(document.getElementById('b_send_data_5').value).toString(16),document.getElementById('send_data_6').innerHTML='0x'+Number(document.getElementById('b_send_data_6').value).toString(16),document.getElementById('send_data_7').innerHTML='0x'+Number(document.getElementById('b_send_data_7').value).toString(16)},copy_row:function(table_type,id){for(var i=0;i<JS_CCtrl.can_data.resume.length;i++)if(id==JS_CCtrl.can_data.resume[i].ID){document.getElementById('b_send_id').value=JS_CCtrl.can_data.resume[i].ID,document.getElementById('b_send_len').value=JS_CCtrl.can_data.resume[i].LEN,0==JS_CCtrl.can_data.resume[i].INTERVAL?(document.getElementById('b_send_time_s').value=0,document.getElementById('b_send_time_ms').value=0):(document.getElementById('b_send_time_ms').value=JS_CCtrl.can_data.resume[i].INTERVAL%1e3,document.getElementById('b_send_time_s').value=(JS_CCtrl.can_data.resume[i].INTERVAL-document.getElementById('b_send_time_ms').value)/1e3),document.getElementById('b_send_data_0').value=JS_CCtrl.can_data.resume[i].DATA[0],document.getElementById('b_send_data_1').value=JS_CCtrl.can_data.resume[i].DATA[1],document.getElementById('b_send_data_2').value=JS_CCtrl.can_data.resume[i].DATA[2],document.getElementById('b_send_data_3').value=JS_CCtrl.can_data.resume[i].DATA[3],document.getElementById('b_send_data_4').value=JS_CCtrl.can_data.resume[i].DATA[4],document.getElementById('b_send_data_5').value=JS_CCtrl.can_data.resume[i].DATA[5],document.getElementById('b_send_data_6').value=JS_CCtrl.can_data.resume[i].DATA[6],document.getElementById('b_send_data_7').value=JS_CCtrl.can_data.resume[i].DATA[7],JS_Table.change();break}},add_table:function(data,elm_ta,elm_size,table_type){let len=data.length,l_size=0,t_size=document.getElementById(elm_size);JS_Table.del_head(elm_ta);let tbody=document.getElementById(elm_ta);for(i=0;i<len;i++){if(0==table_type&&0!=data[i].INTERVAL)continue;l_size++;let tr=document.createElement('tr');if(0==table_type||1==table_type){let td_del=document.createElement('th'),elm_input_del=document.createElement('input');elm_input_del.type='button',elm_input_del.value='delete',elm_input_del.setAttribute('onclick','JS_CCtrl.delete_row('+table_type+','+data[i].ID+');'),td_del.appendChild(elm_input_del),tr.appendChild(td_del);let td_copy=document.createElement('th'),elm_input_copy=document.createElement('input');elm_input_copy.type='button',elm_input_copy.value='copy',elm_input_copy.setAttribute('onclick','JS_Table.copy_row('+table_type+','+data[i].ID+');'),td_copy.appendChild(elm_input_copy),tr.appendChild(td_copy)}let td_time=document.createElement('th');if(td_time.innerHTML=(data[i].TIME/1e3).toFixed(3),tr.appendChild(td_time),1==table_type){let td_loop=document.createElement('td');td_loop.innerHTML=(data[i].INTERVAL/1e3).toFixed(3),tr.appendChild(td_loop)}let td_id=document.createElement('td');td_id.innerHTML='0x'+data[i].ID.toString(16),tr.appendChild(td_id);let td_len=document.createElement('td'),td_size=data[i].LEN;td_len.innerHTML=td_size,tr.appendChild(td_len);for(let j=0;j<td_size;j++){let td_data=document.createElement('td');td_data.innerHTML='0x'+data[i].DATA[j].toString(16),tr.appendChild(td_data)}if(8>td_size){let td_data=document.createElement('td');td_data.colSpan=8-td_size,tr.appendChild(td_data)}if(0==table_type){let td_one_more=document.createElement('th'),elm_input_one_more=document.createElement('input');elm_input_one_more.type='button',elm_input_one_more.value='one more',elm_input_one_more.setAttribute('onclick','JS_CCtrl.one_more('+table_type+','+data[i].ID+');'),td_one_more.appendChild(elm_input_one_more),tr.appendChild(td_one_more)}tbody.appendChild(tr)}t_size.innerHTML=l_size}};";
    this->get_server()->send(200, "text/javascript", js.c_str());
}
void ControllerPage::handle_root()
{
    this->get_server()->send(200, "text/html", this->handle_page(this->page().c_str()));
}

std::string ControllerPage::page()
{
    CanDeviceInfo device_info;
    if (nullptr != callback_device_info) {
        device_info = this->callback_device_info();
    }

    std::string html = "";
    html.append("<p><span>CanType : <b>");
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
                "<span><input type='button' value='On/Off' onclick='JS_CCtrl.change_mode();' />&nbsp;&nbsp;&nbsp;State : <b id='can_mode'>--</b></span>"
                "</p></div><br />");
    ///////////////////////////

    html.append("<details open><summary>send parameter table</summary><table>");
    html.append("<caption><input type='button' value='Send' onclick='JS_CCtrl.send();' /><span>&nbsp;&nbsp;parameter</span></caption>");
    html.append("<tbody>");
    html.append("<tr><th rowspan='2'>type</th><th rowspan='2'>Loop<br>INTERVAL</th><th rowspan='2'>ID</th><th rowspan='2'>Len</th><th colspan='8'>Data</th></tr>");
    html.append("<tr><th>[0]</th><th>[1]</th><th>[2]</th><th>[3]</th><th>[4]</th><th>[5]</th><th>[6]</th><th>[7]</th></tr>");
    html.append(
            "<tr><td class='td_view td_type'><span id='send_type'>Loop</span></td><td id='send_time' class='td_view'>0.500</td><td id='send_id' class='td_view'>0x64</td><td id='send_len' class='td_view'>8</td><td id='send_data_0' class='td_view'>0x00</td><td id='send_data_1' class='td_view'>0x00</td><td id='send_data_2' class='td_view'>0x00</td><td id='send_data_3' class='td_view'>0x00</td><td id='send_data_4' class='td_view'>0x00</td><td id='send_data_5' class='td_view'>0x00</td><td id='send_data_6' class='td_view'>0x00</td><td id='send_data_7' class='td_view'>0x00</td></tr>");
    html.append(
            "<tr><td class='td_left'></td><td><p><input id='b_send_time_s' type='number' value='0' min='0' max='120' onchange='JS_Table.change();' /><span>.</span><input id='b_send_time_ms' type='number' value='500' min='0' max='1000' step='100' onchange='JS_Table.change();' /></p></td><td><input id='b_send_id' type='number' value='100' min='0' max='255' onchange='JS_Table.change();' /></td><td><input id='b_send_len' type='number' value='8' min='1' max='8' onchange='JS_Table.change();' /></td><td><input id='b_send_data_0' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input id='b_send_data_1' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input id='b_send_data_2' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input id='b_send_data_3' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input id='b_send_data_4' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input id='b_send_data_5' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input id='b_send_data_6' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td><td><input id='b_send_data_7' type='number' value='0' min='0' max='255' onchange='JS_Table.change();' /></td></tr>");
    html.append("</tbody></table></details>");

    html.append("<br />");

    html.append("<details open><summary>send (one shot)</summary><table>");
    html.append(
            "<caption><span>send(one shot) : size[<b id='send_one_shot_size'>0</b>]</span><span>&nbsp;&nbsp;</span><input type='button' value='CLEAR' onclick='JS_CCtrl.clear(0);' /></caption>");
    html.append("<tbody id='send_one_shot'>");
    html.append("<tr><th rowspan='2' colspan='2'></th><th rowspan='2'>TIME</th><th rowspan='2'>ID</th><th rowspan='2'>Len</th><th colspan='8'>Data</th><th rowspan='2'></th></tr>");
    html.append("<tr><th>[0]</th><th>[1]</th><th>[2]</th><th>[3]</th><th>[4]</th><th>[5]</th><th>[6]</th><th>[7]</th></tr>");
    html.append("</tbody></table></details>");

    html.append("<br />");

    html.append("<details open><summary>send (loop)</summary><table>");
    html.append(
            "<caption><span>send(loop) : size[<b id='send_loop_size'>0</b>]</span><span>&nbsp;&nbsp;</span><input type='button' value='DEFAULT' onclick='JS_CCtrl.default(1);' /><span>&nbsp;&nbsp;</span><input type='button' value='CLEAR' onclick='JS_CCtrl.clear(1);' /></caption>");
    html.append("<tbody id='send_loop'>");
    html.append(
            "<tr><th rowspan='2' colspan='2'></th><th rowspan='2'>TIME</th><th rowspan='2'>LOOP<br>INTERVAL</th><th rowspan='2'>ID</th><th rowspan='2'>Len</th><th colspan='8'>Data</th></tr>");
    html.append("<tr><th>[0]</th><th>[1]</th><th>[2]</th><th>[3]</th><th>[4]</th><th>[5]</th><th>[6]</th><th>[7]</th></tr>");
    html.append("</tbody></table></details>");

    html.append("<br />");

    html.append("<table>");
    html.append("<caption>received : size[<b id='received_size'>0</b>]</caption>");
    html.append("<tbody id='received_table'>");
    html.append("<tr><th rowspan='2'>Time</th><th rowspan='2'>ID</th><th rowspan='2'>Len</th><th colspan='8'>Data</th></tr>");
    html.append("<tr><th>[0]</th><th>[1]</th><th>[2]</th><th>[3]</th><th>[4]</th><th>[5]</th><th>[6]</th><th>[7]</th></tr>");
    html.append("</tbody></table>");

    return html.c_str();
}
String ControllerPage::handle_page(const char *body)
{
    std::string ip_text = this->get_ip().toString().c_str();

    std::string html = "<!DOCTYPE html><html lang=\"jp\"><head>"
                       "<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";

    html.append("<link href='/style.css' rel='stylesheet' type='text/css' media='all'>");

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
    return html.c_str();
}
/////////////////////////////////////////////////
void ControllerPage::set_clear()
{
#if DEBUG_MODE
    this->happened_message(false, "ControllerPage : set_clear()");
#endif
    bool result      = false;
    std::string json = "{";
    result           = this->callback_data_clear(0);
    if (true == result) {
        json.append("\"result\":\"OK\"");
    } else {
        json.append("\"result\":\"NG\"");
    }
    json.append(",\"status\":{\"num\": 200, \"messages\": \"\"}");
    json.append("}");

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->send(200, "application/json", json.c_str());
}

void ControllerPage::set_default()
{
#if DEBUG_MODE
    this->happened_message(false, "ControllerPage : set_default()");
#endif
    bool result      = false;
    std::string json = "{";
    result           = this->callback_data_default(0);
    if (true == result) {
        json.append("\"result\":\"OK\"");
    } else {
        json.append("\"result\":\"NG\"");
    }
    json.append(",\"status\":{\"num\": 200, \"messages\": \"\"}");
    json.append("}");

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->send(200, "application/json", json.c_str());
}

void ControllerPage::set_delete()
{
#if DEBUG_MODE
    this->happened_message(false, "ControllerPage : set_delete()");
#endif
    bool result      = false;
    std::string json = "{";
    if (this->get_server()->args() > 0) {
        if (this->get_server()->hasArg("id")) {
            int value = this->to_int(this->get_server()->arg("id"));
            if (nullptr != callback_data_delete) {
                result = this->callback_data_delete(value);
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
}

void ControllerPage::set_mode_on()
{
#if DEBUG_MODE
    this->happened_message(false, "ControllerPage : set_mode_on()");
#endif
    if (nullptr != callback_can_mode) {
        this->callback_can_mode(CAN_CTRL_STATE::MODE_RUNNING);
    }
    this->get_server()->send(200, "text/html", this->handle_page(this->page().c_str()));
}

void ControllerPage::set_mode_off()
{
#if DEBUG_MODE
    this->happened_message(false, "ControllerPage : set_mode_off()");
#endif
    if (nullptr != callback_can_mode) {
        this->callback_can_mode(CAN_CTRL_STATE::MODE_STOPPING);
    }
    this->get_server()->send(200, "text/html", this->handle_page(this->page().c_str()));
}

void ControllerPage::get_can_data()
{
#if DEBUG_MODE
    this->happened_message(false, "ControllerPage : get_can_data()");
#endif
    CanDeviceInfo device_info;
    if (nullptr != callback_device_info) {
        device_info = this->callback_device_info();
    }
    std::string json = "{";
    char buffer[255];
    sprintf(buffer, //
            "\"result\":\"OK\", \"time\": %d ,\"status\":{\"num\": 200, \"messages\": \"\", \"mode\": \"%s\"},",
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

    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    this->get_server()->send(200, "application/json", json.c_str());
}
void ControllerPage::set_change_mode()
{
#if DEBUG_MODE
    this->happened_message(false, "ControllerPage : set_change_mode()");
#endif
    CAN_CTRL_STATE mode = CAN_CTRL_STATE::MODE_UNKNOW;
    if (this->get_server()->args() > 0) {
        if (this->get_server()->hasArg("mode")) {
            int value = this->to_int(this->get_server()->arg("mode"));
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
    this->get_server()->sendHeader("Location", String("http://") + this->get_ip().toString(), true);
    char buffer[255];
    std::string json = "{";
    sprintf(buffer, //
            "\"result\":\"OK\",\"status\":{\"num\": 200, \"messages\": \"\", \"mode\": \"%s\" }",
            device_info.mode_text);
    json.append(buffer);
    json.append("}");
    this->get_server()->send(200, "application/json", json.c_str());
}

void ControllerPage::set_can_data()
{
#if DEBUG_MODE
    this->happened_message(false, "ControllerPage : set_can_data()");
#endif
    bool result = false;
    ////////////////////////////////////////////////////////////////////////////////////
    bool flag_loop = false;
    CanData data;
    try {
        WebServer *server = this->get_server();

        if (server->args() > 0) {
            if (server->hasArg("id")) {
                data.Id = this->to_ulong(server->arg("id"));
                if (server->hasArg("loop")) {
                    int value = this->to_int(server->arg("loop"));
                    if (value > 0) {
                        flag_loop          = true;
                        data.loop_interval = value;
                    }
                }
                if (server->hasArg("len")) {
                    data.Length = this->to_byte(server->arg("len"));
                }
                if (server->hasArg("d0")) {
                    data.Data[0] = this->to_byte(server->arg("d0"));
                }
                if (server->hasArg("d1")) {
                    data.Data[1] = this->to_byte(server->arg("d1"));
                }
                if (server->hasArg("d2")) {
                    data.Data[2] = this->to_byte(server->arg("d2"));
                }
                if (server->hasArg("d3")) {
                    data.Data[3] = this->to_byte(server->arg("d3"));
                }
                if (server->hasArg("d4")) {
                    data.Data[4] = this->to_byte(server->arg("d4"));
                }
                if (server->hasArg("d5")) {
                    data.Data[5] = this->to_byte(server->arg("d5"));
                }
                if (server->hasArg("d6")) {
                    data.Data[6] = this->to_byte(server->arg("d6"));
                }
                if (server->hasArg("d7")) {
                    data.Data[7] = this->to_byte(server->arg("d7"));
                }
            }
        }

        if ((0 != data.Id) && (0 < data.Length)) {
            if (true == flag_loop) {
                if (nullptr != this->callback_loop) {
                    result = this->callback_loop(data);
                }
            } else {
                if (nullptr != this->callback_on_shot) {
                    result = this->callback_on_shot(data);
                }
            }
#if DEBUG_MODE
            char buffer[255];
            sprintf(buffer,
                    "set_can_data / interval [%d]"
                    " : id = 0x%02lX[%02d] /  %02d / "
                    "0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                    this->get_server()->args(),
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
            this->happened_message(false, buffer);
#endif
        }
    } catch (...) {
    }
    ////////////////////////////////////////////////////////////////////////////////////
    std::string json = "{";
    if (true == result) {
        json.append("\"result\":\"OK\"");
    } else {
        json.append("\"result\":\"NG\"");
    }
    json.append(",\"status\":{\"num\": 200, \"messages\": \"\"}");
    json.append("}");
    this->get_server()->send(200, "application/json", json.c_str());
    ////////////////////////////////////////////////////////////////////////////////////
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

ControllerPage::ControllerPage() : WEB::WebCommunication()
{
}
ControllerPage::~ControllerPage()
{
}
/////////////////////////////////////////////////
} // namespace WEB
} // namespace MaSiRoProject
