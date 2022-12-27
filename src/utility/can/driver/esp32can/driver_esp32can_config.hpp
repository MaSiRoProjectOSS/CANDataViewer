/**
 * @file driver_esp32can_config.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-22
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_ESP32CAN_CONFIG_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_ESP32CAN_CONFIG_HPP

#include <Arduino.h>
#include <ESP32CAN.h>

////////////////////////////////////////////////////////////////////////////////////
#ifndef CAN_COMMUNICATION_ESP32CAN_SPEED
#define CAN_COMMUNICATION_ESP32CAN_SPEED CAN_SPEED_250KBPS
#endif
#ifndef CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE
#define CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE 10
#endif

#ifndef CAN_COMMUNICATION_GROVE_PIN_SDA
#define CAN_COMMUNICATION_GROVE_PIN_SDA DAC2
#endif
#ifndef CAN_COMMUNICATION_GROVE_PIN_SCL
#define CAN_COMMUNICATION_GROVE_PIN_SCL ADC2
#endif
////////////////////////////////////////////////////////////////////////////////////

#endif