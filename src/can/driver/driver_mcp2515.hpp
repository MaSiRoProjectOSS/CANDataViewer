/**
 * @file driver_mcp2515.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-09
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_MCP2515_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_MCP2515_HPP

#include "can/can_config.h"
#if !LIB_ESP32CAN
#include "can/can_info.hpp"

#include <mcp_can.h>

namespace MaSiRoProject
{
namespace ToyBox
{
namespace CAN
{
class DriverMcp2515 {
public:
    typedef std::function<void(bool, const char *, bool)> MessageFunction;
    /////////////////////////////////
    // setup function
    /////////////////////////////////
public:
    bool setup_can(byte mode, byte speed, byte clock);
    bool begin();
    bool output_error();
    CanDeviceInfo get_device_info();

    /////////////////////////////////
    // communication function
    /////////////////////////////////
public:
    bool interrupt();
    bool send(CanData data);

    /////////////////////////////////
    // set callback
    /////////////////////////////////
public:
    bool set_callback_message(MessageFunction callback);
    bool set_callback_get_received(CAN::CanCommunicationGetReceivedFunction callback);

    /////////////////////////////////
    // Constructor
    /////////////////////////////////
public:
    DriverMcp2515();
    ~DriverMcp2515();

    /////////////////////////////////
    // private function
    /////////////////////////////////
private:
    void happened_message(bool is_error, const char *message);
    void happened_received(CanData data);
    bool setup_filter();

private:
    MessageFunction callback_message;
    CAN::CanCommunicationGetReceivedFunction callback_get_received;
    MCP_CAN *can;
    byte can_mode  = CAN_COMMUNICATION_MCP2515_MODE;
    byte can_speed = CAN_COMMUNICATION_MCP2515_SPEED;
    byte can_clock = CAN_COMMUNICATION_MCP2515_CLOCK;
    CanDeviceInfo device_info;
};
} // namespace CAN
} // namespace ToyBox
} // namespace MaSiRoProject
#endif
#endif