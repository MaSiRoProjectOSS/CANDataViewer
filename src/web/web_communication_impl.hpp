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

#include "../can_data_viewer_conf.hpp"
#include "can_data_viewer_info.hpp"

#include <WiFi.h>
#include <vector>

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
    bool connect(std::string ssid, std::string pass, bool ap_mode);
    bool is_connected(bool force = false);
    bool disconnect();
    bool loop();

public:
    bool is_ap_mode();
    IPAddress get_ip();
    const char *get_ssid();
    bool save_information(std::string ssid, std::string pass, bool ap_mode, bool reconnecting, bool is_save);
    bool set_callback_message(MessageFunction callback);

    std::vector<NetworkList> get_wifi_list();

private:
    MessageFunction callback_message;
    void happened_message(bool is_error, const char *message);
    bool reconnect();
    bool load_information();
    int get_rssi_as_quality(int rssi);

private:
    bool _load_information();
    bool _open_fs = false;
    bool _mode_ap = SETTING_WIFI_MODE_AP;

    /**
     * @brief AP SSID
     *
     */
    std::string _ssid = SETTING_WIFI_SSID_AP;
    /**
     * @brief AP password
     *
     */
    std::string _pass = SETTING_WIFI_PASS_AP;
};

} // namespace WEB
} // namespace MaSiRoProject
#endif