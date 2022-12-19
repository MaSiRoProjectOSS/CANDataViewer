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

#include "can/can_info.hpp"
#include "common/common_function_def.hpp"
#include "conf/pin_setting.h"

#include <Arduino.h>
#include <SPI.h>
#include <vector>

namespace MaSiRoProject
{
namespace ToyBox
{
namespace CAN
{

class CanCommunication {
private:
public:
    CanCommunication();
    ~CanCommunication();

public:
    bool setup(ToyBoxMessageFunction callback_message, CAN::CanCommunicationChangedModeFunction callback_changed_mode);
    bool begin();

public:
    CanDeviceInfo get_device_info();
    std::vector<CanData> get_send_loop();
    std::vector<CanData> get_received();
    std::vector<CanData> get_resume();

    bool add_one_shot(CanData data);

    bool add_loop_shot(CanData data, int interval);
    bool delete_loop_shot(unsigned long Id);
    bool clear_loop_shot();

    bool delete_resume(unsigned long Id);
    bool clear_resume();

public:
    bool request_pause();
    bool request_running();
    bool change_mode(CAN_CTRL_STATE mode);

private:
    byte interrupt;

protected:
    bool set_resume(CanData data);
    virtual bool setup_default(void);
    virtual bool setup_callback(void);
    virtual bool send_for_ready(CanData *data);
    virtual bool send_for_running(CanData *data);
    virtual bool send_for_stopping(CanData *data);
};
} // namespace CAN
} // namespace ToyBox
} // namespace MaSiRoProject
#endif