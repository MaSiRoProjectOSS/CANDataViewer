/**
 * @file driver_mcp2515.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-09
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "driver_mcp2515.hpp"

namespace MaSiRoProject
{
namespace CAN
{
/////////////////////////////////
// setup function
/////////////////////////////////
bool DriverMcp2515::begin()
{
    bool result = false;
    //////////////////////////////////
    SPI.end();
    SPI.begin(CAN_COMMUNICATION_SPI_PIN_CLK, CAN_COMMUNICATION_SPI_PIN_MISO, CAN_COMMUNICATION_SPI_PIN_MOSI, CAN_COMMUNICATION_SPI_PIN_CS);
    if (CAN_OK == this->mcp2515->begin(this->can_mode, this->can_speed, this->can_clock)) {
        this->mcp2515->setMode(MCP_NORMAL);
        this->setup_filter();
        this->interrupt();
        result = true;
    }
    //////////////////////////////////
    return result;
}

/////////////////////////////////
// communication function
/////////////////////////////////
bool DriverMcp2515::send(CanData data)
{
    bool result = true;
    byte status = this->mcp2515->sendMsgBuf(data.Id, data.Length, data.Data);
    if (CAN_OK != status) {
        result = false;
    }
#if DEBUG_MODE
    char buffer[255];
    sprintf(buffer, "DriverMcp2515 : send state[%d]", status);
    happened_message(true, buffer);
#endif
    return result;
}

bool DriverMcp2515::interrupt()
{
    bool result = false;
    char buffer[255];
    try {
        while (CAN_MSGAVAIL == this->mcp2515->checkReceive()) {
            CanData data;
            if (CAN_OK == this->mcp2515->readMsgBuf(&data.Id, &data.Length, data.Data)) {
                result = true;
                happened_received(data);
            } else {
                sprintf(buffer, "Receive failed");
                happened_message(true, buffer);
            }
        }
#if DEBUG_MODE
        if (false == result) {
            sprintf(buffer, "NO MESSAGE");
            happened_message(false, buffer);
        }
#endif
    } catch (...) {
        sprintf(buffer, "Receive panic");
        happened_message(true, buffer);
    }
    return result;
}

/////////////////////////////////
// information function
/////////////////////////////////
bool DriverMcp2515::output_error()
{
    bool result = false;
    //////////////////////////////////
    char buffer[255];
    byte error_checkError   = this->mcp2515->checkError();
    byte error_getError     = this->mcp2515->getError();
    byte error_errorCountTX = this->mcp2515->errorCountTX();
    byte error_errorCountRX = this->mcp2515->errorCountRX();
    byte check_receive      = this->mcp2515->checkReceive();
    if (0 != (error_checkError)) {
        //////////////////////////////////
        sprintf(buffer,
                "CanCommunication : "
                "ERROR[%d] tx[%d] / rx[%d]" // (2)(6)(7)
                "%s",                       // --
                //
                (error_checkError == CAN_OK) ? 0 : error_getError, // (2)
                error_errorCountTX,                                // (6)
                error_errorCountRX,                                // (7)
                (check_receive == CAN_NOMSG) ? "" : " : AVAIL"     // --
        );
        happened_message(true, buffer);
        result = true;
    }
    //////////////////////////////////
    return result;
}

/////////////////////////////////
// Constructor
/////////////////////////////////
DriverMcp2515::DriverMcp2515() : DriverCanAbstract(), mcp2515(new MCP_CAN(CAN_COMMUNICATION_SPI_PIN_CS))
{
    this->setup_can(this->can_mode, this->can_speed, this->can_clock);
}
DriverMcp2515::~DriverMcp2515()
{
}

/////////////////////////////////
// private function
/////////////////////////////////
bool DriverMcp2515::setup_can(byte mode, byte speed, byte clock)
{
    bool result = true;
    ///////////////////////////
    this->can_mode  = mode;
    this->can_speed = speed;
    this->can_clock = clock;
    ///////////////////////////
    switch (this->can_mode) {
        case MCP_STDEXT:
            this->device_info.can_type = 2;
            break;
        case MCP_STD:
            this->device_info.can_type = 0;
            break;
        case MCP_EXT:
            this->device_info.can_type = 1;
            break;
        case MCP_ANY:
        default:
            this->device_info.can_type = 3;
            break;
    }
    this->device_info.can_type  = this->can_mode;
    this->device_info.can_speed = this->can_speed;
    switch (this->can_speed) {
        case CAN_4K096BPS:
            sprintf(this->device_info.can_speed_txt, "4096");
            break;
        case CAN_5KBPS:
            sprintf(this->device_info.can_speed_txt, "5K");
            break;
        case CAN_10KBPS:
            sprintf(this->device_info.can_speed_txt, "10K");
            break;
        case CAN_20KBPS:
            sprintf(this->device_info.can_speed_txt, "20K");
            break;
        case CAN_31K25BPS:
            sprintf(this->device_info.can_speed_txt, "31K25");
            break;
        case CAN_33K3BPS:
            sprintf(this->device_info.can_speed_txt, "33K3");
            break;
        case CAN_40KBPS:
            sprintf(this->device_info.can_speed_txt, "40K");
            break;
        case CAN_50KBPS:
            sprintf(this->device_info.can_speed_txt, "50K");
            break;
        case CAN_80KBPS:
            sprintf(this->device_info.can_speed_txt, "80K");
            break;
        case CAN_100KBPS:
            sprintf(this->device_info.can_speed_txt, "100K");
            break;
        case CAN_125KBPS:
            sprintf(this->device_info.can_speed_txt, "125K");
            break;
        case CAN_200KBPS:
            sprintf(this->device_info.can_speed_txt, "200K");
            break;
        case CAN_250KBPS:
            sprintf(this->device_info.can_speed_txt, "250K");
            break;
        case CAN_500KBPS:
            sprintf(this->device_info.can_speed_txt, "500K");
            break;
        case CAN_1000KBPS:
            sprintf(this->device_info.can_speed_txt, "1000K");
            break;
        default:
            sprintf(this->device_info.can_speed_txt, "unknow");
            break;
    }

    return result;
}

bool DriverMcp2515::setup_filter()
{
    bool result = true;
#if 0
    this->mcp2515->init_Mask(0, 0x52);
    this->mcp2515->init_Mask(1, 0x50);
    this->mcp2515->init_Filt(0, 0x50);
    this->mcp2515->init_Filt(1, 0x51);
#endif
#if 0
#define CAN_ID_PID 0x7DF
    /*
        set mask, set both the mask to 0x3ff
    */
    this->mcp2515->init_Mask(0, 0, 0x7FC);
    this->mcp2515->init_Mask(1, 0, 0x7FC);

    /*
        set filter, we can receive id from 0x04 ~ 0x09
    */
    this->mcp2515->init_Filt(0, 0, 0x7E8);
    this->mcp2515->init_Filt(1, 0, 0x7E8);

    this->mcp2515->init_Filt(2, 0, 0x7E8);
    this->mcp2515->init_Filt(3, 0, 0x7E8);
    this->mcp2515->init_Filt(4, 0, 0x7E8);
    this->mcp2515->init_Filt(5, 0, 0x7E8);
#endif
    return result;
}

} // namespace CAN
} // namespace MaSiRoProject
