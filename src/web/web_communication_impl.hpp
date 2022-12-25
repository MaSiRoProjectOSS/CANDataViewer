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

public:
    bool is_ap_mode();
    IPAddress get_ip();
    const char *get_ssid();
    const char *get_ssid_ap_default();
    void request_connection_info(std::string ssid, std::string pass, bool ap_mode);

    bool set_callback_message(MessageFunction callback);

    std::vector<NetworkList> get_wifi_list();

    bool check_connection();

private:
    void happened_message(OUTPUT_LOG_LEVEL level, const char *message, const char *function_name, const char *file_name, int line);
    bool _save_information(std::string ssid, std::string pass, bool ap_mode);
    MessageFunction callback_message;
    bool load_information();
    int _get_rssi_as_quality(int rssi);
    bool _load_information_for_spiffs();

private:
    bool _running = false;

    bool _open_fs         = false;
    bool _mode_ap         = SETTING_WIFI_MODE_AP;
    bool _mode_ap_current = SETTING_WIFI_MODE_AP;

    std::string _ssid_ap = SETTING_WIFI_SSID_AP;

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
    class config_address {
    public:
        IPAddress local_ip = INADDR_NONE;
        IPAddress gateway  = INADDR_NONE;
        IPAddress subnet   = INADDR_NONE;
        bool flag_set      = false;
    };
    config_address _config_ap;
    config_address _config_sta;

    void set_config_address_ap(IPAddress ip = INADDR_NONE, IPAddress gateway = INADDR_NONE, IPAddress subnet = INADDR_NONE);
    void set_config_address_sta(IPAddress ip = INADDR_NONE, IPAddress gateway = INADDR_NONE, IPAddress subnet = INADDR_NONE);
};

} // namespace WEB
} // namespace MaSiRoProject
#endif