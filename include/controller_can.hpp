/**
 * @file controller_can.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef CONTROLLER_CAN_HPP
#define CONTROLLER_CAN_HPP

#include <can/can_communication.hpp>

namespace MaSiRoProject
{
namespace ToyBox
{
class ControllerCan : public CAN::CanCommunication {
public:
    ControllerCan();

public:
    bool setup_default(void) override;
    bool send_for_ready(CAN::CanData *data) override;
    bool send_for_running(CAN::CanData *data) override;
    bool send_for_stopping(CAN::CanData *data) override;

public:
    void push(CAN::CAN_CTRL_STATE mode);
};

} // namespace ToyBox
} // namespace MaSiRoProject
#endif