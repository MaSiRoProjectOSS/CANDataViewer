/**
 * @file driver_can_abstract.hpp
 * @brief CANドライバーの抽象クラスを定義するヘッダファイルです。
 *        このクラスはCAN通信の基本的なインターフェース（初期化、送信、割り込み、エラー出力など）を提供します。
 *        また、受信データのコールバック設定やデバイス情報の取得機能も備えています。
 * @version 0.2
 * @date 2022-12-22
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 */
#ifndef MASIRO_PROJECT_TOY_BOX_DRIVER_CAN_ABSTRACT_HPP
#define MASIRO_PROJECT_TOY_BOX_DRIVER_CAN_ABSTRACT_HPP

#include <can_data_viewer_info.hpp>

namespace CAN
{
class DriverCanAbstract {
public:
    /////////////////////////////////
    // Constructor
    /////////////////////////////////
    DriverCanAbstract()
    {
    }

    /////////////////////////////////
    // setup function
    /////////////////////////////////
    virtual bool begin() = 0;
    /////////////////////////////////
    // communication function
    /////////////////////////////////
    virtual bool send(CanData data) = 0;
    virtual bool interrupt()        = 0;
    /////////////////////////////////
    // information function
    /////////////////////////////////
    virtual bool output_error() = 0;

public:
    /////////////////////////////////
    // common function
    /////////////////////////////////
    CanDeviceInfo get_device_info()
    {
        return this->device_info;
    }

    bool set_callback_get_received(GetReceivedFunction callback)
    {
        bool result = false;
        try {
            this->callback_get_received = callback;
            result                      = true;
        } catch (...) {
        }
        return result;
    }

protected:
    /////////////////////////////////
    // inner function
    /////////////////////////////////
    void happened_received(CanData data)
    {
        if (nullptr != this->callback_get_received) {
            CanData dest;
            dest.Id     = data.Id;
            dest.Length = data.Length;
            for (int i = 0; i < data.Length; i++) {
                dest.Data[i] = data.Data[i];
            }
            this->callback_get_received(dest);
        }
    }

protected:
    GetReceivedFunction callback_get_received;
    CanDeviceInfo device_info;
};

} // namespace CAN

#endif
