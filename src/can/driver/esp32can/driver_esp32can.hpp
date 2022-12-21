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

#include "driver_esp32can_config.hpp"

namespace MaSiRoProject
{
namespace CAN
{
class DriverEsp32can {
    /////////////////////////////////
    // setup function
    /////////////////////////////////
public:
    bool setup_can(CAN_speed_t speed);
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
    bool set_callback_get_received(GetReceivedFunction callback);

    /////////////////////////////////
    // Constructor
    /////////////////////////////////
public:
    DriverEsp32can();
    ~DriverEsp32can();

    /////////////////////////////////
    // private function
    /////////////////////////////////
private:
    void happened_message(bool is_error, const char *message);
    void happened_received(CanData data);
    bool setup_filter();

private:
    MessageFunction callback_message;
    GetReceivedFunction callback_get_received;
    CAN_speed_t can_speed   = CAN_COMMUNICATION_ESP32CAN_SPEED;
    const int rx_queue_size = CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE;
    CanDeviceInfo device_info;
};
} // namespace CAN
} // namespace MaSiRoProject
#endif
