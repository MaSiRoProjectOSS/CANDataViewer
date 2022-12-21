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

#ifndef LIB_ESP32CAN
#define LIB_ESP32CAN 0
#endif

#include "can_data_viewer_info.hpp"

#if LIB_ESP32CAN
#include <CAN_config.h>
#else
#include <mcp_can_dfs.h>
#endif

namespace MaSiRoProject
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
CAN_COMMUNICATION_SPI_PIN_CS
CAN_COMMUNICATION_SPI_PIN_CLK
CAN_COMMUNICATION_SPI_PIN_MISO
CAN_COMMUNICATION_SPI_PIN_MOSI
---------------------------
---------------------------
------ mcp_can define:
---------------------------
CAN_COMMUNICATION_MCP2515_MODE
CAN_COMMUNICATION_MCP2515_SPEED
CAN_COMMUNICATION_MCP2515_CLOCK
*/

#ifndef CAN_COMMUNICATION_PIN_INTERRUPT
#define CAN_COMMUNICATION_PIN_INTERRUPT G25
#endif

#if LIB_ESP32CAN
////////////////////////////////////////////////////////////////////////////////////
#ifndef CAN_COMMUNICATION_GROVE_PIN_TX
#define CAN_COMMUNICATION_GROVE_PIN_TX G26 /*DAC*/
#endif
#ifndef CAN_COMMUNICATION_GROVE_PIN_RX
#define CAN_COMMUNICATION_GROVE_PIN_RX G36 /*ADC*/
#endif
#ifndef CAN_COMMUNICATION_ESP32CAN_SPEED
#define CAN_COMMUNICATION_ESP32CAN_SPEED CAN_SPEED_250KBPS
#endif
#ifndef CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE
#define CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE 10
#endif
////////////////////////////////////////////////////////////////////////////////////

#else
////////////////////////////////////////////////////////////////////////////////////

#ifndef CAN_COMMUNICATION_SPI_PIN_CS
#define CAN_COMMUNICATION_SPI_PIN_CS G22
#endif
#ifndef CAN_COMMUNICATION_SPI_PIN_CLK
#define CAN_COMMUNICATION_SPI_PIN_CLK G33
#endif
#ifndef CAN_COMMUNICATION_SPI_PIN_MISO
#define CAN_COMMUNICATION_SPI_PIN_MISO G19
#endif
#ifndef CAN_COMMUNICATION_SPI_PIN_MOSI
#define CAN_COMMUNICATION_SPI_PIN_MOSI G23
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
} // namespace MaSiRoProject
#endif