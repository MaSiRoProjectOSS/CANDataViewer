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
    bool set_wifi_info(std::string ssid, std::string pass, bool ap_mode);

protected:
    WebServer *get_server();
    virtual bool setup_server(WebServer *server);
    virtual void handle_not_found();
    IPAddress get_ip();

protected:
    void request_reconnect(std::string ssid, std::string pass, bool ap_mode);
    void happened_message(bool is_error, const char *message);

private:
    bool initialized = false;
    MessageFunction callback_message;
};
} // namespace WEB
} // namespace MaSiRoProject
#endif
