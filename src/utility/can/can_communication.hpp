/**
 * @file can_communication.hpp
 * @brief CAN通信を管理するクラスのヘッダファイル
 * @version 0.1
 * @date 2022-12-05
 *
 * このファイルは、CAN通信の初期化、データ送受信、コールバック設定、モード変更などの機能を提供する
 * CanCommunicationクラスの宣言を含みます。CANデータビューア用ユーティリティです。
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_COMMUNICATION_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_COMMUNICATION_HPP

#include <can_data_viewer_info.hpp>
#include <vector>

namespace CAN
{

class CanCommunication {
public:
    /////////////////////////////////
    // Constructor
    /////////////////////////////////
#if LIB_CAN_DRIVER == 1
    CanCommunication(const uint8_t cs, const uint8_t interrupt = -1);
#else
    CanCommunication(const uint8_t rx, const uint8_t tx);
#endif
    ~CanCommunication();

    /////////////////////////////////
    // Setup
    /////////////////////////////////
    bool begin();
    bool setup_default(void);

    /////////////////////////////////
    // Set Callback
    /////////////////////////////////
    bool set_callback_changed_mode(ChangedModeFunction callback);
    bool set_callback_received(GetReceivedFunction callback);
    bool set_callback_sendable(SendEventFunction callback);
    bool set_callback_setting_default(SettingDefaultFunction callback);

    /////////////////////////////////
    // Getter / Setter
    /////////////////////////////////
public:
    CanDeviceInfo get_device_info();
    bool change_mode(CAN_CTRL_STATE mode);

    /////////////////////////////////
    // Getter / Setter for CAN Data
    /////////////////////////////////
public:
    std::vector<CanData> get_send_loop();
    std::vector<CanData> get_received();
    std::vector<CanData> get_resume();

    bool add_one_shot(CanData data);
    bool add_loop_shot(CanData data, int interval);
    bool add_resume(CanData data);

    bool delete_loop_shot(unsigned long Id);
    bool delete_resume(unsigned long Id);

    bool clear_loop_shot();
    bool clear_resume();

    /////////////////////////////////
    // Control function
    /////////////////////////////////
public:
    bool request_pause();
    bool request_running();

    /////////////////////////////////
    // water_mark
    /////////////////////////////////
public:
    UBaseType_t get_stack_high_water_mark();
    UBaseType_t get_stack_size();

    /////////////////////////////////
    // Member
    /////////////////////////////////
private:
    int8_t interrupt = -1;
    SettingDefaultFunction callback_setting_default;
    TaskHandle_t task_handle;
    UBaseType_t task_assigned_size;
};
} // namespace CAN

#endif
