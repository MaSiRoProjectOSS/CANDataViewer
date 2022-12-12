/**
 * @file pin_setting.h
 * @author akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-10-26
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef USR_LIB_PIN_SETTING_H
#define USR_LIB_PIN_SETTING_H
#include <Arduino.h>

#define USR_LIB_PIN_INNER_LED    G27
#define USR_LIB_PIN_INNER_BUTTON G39
#define USR_LIB_PIN_INNER_IR     G12
#define USR_LIB_PIN_INNER_SCL    G21
#define USR_LIB_PIN_INNER_SDA    G25

#define USR_LIB_PIN_HEAD_SPI_MISO G19
#define USR_LIB_PIN_HEAD_SPI_MOSI G23
#define USR_LIB_PIN_HEAD_SPI_CLK  G33

#define USR_LIB_PIN_HEAD_ADC  G33
#define USR_LIB_PIN_GROVE_ADC G32

#define USR_LIB_PIN_HEAD_DAC  G25
#define USR_LIB_PIN_GROVE_DAC G26

#define USR_LIB_PIN_HEAD_SDA  G25
#define USR_LIB_PIN_HEAD_SCL  G21
#define USR_LIB_PIN_GROVE_SDA G26
#define USR_LIB_PIN_GROVE_SCL G32

//#define USR_LIB_PIN_SIP5_1 3V3
#define USR_LIB_PIN_SIP5_2 G22
#define USR_LIB_PIN_SIP5_3 G19
#define USR_LIB_PIN_SIP5_4 G23
#define USR_LIB_PIN_SIP5_5 G33

#define USR_LIB_PIN_SIP4_1 G21
#define USR_LIB_PIN_SIP4_2 G25
//#define USR_LIB_PIN_SIP4_3 5V
#define USR_LIB_PIN_SIP4_4 GND

#define USR_LIB_PIN_GROVE_1 GND
//#define USR_LIB_PIN_GROVE_2 5V5
#define USR_LIB_PIN_GROVE_3 G26
#define USR_LIB_PIN_GROVE_4 G32

///////////////////////////////////////////////////////////////////
const BaseType_t CORE_NUM_00 = 0;
const BaseType_t CORE_NUM_01 = 1;

#endif