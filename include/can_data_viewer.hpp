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
//////////////////////////////////////
#include "can/can_communication.hpp"
#include "web/controller_page.hpp"

#if STORAGE_SPI_FS
#include <SPIFFS.h>
#endif

class CanDataViewer {
public:
    CanDataViewer();
    ~CanDataViewer();

public:
    bool set_callback_message(MessageFunction callback);
    bool set_callback_changed_mode(ChangedModeFunction callback);
    bool set_callback_received(GetReceivedFunction callback);
    bool set_callback_setting_default(SettingDefaultFunction callback);
    bool set_wifi_info(std::string ssid, std::string pass, bool ap_mode);
    bool begin(std::string ssid = "", std::string pass = "", bool ap_mode = true);

public:
    bool set_mode(CAN_CTRL_STATE mode);
    bool clear_resume(void);
    bool clear_loop_shot(void);
    bool add_one_shot(CanData data);
    bool add_loop_shot(CanData data, int interval);
    bool add_resume(CanData data);
};

#endif
