/**
 * @file web_communication.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_WEB_COMMUNICATION_HPP
#define MASIRO_PROJECT_TOY_BOX_WEB_COMMUNICATION_HPP

#include "can_data_viewer_info.hpp"

#include <WebServer.h>

namespace MaSiRoProject
{
namespace WEB
{
class WebCommunication {
public:
    WebCommunication();
    ~WebCommunication();

public:
    bool setup();
    bool begin();
    bool set_callback_message(MessageFunction callback);
    void set_message(std::string message);
    std::vector<NetworkList> get_wifi_list();
    void request_reconnect(std::string ssid, std::string pass, bool ap_mode, bool force);
    void set_config_address_ap(IPAddress ip = INADDR_NONE, IPAddress gateway = INADDR_NONE, IPAddress subnet = INADDR_NONE);
    void set_config_address_sta(IPAddress ip = INADDR_NONE, IPAddress gateway = INADDR_NONE, IPAddress subnet = INADDR_NONE);

protected:
    IPAddress get_ip();
    WebServer *get_server();
    virtual bool setup_server(WebServer *server);
    virtual void handle_not_found();
    void handle_network_css();
    void handle_network_js();
    void handle_network_html();
    void handle_favicon_ico();
    void handle_js_ajax();
    void set_network();
    void get_network();
    void get_net_list();

protected:
    void happened_message(OUTPUT_LOG_LEVEL level, const char *message, const char *function_name, const char *file_name, int line);
    String ip_to_string(IPAddress ip);
    byte to_byte(String data);
    int to_int(String data);
    unsigned long to_ulong(String data);

#if DEBUG_MODE
public:
    UBaseType_t get_stack_high_water_mark();
    UBaseType_t get_stack_size();
#endif

private:
    bool initialized = false;
    MessageFunction callback_message;
    TaskHandle_t task_handle;
    UBaseType_t task_assigned_size;
    std::string _message = "The system was reconfigured."
                           "<br />"
                           "Please change the network connection.";
};

} // namespace WEB
} // namespace MaSiRoProject
#endif
