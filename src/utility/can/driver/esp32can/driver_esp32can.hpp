/**
 * @file driver_esp32can.hpp
 * @brief ESP32用CANドライバのヘッダファイル
 * @version 0.1
 * @date 2022-12-09
 *
 * @details
 * このファイルは、MaSiRo Projectで使用するESP32向けCAN通信ドライバのクラス定義を提供します。
 * CAN通信の初期化、送信、割り込み処理、エラー出力などの機能を持つDriverEsp32canクラスを定義しています。
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_ESP32CAN_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_DRIVER_ESP32CAN_HPP
#if LIB_CAN_DRIVER != 1

#include "../driver_can_abstract.hpp"
#include "driver_esp32can_config.hpp"

namespace MaSiRoProject
{
namespace CAN
{
class DriverEsp32can : public DriverCanAbstract {
public:
    /////////////////////////////////
    // setup function
    /////////////////////////////////
    bool begin() override;
    /////////////////////////////////
    // communication function
    /////////////////////////////////
    bool send(CanData data) override;
    bool interrupt() override;
    /////////////////////////////////
    // information function
    /////////////////////////////////
    bool output_error() override;

public:
    /////////////////////////////////
    // Constructor
    /////////////////////////////////
    DriverEsp32can(const uint8_t pin_rx, const uint8_t pin_tx);
    ~DriverEsp32can();

public:
    /////////////////////////////////
    // private function
    /////////////////////////////////
private:
    bool setup_can(CAN_speed_t speed);
    bool setup_filter();

private:
    CAN_speed_t can_speed   = CAN_COMMUNICATION_ESP32CAN_SPEED;
    const int rx_queue_size = CAN_COMMUNICATION_ESP32CAN_QUEUE_SIZE;
    uint8_t _pin_rx;
    uint8_t _pin_tx;
    bool _initialized = false;
};
} // namespace CAN
} // namespace MaSiRoProject

#endif
#endif
