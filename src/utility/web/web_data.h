/**
 * @file web_data.h
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2
 * @date 2022-12-27
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_WEB_DATA_HPP
#define MASIRO_PROJECT_TOY_BOX_WEB_DATA_HPP
#include <Arduino.h>

namespace MaSiRoProject
{
namespace WEB
{
const std::string WEB_CSS_NETWORK PROGMEM
        = "body{text-align:center}div,input{padding:5px;font-size:1em}option{text-align:left}#network_area{text-align:left;display:inline-block;min-width:100px}#network_mode_selector{margin-bottom:1em}#network_list{margin-bottom:1em;width:100%;height:2em;text-align:left}.wifi_article{text-align:center;width:95%;padding:5px;font-size:1em}button{border:0;border-radius:.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%}.b_disabled{background-color:#d3d3d3}.div_hide{display:none}#article_message{border:ridge}.input_text{text-align:center;width:95%}";
const std::string WEB_JS_NETWORK PROGMEM
        = "if(!JS_Network)var JS_Network={default_name_ap:'',default_name_sta:'',timeoutID:null,network_list:Array(),timerInterval:3e3,on_change:function(){null!=JS_Network.timeoutID&&(clearTimeout(JS_Network.timeoutID),JS_Network.timeoutID=null)},reception_message:function(data){null!=data&&'OK'==data.result&&(document.getElementById('article_setting').classList.add('div_hide'),document.getElementById('article_message').classList.remove('div_hide'))},set_network:function(){let mode=!0===document.getElementById('mode_ap').checked?1:0;JS_AJAX.post('/set/network?id='+document.getElementById('network_ssid').value+'&pa='+document.getElementById('network_pass').value+'&ap='+mode).then(ok=>JS_Network.reception_message(ok),error=>console.error(error.status.messages))},select_mode:function(sta){JS_Network.on_change();var elem_list=document.getElementById('network_list'),elem_scan=document.getElementById('network_scan'),elem_ssid=document.getElementById('network_ssid');0===sta?(elem_list.disabled=!0,elem_scan.classList.add('b_disabled'),elem_ssid.value=JS_Network.default_name_ap):(elem_list.disabled=!1,elem_scan.classList.remove('b_disabled'),elem_ssid.value=JS_Network.default_name_sta)},select_list:function(id){JS_Network.on_change();for(var set_name='',i=0;i<JS_Network.network_list.length;i++)if(id==JS_Network.network_list[i].index){set_name=JS_Network.network_list[i].name;break}document.getElementById('network_ssid').value=set_name,JS_Network.default_name_sta=set_name},make_option:function(index,name,power){let opt=document.createElement('option');return opt.value=index,opt.innerHTML=(0<power?'  ['+JS_Network.padding(power,3,'&ensp;')+'%]  ':'')+name,opt},set_list:function(data){if(null!=data&&'OK'==data.result){var elm=document.getElementById('network_list');JS_Network.network_list=[],elm.innerHTML='';for(var i=0;i<data.status.data.length;i++){var item=data.status.data[i];let opt;JS_Network.network_list.push({index:i,name:item.name,power:item.power}),document.createElement('option').value=i,elm.appendChild(JS_Network.make_option(i,item.name,item.power))}text='non-public ...';var opt_public=JS_Network.make_option(JS_Network.network_list.length,JS_Network.padding(text,7+text.length,'&ensp;'),-1);opt_public.style.color='darkgrey',elm.appendChild(opt_public)}},padding:function(value,len,pad){for(var buf=''+value,i=buf.length;i<len;i++)buf=pad+buf;return buf},scan:function(){JS_AJAX.get('/get/net_list').then(ok=>JS_Network.set_list(ok),error=>console.error(error.status.messages))},retry_begin:function(data,time_ms){JS_Network.timeoutID=setTimeout(JS_Network.begin,time_ms)},set_info:function(data){var result=!1;if(null!=data&&'OK'==data.result){var mode='mode_sta';1==data.status.data.ap_mode&&(mode='mode_ap'),document.getElementById(mode).checked=!0,JS_Network.default_name_ap=data.status.data.default,JS_Network.default_name_sta=data.status.data.name,document.getElementById('network_ssid').value=data.status.data.name,JS_Network.select_mode(),JS_Network.scan(),result=!0}!1===result&&JS_Network.retry_begin(data,JS_Network.timerInterval)},begin:function(){JS_Network.on_change(),JS_AJAX.get('/get/network').then(ok=>JS_Network.set_info(ok),error=>JS_Network.retry_begin(error,JS_Network.timerInterval))}};window.onload=function(){JS_Network.begin()};";
const std::string WEB_JS_AJAX PROGMEM
        = "if(!JS_AJAX)var JS_AJAX={post:function(url){return JS_AJAX.send('post',url)},get:function(url){return JS_AJAX.send('get',url)},send:function(method,url){return new Promise((resolve,reject)=>{'post'!=method&&'get'!=method&&(method=get);const request=new XMLHttpRequest;request.ontimeout=()=>{reject({subject:url,result:'NG',status:{num:503,messages:'Timeout'}})},request.onload=function(event){4===request.readyState&&200===request.status?(request.responseText||reject({subject:url,result:'NG',status:{num:500,messages:'Internal Server Error'}}),resolve(JSON.parse(request.responseText))):reject({subject:url,result:'NG',status:{num:request.status,messages:request.statusText}})},request.onerror=function(event){reject({subject:url,result:'NG',status:{num:404,messages:'Not found'}})},request.timeout=1e4,request.open(method,url,!0),request.send(null)})}};";

const char IMAGE_FAVICON_ICO[] PROGMEM = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x08, 0x06, 0x00, 0x00, 0x00,
    0xAA, 0x69, 0x71, 0xDE, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x0B, 0x13, 0x00, 0x00, 0x0B, 0x13, 0x01, 0x00, 0x9A, 0x9C, 0x18, 0x00, 0x00, 0x0C, 0xD0,
    0x49, 0x44, 0x41, 0x54, 0x78, 0x9C, 0xED, 0x5B, 0x7B, 0x94, 0x55, 0xD5, 0x7D, 0xFE, 0x7E, 0xFB, 0x9C, 0xFB, 0x82, 0x61, 0x78, 0x83, 0x82, 0x51, 0xF1, 0x89, 0x4D, 0x4C, 0x57,
    0x5D, 0xD1, 0xA6, 0x6A, 0x55, 0x58, 0x3C, 0x06, 0x04, 0x44, 0x5B, 0x10, 0x54, 0x8C, 0x44, 0x60, 0x70, 0x42, 0xDB, 0x58, 0xDB, 0x24, 0x9A, 0xA6, 0x6A, 0x62, 0x56, 0xD3, 0xA6,
    0x2E, 0x35, 0xA4, 0xC0, 0x30, 0x33, 0x18, 0x0C, 0x0F, 0x65, 0x56, 0x8D, 0x8A, 0xC8, 0x08, 0x0C, 0x13, 0x5A, 0x4D, 0x63, 0xA3, 0x42, 0x1A, 0x92, 0xA8, 0xC4, 0x57, 0x78, 0x88,
    0x60, 0x19, 0x61, 0x86, 0xB9, 0x8F, 0x73, 0xF6, 0xEF, 0xEB, 0x1F, 0xE7, 0xCE, 0x65, 0x1E, 0x97, 0x99, 0x73, 0xEF, 0x3C, 0xD2, 0x95, 0xF8, 0xAD, 0x75, 0xD7, 0xCC, 0xBD, 0x7B,
    0xEF, 0xDF, 0xEF, 0xFB, 0x7D, 0x7B, 0x9F, 0xFD, 0x3E, 0xC0, 0x27, 0xF8, 0xC3, 0x86, 0xF4, 0x96, 0xA1, 0x13, 0x95, 0x33, 0x46, 0xC4, 0x35, 0x73, 0x9E, 0xAA, 0x9C, 0x65, 0xA2,
    0x91, 0x91, 0x1A, 0x19, 0x38, 0x9C, 0x5E, 0xCB, 0x09, 0x63, 0xD3, 0x47, 0xAD, 0xE8, 0xE1, 0x84, 0x33, 0x60, 0x9F, 0x2C, 0x7A, 0xEE, 0x50, 0x6F, 0xF9, 0xEB, 0x2D, 0x14, 0x2D,
    0xC0, 0xD1, 0x7F, 0xBE, 0x72, 0x50, 0xC9, 0x90, 0x21, 0x0B, 0xA8, 0x76, 0x1A, 0x80, 0x6B, 0xC4, 0x66, 0x4A, 0x49, 0xC9, 0x5A, 0x6C, 0x6B, 0x96, 0x81, 0x23, 0x12, 0x70, 0xA3,
    0x69, 0x00, 0xAF, 0x88, 0x71, 0xB6, 0x5A, 0xD1, 0x75, 0x03, 0x97, 0xD4, 0x1D, 0xE8, 0x09, 0xF9, 0xDE, 0x40, 0xC1, 0x02, 0xA4, 0xD6, 0x5C, 0x7F, 0x21, 0x3C, 0xFB, 0x80, 0x5A,
    0x9D, 0x07, 0xB5, 0x06, 0x22, 0xAD, 0x66, 0x18, 0xDA, 0x1F, 0x35, 0x28, 0x66, 0xDC, 0xCD, 0x26, 0xE2, 0xDE, 0x17, 0x5B, 0xB4, 0x65, 0x6F, 0xA1, 0x3C, 0x7A, 0x0B, 0xA1, 0x05,
    0xE0, 0xA6, 0x39, 0xD1, 0xF4, 0xB1, 0xA6, 0x87, 0x69, 0xFD, 0x65, 0x24, 0x10, 0x44, 0x10, 0x2A, 0xE8, 0x2E, 0x7C, 0x13, 0x42, 0xC2, 0xB8, 0x91, 0xC7, 0x62, 0x4B, 0xEB, 0xBE,
    0xDC, 0x03, 0x5B, 0x3D, 0x21, 0xD1, 0x3D, 0x92, 0xAB, 0x27, 0x8F, 0xA3, 0x2F, 0x3B, 0xA1, 0xF6, 0x5C, 0x88, 0x01, 0x7A, 0x16, 0x78, 0x67, 0x0E, 0xB4, 0x10, 0xC7, 0x7D, 0x3D,
    0x6E, 0x06, 0x4F, 0x94, 0xF2, 0xDA, 0xE3, 0xBD, 0x68, 0xBB, 0x5B, 0x98, 0xEE, 0x32, 0xB4, 0xD4, 0xCC, 0xBE, 0x9A, 0xBE, 0xFC, 0x12, 0xE4, 0xB9, 0x10, 0x43, 0xF4, 0x6E, 0xF0,
    0x00, 0x40, 0x88, 0x43, 0xAA, 0x5E, 0x96, 0xD6, 0xA6, 0xDD, 0xDC, 0x34, 0x27, 0xD1, 0xCB, 0xF6, 0xBB, 0x44, 0x97, 0x02, 0xA4, 0x2A, 0xA7, 0x8C, 0x47, 0xBA, 0xA5, 0x1E, 0x64,
    0x02, 0xBD, 0x1F, 0x78, 0x07, 0x08, 0xD5, 0xFA, 0xE3, 0xD2, 0xFF, 0x7B, 0x62, 0x6B, 0xDF, 0xFA, 0x69, 0x8F, 0xD3, 0x0A, 0xC0, 0xEA, 0x59, 0x83, 0x54, 0x65, 0x17, 0xC8, 0x28,
    0xFA, 0x3C, 0x78, 0x00, 0x80, 0x40, 0x84, 0xEA, 0x7B, 0xD7, 0xA6, 0x2A, 0xA7, 0x7E, 0xA3, 0x1F, 0xFC, 0x01, 0xE8, 0x42, 0x80, 0x94, 0x97, 0x7E, 0x02, 0xD6, 0x1F, 0x85, 0xFE,
    0x09, 0x1E, 0x39, 0x3F, 0xC6, 0x01, 0x7D, 0xFB, 0xCD, 0xD4, 0xCA, 0xEB, 0x2F, 0xEC, 0x0F, 0xA7, 0x79, 0x05, 0x48, 0xAE, 0x9C, 0x32, 0x99, 0x7E, 0x66, 0x76, 0xB6, 0xC3, 0xEB,
    0x2F, 0xB4, 0x76, 0xC8, 0x0C, 0x94, 0xB0, 0xDF, 0xEB, 0x0F, 0xA7, 0xF9, 0x23, 0x24, 0x1F, 0x81, 0x71, 0x81, 0xE2, 0x6A, 0x5F, 0x40, 0x0A, 0xA8, 0xC1, 0x07, 0xCC, 0x4D, 0x14,
    0xBA, 0x41, 0x1B, 0x5F, 0x42, 0xFA, 0xE9, 0xB2, 0x74, 0xE5, 0xB4, 0x4B, 0x8B, 0xF0, 0x5F, 0x10, 0xDC, 0x8E, 0x3F, 0xA4, 0x56, 0x95, 0xCD, 0x54, 0x2F, 0xFD, 0x69, 0x18, 0xB7,
    0xD0, 0xE0, 0x45, 0x68, 0x21, 0x4E, 0x74, 0x0F, 0x61, 0x76, 0x8A, 0xC8, 0x7E, 0x35, 0x52, 0x22, 0xEA, 0x5F, 0x48, 0xD5, 0x1B, 0xA1, 0x76, 0x50, 0x76, 0x14, 0x09, 0x05, 0x1A,
    0x07, 0x6A, 0xED, 0x57, 0x00, 0x2C, 0x28, 0x90, 0x47, 0x41, 0xE8, 0x54, 0x33, 0xA9, 0x95, 0x65, 0xBB, 0x54, 0xFD, 0x6B, 0x00, 0x09, 0x2F, 0x00, 0x55, 0xC4, 0x71, 0xDF, 0x72,
    0xDC, 0xC4, 0x82, 0xE8, 0xE2, 0x67, 0xFE, 0xBB, 0x53, 0xF2, 0xFD, 0xF7, 0x9B, 0xCC, 0x98, 0x57, 0x1E, 0x50, 0xCF, 0xFB, 0x06, 0xC3, 0xCF, 0x23, 0x04, 0x02, 0x3D, 0xD9, 0x74,
    0x72, 0xC8, 0xC8, 0xAF, 0xFE, 0xA4, 0x29, 0x34, 0x97, 0x02, 0xD1, 0x4E, 0x00, 0xD6, 0x94, 0x8D, 0x4C, 0x25, 0xBD, 0x23, 0x2C, 0xE4, 0xD9, 0x27, 0x21, 0x8E, 0xF3, 0xD2, 0xE6,
    0x9D, 0xA5, 0xD7, 0xCD, 0xAD, 0xAD, 0xB5, 0x5D, 0x65, 0x4D, 0xAD, 0x9A, 0x31, 0x93, 0x7E, 0xF2, 0xB9, 0xD0, 0xF6, 0x69, 0x21, 0xC6, 0x2D, 0x4F, 0x54, 0x6C, 0x5B, 0x1D, 0x9E,
    0x50, 0x61, 0x68, 0xC7, 0x24, 0x95, 0xC1, 0x9C, 0x6C, 0xD5, 0x30, 0xCC, 0x47, 0x00, 0x88, 0x31, 0x87, 0x4F, 0xD8, 0xD2, 0x69, 0xDD, 0x05, 0x0F, 0x00, 0xF1, 0xA5, 0xCF, 0x6F,
    0x96, 0x58, 0xFC, 0x1F, 0x41, 0x1B, 0xC6, 0x47, 0x2B, 0xFA, 0xF4, 0x11, 0x68, 0x5F, 0x15, 0xB4, 0x93, 0xB3, 0x73, 0xFC, 0x50, 0xA0, 0xFA, 0x10, 0x63, 0x96, 0x8C, 0x5E, 0x56,
    0xDB, 0x1C, 0xB6, 0x4C, 0x7C, 0xF1, 0x96, 0x6F, 0x19, 0xE3, 0xFC, 0x06, 0xED, 0x5B, 0x9F, 0x04, 0xEB, 0x48, 0x0A, 0x68, 0x45, 0xD4, 0x17, 0xE3, 0xB8, 0xFB, 0x8D, 0x89, 0x56,
    0x8A, 0x13, 0x7D, 0x34, 0x34, 0xA1, 0x22, 0xD0, 0xB1, 0x13, 0xBC, 0x4E, 0x20, 0x61, 0xBB, 0x7E, 0x11, 0x27, 0xFA, 0x46, 0x7C, 0x69, 0xDD, 0xE6, 0x42, 0x9D, 0x52, 0xE4, 0x31,
    0xA8, 0xBF, 0x1C, 0x62, 0x04, 0x24, 0x84, 0x0A, 0xB8, 0xF1, 0x63, 0x22, 0xFA, 0x3C, 0x4C, 0xB4, 0x21, 0x1E, 0x41, 0x9D, 0x2C, 0x7C, 0xE1, 0x70, 0xA1, 0x76, 0x8B, 0x41, 0x4E,
    0x00, 0x2E, 0x9F, 0x34, 0x26, 0x65, 0xFD, 0x21, 0x0C, 0xDB, 0xFB, 0x53, 0x41, 0x27, 0xB2, 0xBE, 0x18, 0xA7, 0x3E, 0xB9, 0xC9, 0x75, 0x22, 0xFF, 0x6A, 0x20, 0x5B, 0x54, 0x64,
    0x27, 0x22, 0x66, 0x6B, 0xE2, 0xCE, 0x2D, 0xEF, 0x14, 0x63, 0xAB, 0xA7, 0xC8, 0x09, 0x90, 0x72, 0x70, 0x29, 0xB5, 0x80, 0xED, 0x01, 0x12, 0xAE, 0xEB, 0xD6, 0x15, 0xE3, 0x74,
    0x50, 0xC5, 0xF6, 0x23, 0x00, 0xE2, 0xC5, 0x94, 0xED, 0x6D, 0xE4, 0xFA, 0x00, 0x81, 0x73, 0x71, 0x01, 0xDB, 0x03, 0x22, 0x8E, 0x8B, 0x67, 0x4A, 0x63, 0xBB, 0xFB, 0x86, 0x56,
    0xFF, 0x21, 0x27, 0x80, 0x05, 0xCF, 0x09, 0x5B, 0x28, 0x90, 0x49, 0x0E, 0xCC, 0x9D, 0xDB, 0x7D, 0xCF, 0xFF, 0xFF, 0x1D, 0x39, 0x01, 0x8C, 0x60, 0x64, 0xD8, 0x11, 0x80, 0x00,
    0x20, 0x38, 0xD8, 0x47, 0x9C, 0xFA, 0x15, 0x39, 0x01, 0x08, 0x0C, 0x0E, 0x5F, 0x8C, 0x20, 0xA4, 0xB1, 0x2F, 0x08, 0xF5, 0x37, 0x4E, 0xCD, 0x03, 0xC8, 0x41, 0x85, 0x14, 0x14,
    0xE0, 0x64, 0xAF, 0xB3, 0xF9, 0x1D, 0xE0, 0x94, 0x00, 0x12, 0x19, 0x1C, 0xBA, 0x13, 0x24, 0x40, 0xE8, 0xEF, 0x57, 0x0B, 0x10, 0xD8, 0x54, 0xE8, 0x52, 0x02, 0x80, 0xBD, 0x77,
    0xA8, 0xF2, 0xBB, 0xC4, 0x29, 0x01, 0xC8, 0xE6, 0x42, 0x96, 0xFF, 0x22, 0x32, 0xA4, 0x4F, 0x18, 0xF5, 0x33, 0x4E, 0x75, 0x82, 0x82, 0x82, 0xB6, 0xA3, 0x09, 0x19, 0xD8, 0xFB,
    0x74, 0xFA, 0x1F, 0xA7, 0x04, 0x20, 0x4E, 0x84, 0x6F, 0x00, 0x02, 0x81, 0xFE, 0x7E, 0xB5, 0x00, 0x21, 0x3E, 0x08, 0xFB, 0x08, 0x04, 0x7B, 0x5C, 0x32, 0xBA, 0x27, 0x8E, 0x53,
    0x95, 0xD3, 0x66, 0xA7, 0x56, 0x4D, 0xBF, 0xA9, 0x27, 0x36, 0x7A, 0x03, 0x6D, 0x56, 0x83, 0x7C, 0x2F, 0x6C, 0x21, 0x02, 0x20, 0x79, 0x76, 0x4F, 0x1C, 0x53, 0xF5, 0xCB, 0xF4,
    0xD3, 0xD7, 0x26, 0x57, 0x4E, 0x4D, 0x8A, 0xC8, 0x3A, 0x85, 0x59, 0x37, 0x60, 0xE9, 0x0B, 0xFF, 0xD1, 0x13, 0x9B, 0xC5, 0x20, 0xD7, 0x93, 0x67, 0x56, 0x4E, 0xBE, 0xCA, 0xF7,
    0xED, 0x4B, 0x30, 0x4E, 0xB8, 0x66, 0xA0, 0xBE, 0x98, 0xD8, 0xE0, 0x8B, 0xE3, 0x4B, 0x9E, 0x79, 0xAB, 0x18, 0xC7, 0xC9, 0x15, 0x53, 0xD2, 0x0C, 0xCE, 0x1C, 0x00, 0x10, 0x50,
    0x0B, 0x71, 0x63, 0x1F, 0x82, 0x58, 0x6B, 0x12, 0xB1, 0x8D, 0xB1, 0x85, 0xCF, 0xEE, 0x29, 0xC6, 0x6E, 0xA1, 0xC8, 0x3D, 0x02, 0x11, 0x9B, 0xF9, 0x79, 0x76, 0x26, 0x1C, 0x6E,
    0x78, 0x13, 0x03, 0x6A, 0xFA, 0xAA, 0x62, 0x9C, 0x26, 0x57, 0x4C, 0x9D, 0x44, 0xEB, 0xB7, 0x1E, 0xB8, 0x10, 0x10, 0xC2, 0xB8, 0xA0, 0xDA, 0xD1, 0x54, 0xEF, 0x2B, 0xDA, 0x7C,
    0x62, 0x77, 0x72, 0x55, 0xD9, 0x1B, 0xA9, 0x15, 0x65, 0x5F, 0x6B, 0x59, 0x3B, 0xA7, 0x47, 0x2D, 0xAD, 0x3B, 0x9C, 0xEA, 0x03, 0x96, 0xED, 0x6A, 0x86, 0x71, 0xF7, 0x85, 0x2E,
    0x29, 0x02, 0x40, 0xA7, 0x17, 0xE7, 0x96, 0x4B, 0x60, 0xF2, 0xEE, 0x0B, 0x12, 0x62, 0x48, 0xE3, 0x92, 0xD6, 0x5E, 0xAC, 0xEA, 0xFD, 0x93, 0x1C, 0x3F, 0xF2, 0x7E, 0x4B, 0xD5,
    0xCC, 0xA2, 0x84, 0x0E, 0x83, 0x76, 0x2C, 0x04, 0xA8, 0x0F, 0x3F, 0x17, 0x10, 0x50, 0x79, 0xD3, 0xC7, 0x2B, 0xAE, 0x1F, 0x5A, 0x88, 0xC3, 0x96, 0xEF, 0x4F, 0x3F, 0x07, 0xD4,
    0x39, 0x79, 0x76, 0x9D, 0x3B, 0x7F, 0x17, 0x43, 0x38, 0x11, 0x34, 0x2A, 0xFB, 0x6C, 0xD9, 0xDD, 0xBE, 0x1A, 0x1C, 0xE7, 0x39, 0xA8, 0x86, 0x2D, 0x4B, 0x50, 0x4D, 0x5C, 0xFC,
    0x15, 0x05, 0x39, 0x74, 0xB8, 0xBA, 0x80, 0x03, 0x07, 0x81, 0x71, 0x5E, 0x1D, 0x5B, 0xFE, 0x7C, 0x4B, 0x21, 0x3E, 0x0A, 0xE2, 0xD3, 0xF6, 0x4B, 0xEC, 0xD0, 0xD6, 0x17, 0xE1,
    0xB8, 0x2D, 0x12, 0x7A, 0x51, 0x20, 0x54, 0x3F, 0x33, 0x2F, 0xB5, 0x6A, 0xEA, 0xDF, 0x86, 0xC9, 0xDD, 0xB2, 0xAA, 0xEC, 0xAB, 0xEA, 0xA7, 0xA6, 0x74, 0xA8, 0xFD, 0xDC, 0x86,
    0xA8, 0xB4, 0x8E, 0xB0, 0xAD, 0xA0, 0x82, 0xC0, 0xD3, 0xE1, 0xB8, 0x14, 0x87, 0xF6, 0x8F, 0xC0, 0x83, 0x50, 0x63, 0x64, 0x0D, 0x19, 0xBA, 0x15, 0x00, 0xC6, 0x85, 0x7A, 0x99,
    0x87, 0x53, 0xAB, 0xCA, 0x9E, 0x38, 0xBE, 0x7C, 0xF6, 0xF0, 0x7C, 0x59, 0x8E, 0xAF, 0x9D, 0x3D, 0x3C, 0x55, 0x39, 0xED, 0x07, 0xF0, 0xD2, 0xDF, 0x69, 0x7F, 0xE2, 0x44, 0x81,
    0xFA, 0x00, 0xE4, 0xA0, 0x40, 0x9E, 0x87, 0xC8, 0x2B, 0xD9, 0x2D, 0x73, 0x01, 0x20, 0x02, 0x02, 0xCA, 0x75, 0x85, 0x87, 0x15, 0x1E, 0x9D, 0x6A, 0x3A, 0xB9, 0x7A, 0xE6, 0x38,
    0x64, 0x5A, 0xDE, 0x61, 0x01, 0xC7, 0x58, 0x00, 0x00, 0x52, 0x60, 0x44, 0x8D, 0x71, 0x7E, 0x4C, 0xE0, 0x17, 0xA2, 0x7A, 0x18, 0xC6, 0x8C, 0x05, 0xE4, 0x33, 0xAA, 0xFE, 0x35,
    0x50, 0x1A, 0x48, 0x9B, 0x9A, 0x57, 0x5F, 0x8C, 0x71, 0xEB, 0x5B, 0x22, 0xD1, 0x07, 0xAB, 0x2F, 0x18, 0xAC, 0x09, 0xF5, 0x3F, 0xF5, 0x5E, 0x22, 0xBA, 0xF3, 0xA1, 0x3D, 0x1F,
    0xCE, 0xA4, 0xFA, 0xD5, 0x30, 0x2E, 0x44, 0x64, 0x57, 0xA2, 0x62, 0xDB, 0x75, 0x3D, 0x8E, 0xB2, 0x0B, 0xE4, 0x6D, 0xEA, 0xC9, 0x15, 0x53, 0xD7, 0x52, 0xED, 0xED, 0xED, 0x08,
    0x17, 0x62, 0x8F, 0xAD, 0x67, 0x1B, 0x82, 0x36, 0xBB, 0x4C, 0x59, 0x5B, 0x22, 0x42, 0x8B, 0x88, 0x44, 0xBE, 0xB4, 0xEA, 0xA2, 0xA1, 0x8C, 0x50, 0xBF, 0x43, 0x6A, 0xA9, 0x40,
    0x20, 0x62, 0xFC, 0x97, 0x47, 0xC6, 0x46, 0xAE, 0x7C, 0xF9, 0xE0, 0xD3, 0xB0, 0x99, 0x09, 0x26, 0x9E, 0x98, 0x11, 0x2F, 0x7F, 0x71, 0x4B, 0x31, 0x81, 0x85, 0x45, 0xDE, 0xB1,
    0x28, 0x9D, 0xE4, 0xDD, 0x10, 0xE3, 0x21, 0x74, 0x5F, 0x90, 0x43, 0x10, 0xB9, 0x48, 0xD0, 0x83, 0x07, 0x02, 0xE6, 0x4E, 0x7A, 0x44, 0x44, 0x00, 0xCD, 0x64, 0x22, 0xD1, 0xE9,
    0xAB, 0x2F, 0x1A, 0x3A, 0xC9, 0x55, 0x7F, 0x85, 0x88, 0x29, 0x15, 0xE3, 0x52, 0xC4, 0x80, 0xA0, 0x7B, 0xE5, 0x47, 0x99, 0x47, 0x12, 0x3E, 0xCA, 0x25, 0x3A, 0xF0, 0xD1, 0x7C,
    0xC1, 0xD7, 0x34, 0xDC, 0x32, 0xA3, 0x7A, 0xC7, 0x2D, 0xEF, 0xD7, 0x6C, 0x9F, 0x7F, 0x75, 0x57, 0x44, 0x5E, 0x7D, 0xB5, 0x32, 0x12, 0x86, 0x70, 0x5E, 0x01, 0x86, 0xDC, 0xB3,
    0xED, 0x98, 0x38, 0xCE, 0xDD, 0xC1, 0xF3, 0xD9, 0x6B, 0x10, 0x5A, 0x1F, 0x06, 0xF2, 0x97, 0x4F, 0x9E, 0x37, 0xEC, 0xEF, 0x1D, 0xDA, 0x1B, 0xC5, 0xB8, 0x74, 0xAD, 0xF2, 0x82,
    0xE3, 0x29, 0x0C, 0x4B, 0x5B, 0x00, 0x86, 0xA0, 0xDE, 0xB1, 0xFA, 0x8F, 0x46, 0xEC, 0xAD, 0x1A, 0x57, 0xB2, 0x31, 0x9F, 0x11, 0xF5, 0x59, 0x63, 0xAD, 0x7F, 0xB6, 0xA5, 0x96,
    0xE7, 0x4B, 0x5F, 0x53, 0x7F, 0xDB, 0xC4, 0xEA, 0x1D, 0xF3, 0xFF, 0xE7, 0xF5, 0x63, 0x3B, 0x33, 0xD5, 0xDB, 0xE7, 0xEF, 0xAF, 0xDA, 0x71, 0xFB, 0x15, 0x5D, 0x91, 0x3A, 0xED,
    0x29, 0x65, 0x62, 0xE9, 0xD6, 0x7F, 0x83, 0x1B, 0xAD, 0x07, 0xD9, 0x3B, 0x1B, 0x1F, 0x6A, 0xE1, 0xAA, 0xDC, 0xB3, 0x66, 0xFC, 0xC8, 0x29, 0x50, 0x6F, 0x02, 0xC4, 0xE1, 0xA8,
    0xA4, 0x87, 0xBF, 0x78, 0xB7, 0x11, 0x57, 0x1D, 0x6E, 0xC6, 0xF4, 0xDF, 0x7E, 0xCC, 0x98, 0x2A, 0x04, 0x42, 0x50, 0xA3, 0x71, 0x89, 0x5C, 0xDE, 0xD1, 0x44, 0xD5, 0x8E, 0x79,
    0xA3, 0xAD, 0x7A, 0xA3, 0x44, 0x0C, 0x00, 0xE7, 0xFC, 0x8E, 0xE9, 0x35, 0xF5, 0xB7, 0xCF, 0xCF, 0xF8, 0xE9, 0x7A, 0x55, 0x5E, 0x0A, 0x08, 0x95, 0x3C, 0x8B, 0x9A, 0xAE, 0x9B,
    0xB3, 0x09, 0xCE, 0xE9, 0x68, 0x75, 0x79, 0x4C, 0x9B, 0x18, 0xA8, 0x37, 0x88, 0x91, 0xFD, 0xE8, 0xF1, 0x95, 0x5A, 0x8A, 0x11, 0xB3, 0xAD, 0xEA, 0x8F, 0xCF, 0xDC, 0x67, 0xFC,
    0xCC, 0x32, 0x88, 0xE1, 0xB0, 0x94, 0x8F, 0x29, 0xFB, 0x8F, 0x23, 0xA2, 0x84, 0x0A, 0x60, 0x08, 0x8C, 0x48, 0xF9, 0xB9, 0xD9, 0x90, 0xAF, 0xFE, 0x6F, 0x3A, 0x5A, 0x71, 0xDC,
    0x78, 0x69, 0x2B, 0x15, 0x23, 0xD2, 0x2E, 0xA8, 0xB5, 0xDB, 0x17, 0x9C, 0x6D, 0xAD, 0xB7, 0xDE, 0x88, 0x83, 0x36, 0x67, 0xB7, 0x24, 0x39, 0x74, 0xE2, 0xE0, 0x9B, 0xEF, 0x38,
    0x1D, 0xB3, 0x2E, 0x05, 0x90, 0xDB, 0xB7, 0x9F, 0x44, 0x62, 0xC0, 0x35, 0x62, 0xDC, 0xA6, 0xAC, 0xC1, 0x82, 0x11, 0x3C, 0xF7, 0xD2, 0xB2, 0x77, 0x78, 0x49, 0x45, 0x24, 0xE3,
    0xAF, 0x33, 0xC6, 0x85, 0x4B, 0x60, 0xE2, 0xA1, 0x13, 0x1D, 0xA7, 0xA1, 0xE2, 0x90, 0x12, 0x0C, 0x7E, 0x0A, 0xC7, 0xE3, 0xAF, 0xF3, 0x98, 0xCB, 0x9E, 0x26, 0x11, 0x96, 0x7E,
    0xBB, 0x6D, 0xF9, 0x0C, 0xBC, 0xF5, 0x80, 0x0A, 0xA1, 0x30, 0x62, 0x6A, 0x00, 0xD9, 0x4C, 0x50, 0x44, 0x04, 0x0E, 0x30, 0xEB, 0x74, 0xFC, 0xBA, 0x3D, 0xA8, 0x4F, 0x2C, 0x7C,
    0xF6, 0xBD, 0x78, 0xA2, 0xE4, 0x12, 0x88, 0xB3, 0x2F, 0xB8, 0xF2, 0x52, 0x58, 0x6B, 0xA0, 0xF5, 0x10, 0x85, 0xDC, 0xF5, 0xB3, 0x11, 0xB1, 0xAF, 0x13, 0x2C, 0x55, 0x08, 0x3F,
    0x7F, 0xA4, 0x19, 0x03, 0x7C, 0x3D, 0x35, 0xF7, 0xA5, 0x8A, 0x11, 0xF3, 0xFE, 0xF1, 0x01, 0x89, 0xFD, 0x42, 0x8A, 0x11, 0xD7, 0x2F, 0x9F, 0x55, 0xFB, 0xDB, 0x4E, 0xC6, 0x3C,
    0x2F, 0x1E, 0x84, 0x0F, 0x08, 0xB0, 0xBF, 0xF5, 0xE7, 0xAA, 0x6D, 0x73, 0xA7, 0x5B, 0xF5, 0xAF, 0x56, 0x2A, 0x1C, 0xE3, 0xDE, 0xB5, 0x68, 0xF2, 0xC6, 0x45, 0x4B, 0xA6, 0x3C,
    0x39, 0x0B, 0xC4, 0x41, 0x40, 0x20, 0x46, 0x2E, 0x2B, 0x5A, 0x00, 0x00, 0x90, 0x3B, 0x6A, 0x0F, 0x26, 0x46, 0x94, 0x5E, 0x62, 0xDC, 0xC8, 0xF7, 0x85, 0x16, 0x61, 0xEF, 0xFD,
    0x08, 0x20, 0x22, 0xEE, 0xEE, 0xEF, 0x8D, 0x3F, 0xE3, 0x35, 0x51, 0x5D, 0x28, 0x30, 0x18, 0x9D, 0xF4, 0x70, 0xC1, 0xF1, 0x74, 0x6E, 0x4C, 0x84, 0xFA, 0xE2, 0x44, 0xE3, 0x5B,
    0x23, 0x25, 0x9F, 0xF9, 0x74, 0x93, 0xC3, 0xE1, 0x00, 0x40, 0xF2, 0xED, 0xBC, 0x64, 0x23, 0x4E, 0x49, 0x76, 0x9C, 0x05, 0x29, 0xEF, 0xB5, 0xFE, 0xAE, 0x34, 0x8F, 0x88, 0x18,
    0xB8, 0x4E, 0xA4, 0x6E, 0xD1, 0xA4, 0x0D, 0xAB, 0xDA, 0x14, 0xA9, 0x25, 0x08, 0xA5, 0x9E, 0xF5, 0xD8, 0x0B, 0x65, 0xB1, 0xA2, 0x05, 0x00, 0x00, 0x99, 0x5B, 0x6B, 0xE3, 0x4B,
    0xEB, 0xFE, 0x4A, 0x4C, 0x64, 0xBC, 0x31, 0xCE, 0x93, 0x22, 0x50, 0xD0, 0x76, 0x79, 0x09, 0x8A, 0xEA, 0x23, 0x9E, 0x4A, 0xDF, 0x59, 0xA2, 0xDE, 0x77, 0x8D, 0x18, 0xA8, 0x80,
    0x57, 0x1C, 0x69, 0x16, 0x05, 0x45, 0xD4, 0x17, 0x71, 0x23, 0x6F, 0x9A, 0xC4, 0x90, 0x1B, 0x62, 0x4B, 0xB6, 0x4C, 0x7F, 0x7C, 0xEC, 0x87, 0x7F, 0x6A, 0xAD, 0x37, 0x20, 0x2B,
    0x40, 0xDE, 0x93, 0x62, 0xEB, 0xF9, 0x03, 0xB3, 0x57, 0x8C, 0x01, 0xCA, 0xBB, 0x00, 0xB0, 0x7A, 0xFB, 0xCD, 0x33, 0x08, 0x7B, 0x91, 0x88, 0x81, 0x3A, 0xB2, 0xB8, 0x3D, 0x01,
    0xBE, 0x04, 0x12, 0x24, 0x11, 0x8D, 0x0E, 0xEA, 0xD4, 0x69, 0x02, 0x79, 0x2E, 0x49, 0x75, 0x87, 0x78, 0xC5, 0x8B, 0x6F, 0x02, 0x98, 0xDF, 0xF8, 0xF8, 0x0D, 0x77, 0xC5, 0x93,
    0xE9, 0x9B, 0x60, 0x64, 0x1A, 0x54, 0xAF, 0x80, 0x7A, 0x67, 0x93, 0x6D, 0xA4, 0x20, 0x01, 0x71, 0x77, 0x3C, 0x74, 0xF9, 0xA8, 0x8F, 0x46, 0xA5, 0xFC, 0x69, 0x10, 0x17, 0xE7,
    0x34, 0xA7, 0x64, 0xA4, 0x2F, 0xEF, 0x68, 0x2C, 0xBE, 0xC3, 0x44, 0x13, 0x1B, 0xA2, 0x5F, 0xA8, 0xDD, 0x95, 0x0B, 0x8E, 0xF6, 0x6E, 0x23, 0x06, 0x00, 0x01, 0x23, 0x9D, 0x9B,
    0x3F, 0x00, 0x18, 0x33, 0x0C, 0x1A, 0x9C, 0x4B, 0x59, 0x5F, 0xDF, 0x0C, 0xFC, 0xC8, 0x7D, 0x02, 0x81, 0x81, 0xA9, 0x5E, 0x34, 0x61, 0x7D, 0xBB, 0xEB, 0xF7, 0x91, 0xB8, 0xFB,
    0x56, 0x26, 0xED, 0x41, 0x20, 0x30, 0x8A, 0x4F, 0x01, 0xF8, 0x55, 0x8F, 0x05, 0x68, 0xC5, 0xD0, 0x85, 0xCF, 0x7E, 0x0C, 0x60, 0x4D, 0xF6, 0x03, 0x3E, 0xFC, 0xF9, 0x44, 0x66,
    0xF0, 0xF0, 0xF3, 0x15, 0x32, 0x5A, 0x3D, 0x7F, 0x68, 0x24, 0x36, 0xE8, 0x8C, 0xC8, 0x89, 0xC6, 0xE7, 0x46, 0x67, 0xE4, 0xAF, 0x29, 0x06, 0xA4, 0x8F, 0x63, 0x83, 0x87, 0x7F,
    0x2E, 0x7A, 0x63, 0xF5, 0x6B, 0x1D, 0x6D, 0xAD, 0x6D, 0xF8, 0xE2, 0xE7, 0x52, 0x99, 0xA6, 0x19, 0x22, 0x0E, 0x08, 0x05, 0x14, 0x1F, 0xE4, 0xF3, 0x49, 0xE5, 0x08, 0x08, 0x60,
    0x60, 0xB0, 0x74, 0xC6, 0x53, 0x6F, 0x2C, 0xDF, 0x7A, 0xE3, 0x65, 0x4A, 0xFF, 0xCF, 0x8C, 0x71, 0x01, 0xEA, 0xFD, 0x1D, 0xF3, 0xC7, 0x1C, 0x1C, 0xF0, 0xB2, 0xFF, 0x1B, 0x31,
    0x63, 0xF3, 0xD9, 0x2C, 0x5A, 0x80, 0x8E, 0x90, 0x7B, 0x7E, 0x9A, 0x04, 0xB0, 0x37, 0xFB, 0xC9, 0x61, 0xF5, 0xB6, 0x9B, 0x6F, 0x15, 0x00, 0xC6, 0xB8, 0xAF, 0xDF, 0x3A, 0xB1,
    0x73, 0xF0, 0x00, 0xE0, 0x6B, 0xBA, 0x26, 0x3B, 0xAA, 0x05, 0x6D, 0x88, 0xFC, 0x28, 0xAF, 0x0F, 0xE1, 0x19, 0x50, 0x00, 0x22, 0x6F, 0x03, 0x60, 0xD4, 0x89, 0x7E, 0x0D, 0x54,
    0x08, 0xE4, 0x07, 0x8B, 0xA6, 0x6C, 0xEC, 0xF4, 0x36, 0xCA, 0xAD, 0x7F, 0xBE, 0xA1, 0xB1, 0x6A, 0xFB, 0x3C, 0x55, 0xAA, 0x51, 0x20, 0xEF, 0xE9, 0x77, 0x9F, 0x5E, 0x05, 0x5D,
    0xBE, 0x65, 0xF6, 0x45, 0x4A, 0x7B, 0x26, 0x41, 0x88, 0x98, 0x7F, 0xCF, 0x97, 0xE7, 0xF1, 0x86, 0x2F, 0x3C, 0xE0, 0xFB, 0x99, 0xCF, 0xE6, 0x96, 0x0A, 0x04, 0x08, 0x1C, 0xCD,
    0x97, 0x57, 0x81, 0x31, 0x00, 0x40, 0xEA, 0xDE, 0xE5, 0xDB, 0xE6, 0x8D, 0x01, 0x19, 0x5C, 0xEA, 0xA2, 0xFB, 0x2F, 0xA7, 0x25, 0x41, 0x39, 0x92, 0x5D, 0x5C, 0x9E, 0x97, 0x2F,
    0xB9, 0x4F, 0x05, 0x70, 0x5C, 0xF7, 0x32, 0x81, 0xC9, 0xDE, 0x26, 0x89, 0xFD, 0x57, 0xC7, 0xF4, 0xB5, 0x0D, 0x0B, 0xAE, 0xF6, 0xBC, 0xD4, 0xFD, 0x80, 0xC0, 0x48, 0x64, 0x03,
    0x40, 0x81, 0x00, 0xE2, 0x60, 0xD8, 0x69, 0x4C, 0x06, 0xB5, 0x48, 0xFE, 0x22, 0x0A, 0xDE, 0x07, 0x00, 0xAE, 0x13, 0xD9, 0xB9, 0x78, 0xCA, 0x0F, 0xF3, 0xCD, 0x19, 0x02, 0x08,
    0x0E, 0x05, 0x7F, 0x30, 0x3E, 0x5F, 0x72, 0xDF, 0x0A, 0x40, 0xE7, 0xFC, 0xD6, 0x4E, 0xDB, 0xCF, 0xB4, 0xBC, 0xD9, 0x36, 0x6D, 0x63, 0x43, 0xF9, 0xB9, 0x19, 0xEB, 0xD7, 0x11,
    0x80, 0x11, 0xF3, 0xDA, 0xD1, 0x74, 0xE3, 0x32, 0x0D, 0xF6, 0x02, 0x20, 0x30, 0xA3, 0xF2, 0xD9, 0x13, 0xF2, 0x7C, 0x11, 0x81, 0x11, 0xE7, 0x72, 0x81, 0x54, 0x10, 0x0A, 0x85,
    0x7D, 0xA0, 0x4B, 0x12, 0x22, 0xEF, 0x66, 0xFB, 0xE5, 0x3F, 0x61, 0x9E, 0x69, 0x7D, 0xDF, 0x0A, 0xE0, 0xB8, 0x63, 0x5B, 0x87, 0x05, 0xD7, 0x29, 0xCD, 0xBD, 0x08, 0xB1, 0xE9,
    0x27, 0xE5, 0x63, 0x9B, 0x6D, 0xD3, 0xCB, 0x54, 0x1D, 0x28, 0x90, 0x93, 0x6A, 0x9D, 0x99, 0xF7, 0xCE, 0xD8, 0xF2, 0xB1, 0x11, 0x17, 0x12, 0x28, 0xD6, 0xA9, 0xB6, 0x1A, 0x1A,
    0xAE, 0x75, 0x95, 0x7A, 0x26, 0x20, 0xA0, 0x60, 0x2A, 0x00, 0x11, 0x71, 0xF6, 0x2C, 0x99, 0xF4, 0xD4, 0x7F, 0x76, 0xC5, 0x81, 0xC0, 0xAF, 0x82, 0xB9, 0x80, 0x75, 0x6A, 0xEA,
    0x6F, 0xBB, 0xF7, 0x87, 0xBB, 0x96, 0x9C, 0x59, 0xB9, 0xED, 0xE6, 0xBF, 0x59, 0xFE, 0x74, 0xB0, 0x79, 0xD3, 0xA7, 0x02, 0x58, 0x7A, 0x1F, 0x01, 0x80, 0x88, 0x01, 0x25, 0x7D,
    0x07, 0x00, 0x3C, 0xF1, 0xE3, 0x3B, 0xE7, 0x1C, 0x3F, 0xD9, 0xF4, 0x6B, 0xAA, 0x1D, 0x03, 0x08, 0x7C, 0x3F, 0x3D, 0xA9, 0x7C, 0xDA, 0xFA, 0x0F, 0x00, 0xD0, 0x88, 0x7C, 0x18,
    0xBC, 0x36, 0x80, 0xB2, 0x8E, 0xB6, 0xF6, 0xF9, 0x67, 0x9C, 0xA7, 0xC1, 0x4E, 0x15, 0x01, 0x50, 0x69, 0x61, 0x89, 0xFB, 0xBA, 0xE3, 0x20, 0xE0, 0xCF, 0x48, 0x42, 0x60, 0x68,
    0xD5, 0xFB, 0x76, 0x4B, 0xBA, 0xF1, 0x90, 0x55, 0xEF, 0xD1, 0xF8, 0xC0, 0x92, 0x2B, 0x81, 0x3E, 0x16, 0x40, 0x7D, 0xEE, 0x66, 0xD0, 0x4B, 0x33, 0xE3, 0x27, 0xFF, 0xA1, 0x6A,
    0xC7, 0x2D, 0x7E, 0x32, 0xDD, 0xB4, 0x89, 0xD4, 0x41, 0xC1, 0x14, 0xD5, 0x9D, 0x55, 0x31, 0xFD, 0x47, 0x3F, 0x6D, 0xCD, 0x4F, 0xE0, 0xE7, 0xC1, 0x5F, 0x0E, 0xAD, 0xDE, 0x7E,
    0xCB, 0x43, 0x6D, 0x6D, 0xC5, 0x9C, 0xC4, 0xB8, 0x36, 0xED, 0x57, 0xC4, 0x38, 0x7B, 0x2A, 0xA6, 0x3E, 0xD5, 0xED, 0xDB, 0x25, 0x07, 0x9C, 0x43, 0x5B, 0x8D, 0x71, 0x9A, 0x00,
    0x88, 0xC0, 0x00, 0x10, 0x44, 0x4C, 0xEC, 0xED, 0xC5, 0x53, 0xD7, 0x6D, 0x06, 0xFA, 0x58, 0x80, 0xBB, 0xA6, 0xD7, 0xFE, 0x48, 0x60, 0x0E, 0x04, 0x1B, 0x9E, 0x06, 0x54, 0x0D,
    0x56, 0x70, 0x82, 0x23, 0x06, 0xB8, 0x7C, 0xF1, 0xA4, 0x75, 0xED, 0x2F, 0x59, 0x0A, 0x36, 0x07, 0x82, 0x19, 0xFA, 0x9A, 0xF9, 0x7A, 0xCD, 0x8E, 0xDB, 0xBE, 0xD4, 0x9A, 0x64,
    0xB5, 0xE5, 0x28, 0x83, 0x37, 0x09, 0xC4, 0xAA, 0x07, 0x92, 0x7F, 0x17, 0x86, 0xC3, 0x83, 0x13, 0x76, 0xF9, 0x46, 0xA2, 0x37, 0x88, 0x31, 0xEF, 0x18, 0x31, 0x2D, 0x8E, 0x71,
    0x36, 0x78, 0x19, 0xC9, 0x2D, 0xB5, 0x7B, 0x6D, 0x1E, 0x70, 0x5A, 0xD0, 0x4C, 0x26, 0xF8, 0xB4, 0xD2, 0xBB, 0xC4, 0x31, 0x91, 0x83, 0xA0, 0x3C, 0x7E, 0xB0, 0xD1, 0x7E, 0xEB,
    0xC1, 0xB9, 0xB5, 0x99, 0x8E, 0x59, 0x5B, 0x7E, 0xD9, 0x58, 0x95, 0xB8, 0x64, 0xC8, 0xB7, 0x09, 0x96, 0x92, 0x04, 0x45, 0x4F, 0xB4, 0xA6, 0x7D, 0x71, 0xD2, 0xA6, 0xD7, 0x6B,
    0xEA, 0x6F, 0x6D, 0x50, 0xEA, 0x04, 0x57, 0xA2, 0xF7, 0x2E, 0x9E, 0xBC, 0xB1, 0x3E, 0x2C, 0x85, 0x45, 0x93, 0x9E, 0x68, 0x00, 0x90, 0x77, 0x2A, 0xDC, 0x6F, 0xD8, 0xD4, 0x50,
    0x51, 0x12, 0x26, 0x5F, 0xD5, 0x0B, 0xF3, 0x3E, 0x5B, 0xBD, 0x7D, 0xFE, 0xFA, 0xCA, 0x17, 0xE7, 0xCE, 0xCB, 0x97, 0x4E, 0x6E, 0x3A, 0xED, 0xE6, 0xC6, 0x27, 0xF8, 0x04, 0x85,
    0xE3, 0xFF, 0x00, 0xA9, 0xA9, 0x93, 0x90, 0x47, 0x08, 0x92, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};
} // namespace WEB
} // namespace MaSiRoProject
#endif
