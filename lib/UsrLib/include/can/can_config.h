/**
 * @file can_config.h
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-09
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_CONFIG_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_CONFIG_HPP

#include "conf/pin_setting.h"

#if LIB_ESP32CAN
#include <CAN_config.h>
#else
#include <mcp_can_dfs.h>
#endif

namespace MaSiRoProject
{
namespace ToyBox
{
namespace CAN
{
/*
---------------------------
------ system define:
---------------------------
CAN_MESSAGE_SIZE
CAN_THREAD_INTERVAL
---------------------------
---------------------------
------ pin define:
---------------------------
CAN_COMMUNICATION_PIN_INTERRUPT
CAN_COMMUNICATION_MCP2515_PIN_CS
CAN_COMMUNICATION_MCP2515_PIN_CLK
CAN_COMMUNICATION_MCP2515_PIN_MISO
CAN_COMMUNICATION_MCP2515_PIN_MOSI
---------------------------
---------------------------
------ mcp_can define:
---------------------------
CAN_COMMUNICATION_MCP2515_MODE
CAN_COMMUNICATION_MCP2515_SPEED
CAN_COMMUNICATION_MCP2515_CLOCK
*/

#ifndef CAN_MESSAGE_SIZE
#define CAN_MESSAGE_SIZE (8)
#endif
#ifndef CAN_THREAD_INTERVAL
#define CAN_THREAD_INTERVAL (5)
#endif

#ifndef CAN_COMMUNICATION_PIN_INTERRUPT
#define CAN_COMMUNICATION_PIN_INTERRUPT USR_LIB_PIN_SIP4_2
#endif

#if LIB_ESP32CAN
////////////////////////////////////////////////////////////////////////////////////
#ifndef CAN_COMMUNICATION_ESP32CAN_SPEED
#define CAN_COMMUNICATION_ESP32CAN_SPEED CAN_SPEED_250KBPS
#endif
#ifndef CAN_COMMUNICATION_ESP32CAN_PIN_TX
#define CAN_COMMUNICATION_ESP32CAN_PIN_TX USR_LIB_PIN_GROVE_3
//#define CAN_COMMUNICATION_ESP32CAN_PIN_TX G26 /*DAC*/
#endif
#ifndef CAN_COMMUNICATION_ESP32CAN_PIN_RX
#define CAN_COMMUNICATION_ESP32CAN_PIN_RX USR_LIB_PIN_GROVE_4
// #define CAN_COMMUNICATION_ESP32CAN_PIN_RX G36 /*ADC*/
#endif
#ifndef CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE
#define CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE 10
#endif
////////////////////////////////////////////////////////////////////////////////////

#else
////////////////////////////////////////////////////////////////////////////////////
#ifndef CAN_COMMUNICATION_MCP2515_PIN_CS
#define CAN_COMMUNICATION_MCP2515_PIN_CS USR_LIB_PIN_SIP5_2
#endif
#ifndef CAN_COMMUNICATION_MCP2515_PIN_CLK
#define CAN_COMMUNICATION_MCP2515_PIN_CLK USR_LIB_PIN_HEAD_SPI_CLK
#endif
#ifndef CAN_COMMUNICATION_MCP2515_PIN_MISO
#define CAN_COMMUNICATION_MCP2515_PIN_MISO USR_LIB_PIN_HEAD_SPI_MISO
#endif
#ifndef CAN_COMMUNICATION_MCP2515_PIN_MOSI
#define CAN_COMMUNICATION_MCP2515_PIN_MOSI USR_LIB_PIN_HEAD_SPI_MOSI
#endif

#ifndef CAN_COMMUNICATION_MCP2515_MODE
#define CAN_COMMUNICATION_MCP2515_MODE MCP_STDEXT
#endif
#ifndef CAN_COMMUNICATION_MCP2515_SPEED
#define CAN_COMMUNICATION_MCP2515_SPEED CAN_250KBPS
#endif
#ifndef CAN_COMMUNICATION_MCP2515_CLOCK
#define CAN_COMMUNICATION_MCP2515_CLOCK MCP_8MHZ
#endif
////////////////////////////////////////////////////////////////////////////////////
#endif

} // namespace CAN
} // namespace ToyBox
} // namespace MaSiRoProject
#endif