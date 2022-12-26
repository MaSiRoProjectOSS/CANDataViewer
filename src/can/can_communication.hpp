/**
 * @file can_communication.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_COMMUNICATION_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_COMMUNICATION_HPP

#include "can_data_viewer_info.hpp"

#include <vector>

namespace MaSiRoProject
{
namespace CAN
{

class CanCommunication {
public:
    CanCommunication();
    ~CanCommunication();
    bool set_callback_message(MessageFunction callback);
    bool set_callback_changed_mode(ChangedModeFunction callback);
    bool set_callback_received(GetReceivedFunction callback);
    bool set_callback_sendable(SendEventFunction callback);
    bool set_callback_setting_default(SettingDefaultFunction callback);

public:
    bool begin();

public:
    CanDeviceInfo get_device_info();
    std::vector<CanData> get_send_loop();
    std::vector<CanData> get_received();
    std::vector<CanData> get_resume();

    bool add_one_shot(CanData data);
    bool add_loop_shot(CanData data, int interval);
    bool add_resume(CanData data);

    bool delete_loop_shot(unsigned long Id);
    bool clear_loop_shot();

    bool delete_resume(unsigned long Id);
    bool clear_resume();

public:
    bool request_pause();
    bool request_running();
    bool change_mode(CAN_CTRL_STATE mode);
    bool setup_default(void);
    bool setup_callback(void);

#if DEBUG_MODE
public:
    UBaseType_t get_stack_high_water_mark();
    UBaseType_t get_stack_size();
#endif

private:
    byte interrupt;
    SettingDefaultFunction callback_setting_default;
    TaskHandle_t task_handle;
    UBaseType_t task_assigned_size;
};
} // namespace CAN
} // namespace MaSiRoProject
#endif