/**
 * @file driver_esp32can_config.hpp
 * @brief ESP32用CANドライバの設定ヘッダファイル
 * @version 0.1
 * @date 2022-12-22
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 * このファイルは、ESP32のCAN通信に必要な設定（通信速度、キューサイズ、TX/RXピンなど）を定義します。
 * LIB_CAN_DRIVERが1でない場合に有効となり、ArduinoおよびESP32CANライブラリを利用します。
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_ESP32CAN_CONFIG_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_ESP32CAN_CONFIG_HPP
#if LIB_CAN_DRIVER != 1

#include <Arduino.h>
#include <ESP32CAN.h>

////////////////////////////////////////////////////////////////////////////////////
#ifndef CAN_COMMUNICATION_ESP32CAN_SPEED
#define CAN_COMMUNICATION_ESP32CAN_SPEED CAN_SPEED_1000KBPS
#endif
#ifndef CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE
#define CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE 20
#endif

#ifndef CAN_COMMUNICATION_PIN_ESP32CAN_TX
#define CAN_COMMUNICATION_PIN_ESP32CAN_TX DAC2
#endif
#ifndef CAN_COMMUNICATION_PIN_ESP32CAN_RX
#define CAN_COMMUNICATION_PIN_ESP32CAN_RX ADC2
#endif
////////////////////////////////////////////////////////////////////////////////////

#endif
#endif
