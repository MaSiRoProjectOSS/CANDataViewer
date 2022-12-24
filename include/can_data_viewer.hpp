/**
 * @file can_data_viewer.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-20
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_CAN_DATA_VIEWER_HPP
#define MASIRO_PROJECT_CAN_DATA_VIEWER_HPP

#include "can_data_viewer_info.hpp"

#include <WebServer.h>
#if STORAGE_SPI_FS
#include <SPIFFS.h>
#endif

class CanDataViewer {
    ////////////////////////////////////////////////
    // standard function
    ////////////////////////////////////////////////
public:
    CanDataViewer();
    ~CanDataViewer();
    bool begin(std::string ssid = "", std::string pass = "", bool ap_mode = true);

    ////////////////////////////////////////////////
    // setup function
    ////////////////////////////////////////////////
public:
    bool set_callback_message(MessageFunction callback);
    bool set_callback_changed_mode(ChangedModeFunction callback);
    bool set_callback_received(GetReceivedFunction callback);
    bool set_callback_setting_default(SettingDefaultFunction callback);

    void set_wifi_info(std::string ssid, std::string pass, bool ap_mode);
    void set_config_address_ap(IPAddress ip = INADDR_NONE, IPAddress gateway = INADDR_NONE, IPAddress subnet = INADDR_NONE);
    void set_config_address_sta(IPAddress ip = INADDR_NONE, IPAddress gateway = INADDR_NONE, IPAddress subnet = INADDR_NONE);

    ////////////////////////////////////////////////
    // control function
    ////////////////////////////////////////////////
public:
    bool set_mode(CAN_CTRL_STATE mode = CAN_CTRL_STATE::MODE_UNKNOW);
    bool clear_resume(void);
    bool clear_loop_shot(void);
    bool add_one_shot(CanData data);
    bool add_loop_shot(CanData data, int interval);
    bool add_resume(CanData data);

    ////////////////////////////////////////////////
    // debug function
    ////////////////////////////////////////////////
#if DEBUG_MODE
public:
    UBaseType_t get_stack_high_water_mark_can();
    UBaseType_t get_stack_high_water_mark_server();
    UBaseType_t get_stack_size_can();
    UBaseType_t get_stack_size_server();
#endif
};

#endif
