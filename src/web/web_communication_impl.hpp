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
    virtual bool setup() final;
    bool begin();
    bool end();
    bool loop();
    IPAddress get_ip();
    const char *get_ssid();
    bool save_information(std::string ssid, std::string pass, bool ap_mode, bool reconnecting);

private:
    bool set_callback_message(MessageFunction callback);
    MessageFunction callback_message;
    void happened_message(bool is_error, const char *message);
    bool reconnect();
    bool load_information();

private:
#if STORAGE_SPI_FS
    bool open_fs = false;
#endif
    bool _mode_ap = SETTING_WIFI_MODE_AP;

    /**
     * @brief AP SSID
     *
     */
    std::string _ssid = SETTING_WIFI_SSID;
    /**
     * @brief AP password
     *
     */
    std::string _pass = SETTING_WIFI_PASS;
};

} // namespace WEB
} // namespace MaSiRoProject
#endif