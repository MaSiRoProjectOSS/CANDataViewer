/**
 * @file driver_mcp2515_config.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-22
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_MCP2515_CONFIG_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_MCP2515_CONFIG_HPP

#include <Arduino.h>
#include <mcp_can.h>

////////////////////////////////////////////////////////////////////////////////////
#ifndef CAN_COMMUNICATION_MCP2515_MODE
#define CAN_COMMUNICATION_MCP2515_MODE MCP_STDEXT
#endif
#ifndef CAN_COMMUNICATION_MCP2515_SPEED
#define CAN_COMMUNICATION_MCP2515_SPEED CAN_500KBPS
#endif
#ifndef CAN_COMMUNICATION_MCP2515_CLOCK
#define CAN_COMMUNICATION_MCP2515_CLOCK MCP_8MHZ
#endif

#ifndef CAN_COMMUNICATION_SPI_PIN_CS
#define CAN_COMMUNICATION_SPI_PIN_CS SS
#endif
#ifndef CAN_COMMUNICATION_SPI_PIN_CLK
#define CAN_COMMUNICATION_SPI_PIN_CLK SCK
#endif
#ifndef CAN_COMMUNICATION_SPI_PIN_MISO
#define CAN_COMMUNICATION_SPI_PIN_MISO MISO
#endif
#ifndef CAN_COMMUNICATION_SPI_PIN_MOSI
#define CAN_COMMUNICATION_SPI_PIN_MOSI MOSI
#endif
////////////////////////////////////////////////////////////////////////////////////

#endif
