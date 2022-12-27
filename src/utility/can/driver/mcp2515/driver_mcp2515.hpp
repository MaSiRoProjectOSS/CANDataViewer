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

#include "../driver_can_abstract.hpp"
#include "driver_mcp2515_config.hpp"

namespace MaSiRoProject
{
namespace CAN
{
class DriverMcp2515 : public DriverCanAbstract {
public:
    /////////////////////////////////
    // setup function
    /////////////////////////////////
    bool begin() override;
    /////////////////////////////////
    // communication function
    /////////////////////////////////
    bool send(CanData data) override;
    bool interrupt() override;
    /////////////////////////////////
    // information function
    /////////////////////////////////
    bool output_error() override;

public:
    /////////////////////////////////
    // Constructor
    /////////////////////////////////
    DriverMcp2515(int cs);
    ~DriverMcp2515();

private:
    /////////////////////////////////
    // private function
    /////////////////////////////////
    bool setup_can(byte mode, byte speed, byte clock);
    bool setup_filter();

private:
    MCP_CAN *mcp2515;
    byte can_mode  = CAN_COMMUNICATION_MCP2515_MODE;
    byte can_speed = CAN_COMMUNICATION_MCP2515_SPEED;
    byte can_clock = CAN_COMMUNICATION_MCP2515_CLOCK;
};
} // namespace CAN
} // namespace MaSiRoProject
#endif
