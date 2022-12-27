/**
 * @file driver_esp32can.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-09
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_ESP32CAN_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_ESP32CAN_HPP

#include "../driver_can_abstract.hpp"
#include "driver_esp32can_config.hpp"

namespace MaSiRoProject
{
namespace CAN
{
class DriverEsp32can : public DriverCanAbstract {
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
    DriverEsp32can();
    ~DriverEsp32can();

public:
    /////////////////////////////////
    // private function
    /////////////////////////////////
private:
    bool setup_can(CAN_speed_t speed);
    bool setup_filter();

private:
    CAN_speed_t can_speed   = CAN_COMMUNICATION_ESP32CAN_SPEED;
    const int rx_queue_size = CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE;
};
} // namespace CAN
} // namespace MaSiRoProject
#endif
