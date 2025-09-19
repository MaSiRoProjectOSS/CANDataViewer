/**
 * @file can_config.h
 * @brief CAN通信の設定を行うヘッダファイルです。
 * @version 0.1
 * @date 2022-12-09
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 * このファイルはCAN通信の割り込みピンなどの設定を定義します。
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_CONFIG_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_CONFIG_HPP

#include <Arduino.h>

#ifndef CAN_COMMUNICATION_PIN_INTERRUPT
#define CAN_COMMUNICATION_PIN_INTERRUPT G25
#endif

#endif
