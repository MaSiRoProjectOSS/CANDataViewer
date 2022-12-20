/**
 * @file web_communication_impl.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_WEB_COMMUNICATION_IMPL_HPP
#define MASIRO_PROJECT_TOY_BOX_WEB_COMMUNICATION_IMPL_HPP

#include "can_data_viewer_info.hpp"
#include "web_communication.hpp"

#include <WiFi.h>

namespace MaSiRoProject
{
namespace WEB
{
class WebCommunicationImpl {
    friend class WebCommunication;

public:
    WebCommunicationImpl();
    ~WebCommunicationImpl();

public:
    bool setup();
    bool begin();
    bool end();
    bool loop();
    IPAddress get_ip();
    char *get_ssid();

private:
    bool set_callback_message(MessageFunction callback);
    MessageFunction callback_message;
    void happened_message(bool is_error, const char *message);
    bool reconnect();

private:
    bool mode_ap = SETTING_WIFI_MODE_AP;

private:
    /**
     * @brief AP SSID
     *
     */
    char ssid[64];
    /**
     * @brief AP password
     *
     */
    char pass[64];
};

} // namespace WEB
} // namespace MaSiRoProject
#endif