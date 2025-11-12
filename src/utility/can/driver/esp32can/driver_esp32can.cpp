/**
 * @file driver_esp32can.cpp
 * @brief ESP32用CANドライバの実装ファイル
 * @version 0.1
 * @date 2022-12-09
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 * このファイルは、ESP32マイコンでCAN通信を行うためのドライバクラスの実装です。
 * CAN通信の初期化、送信、受信割り込み処理、エラー出力、フィルタ設定などの機能を提供します。
 * CAN名前空間内でDriverEsp32canクラスとして定義されています。
 */
#if LIB_CAN_DRIVER != 1
#include "driver_esp32can.hpp"

namespace CAN
{
CAN_device_t CAN_cfg;

/////////////////////////////////
// setup function
/////////////////////////////////
bool DriverEsp32can::begin()
{
    bool result = false;
    if (0 <= this->_pin_tx && 0 <= this->_pin_rx) {
        //////////////////////////////////
        CAN_cfg.speed     = this->can_speed;
        CAN_cfg.tx_pin_id = (gpio_num_t)this->_pin_tx;
        CAN_cfg.rx_pin_id = (gpio_num_t)this->_pin_rx;
        CAN_cfg.rx_queue  = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
        if (0 == ESP32Can.CANInit()) {
            result = true;
        }
    }
    //////////////////////////////////
    this->_initialized = result;
    return result;
}
/////////////////////////////////
// communication function
/////////////////////////////////
bool DriverEsp32can::send(CanData data)
{
    bool result = true;
    if (true == this->_initialized) {
#if DEBUG_MODE
        log_i("SEND   "
              " : id = 0x%02lX / %02d / "
              " : data = 0x%02lX 0x%02lX 0x%02lX 0x%02lX 0x%02lX 0x%02lX 0x%02lX 0x%02lX",
              data.Id,
              data.Length,
              data.Data[0],
              data.Data[1],
              data.Data[2],
              data.Data[3],
              data.Data[4],
              data.Data[5],
              data.Data[6],
              data.Data[7]);
#endif
        CAN_frame_t tx_frame;
        if (false == data.ExtFlag) {
            tx_frame.FIR.B.FF = CAN_frame_std;
        } else {
            tx_frame.FIR.B.FF = CAN_frame_ext;
        }
        tx_frame.MsgID     = data.Id;
        tx_frame.FIR.B.DLC = data.Length;
        for (int i = 0; i < 8; i++) {
            tx_frame.data.u8[i] = data.Data[i];
        }

        log_v("DriverEsp32can : send Id=0x%lX / len=%d", tx_frame.MsgID, tx_frame.FIR.B.DLC);
        if (0 != ESP32Can.CANWriteFrame(&tx_frame)) {
            result = false;
        }
        log_v("DriverEsp32can : send state[%d]", (int)result);
    } else {
        log_v("DriverEsp32can : Not initialized");
    }
    return result;
}

bool DriverEsp32can::interrupt()
{
    bool result = false;
    if (true == this->_initialized) {
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
                log_v("NO MESSAGE");
            }
#endif
        } catch (...) {
            log_e("Receive panic");
        }
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
DriverEsp32can::DriverEsp32can(const uint8_t pin_rx, const uint8_t pin_tx) : DriverCanAbstract()
{
    this->_pin_rx = pin_rx;
    this->_pin_tx = pin_tx;
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

#endif
