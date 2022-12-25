/**
 * @file driver_esp32can.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-09
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "driver_esp32can.hpp"

CAN_device_t CAN_cfg;

namespace MaSiRoProject
{
namespace CAN
{
/////////////////////////////////
// setup function
/////////////////////////////////
bool DriverEsp32can::begin()
{
    bool result = false;
    //////////////////////////////////
    CAN_cfg.speed     = this->can_speed;
    CAN_cfg.tx_pin_id = (gpio_num_t)CAN_COMMUNICATION_GROVE_PIN_TX;
    CAN_cfg.rx_pin_id = (gpio_num_t)CAN_COMMUNICATION_GROVE_PIN_RX;
    CAN_cfg.rx_queue  = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
    ESP32Can.CANInit();
    //////////////////////////////////
    return result;
}
/////////////////////////////////
// communication function
/////////////////////////////////
bool DriverEsp32can::send(CanData data)
{
    bool result = true;

    CAN_frame_t tx_frame;
    if (false == data.ExtFlag) {
        tx_frame.FIR.B.FF = CAN_frame_std;
    } else {
        tx_frame.FIR.B.FF = CAN_frame_ext;
    }
    tx_frame.MsgID      = data.Id;
    tx_frame.FIR.B.DLC  = data.Length;
    tx_frame.data.u8[0] = data.Data[0];
    tx_frame.data.u8[1] = data.Data[1];
    tx_frame.data.u8[2] = data.Data[2];
    tx_frame.data.u8[3] = data.Data[3];
    tx_frame.data.u8[4] = data.Data[4];
    tx_frame.data.u8[5] = data.Data[5];
    tx_frame.data.u8[6] = data.Data[6];
    tx_frame.data.u8[7] = data.Data[7];

    if (0 != ESP32Can.CANWriteFrame(&tx_frame)) {
        result = false;
    }
    return result;
}

bool DriverEsp32can::interrupt()
{
    bool result = false;
    char buffer[255];
    try {
        CAN_frame_t rx_frame;
        while (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
            CanData data;
            data.ExtFlag = (rx_frame.FIR.B.FF == CAN_frame_std) ? 0 : 1;
            data.Id      = rx_frame.MsgID;
            data.Length  = rx_frame.FIR.B.DLC;
            if (rx_frame.FIR.B.RTR != CAN_RTR) {
                for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
                    data.Data[i] = rx_frame.data.u8[i];
                }
                result = true;
                happened_received(data);
            }
        }
#if DEBUG_MODE
        if (false == result) {
            sprintf(buffer, "NO MESSAGE");
            happened_message(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_TRACE, buffer, __func__, __FILENAME__, __LINE__);
        }
#endif
    } catch (...) {
        sprintf(buffer, "Receive panic");
        happened_message(OUTPUT_LOG_LEVEL::OUTPUT_LOG_LEVEL_FATAL, buffer, __func__, __FILENAME__, __LINE__);
    }
    return result;
}
/////////////////////////////////
// information function
/////////////////////////////////
bool DriverEsp32can::output_error()
{
    bool result = false;
    //////////////////////////////////
    //////////////////////////////////
    return result;
}
/////////////////////////////////
// Constructor
/////////////////////////////////
DriverEsp32can::DriverEsp32can() : DriverCanAbstract()
{
    this->setup_can(this->can_speed);
}
DriverEsp32can::~DriverEsp32can()
{
    int CANStop();
}

/////////////////////////////////
// private function
/////////////////////////////////
bool DriverEsp32can::setup_can(CAN_speed_t speed)
{
    bool result = true;
    ///////////////////////////
    this->can_speed = speed;
    ///////////////////////////
    this->device_info.can_type  = 2; // std/ext
    this->device_info.can_speed = this->can_speed;
    switch (this->can_speed) {
        case CAN_SPEED_100KBPS:
            sprintf(this->device_info.can_speed_txt, "100K");
            break;
        case CAN_SPEED_125KBPS:
            sprintf(this->device_info.can_speed_txt, "125K");
            break;
        case CAN_SPEED_200KBPS:
            sprintf(this->device_info.can_speed_txt, "200K");
            break;
        case CAN_SPEED_250KBPS:
            sprintf(this->device_info.can_speed_txt, "250K");
            break;
        case CAN_SPEED_500KBPS:
            sprintf(this->device_info.can_speed_txt, "500K");
            break;
        case CAN_SPEED_800KBPS:
            sprintf(this->device_info.can_speed_txt, "800K");
            break;
        case CAN_SPEED_1000KBPS:
            sprintf(this->device_info.can_speed_txt, "1000K");
            break;
        default:
            sprintf(this->device_info.can_speed_txt, "unknow");
            break;
    }
    return result;
}

bool DriverEsp32can::setup_filter()
{
    bool result = true;
#if 0
    CAN_filter_t p_filter;
    if(0!= CANConfigFilter(&p_filter)){
        result = false;
    }
#endif
    return result;
}

} // namespace CAN
} // namespace MaSiRoProject
