/**
 * @file driver_can_abstract.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2
 * @date 2022-12-22
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_DRIVER_CAN_ABSTRACT_HPP
#define MASIRO_PROJECT_TOY_BOX_DRIVER_CAN_ABSTRACT_HPP

#include "../../../can_data_viewer_info.hpp"

namespace MaSiRoProject
{
namespace CAN
{
class DriverCanAbstract {
public:
    /////////////////////////////////
    // Constructor
    /////////////////////////////////
    DriverCanAbstract() {}

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
    CanDeviceInfo get_device_info() { return this->device_info; }
    bool set_callback_message(MessageFunction callback)
    {
        bool result = false;
        try {
            this->callback_message = callback;
            result                 = true;
        } catch (...) {
        }
        return result;
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
    void happened_message(OUTPUT_LOG_LEVEL level, const char *message, const char *function_name, const char *file_name, int line)
    {
        if (nullptr != this->callback_message) {
            this->callback_message(level, message, function_name, file_name, line);
        }
    }

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
    MessageFunction callback_message;
    GetReceivedFunction callback_get_received;
    CanDeviceInfo device_info;
};

} // namespace CAN
} // namespace MaSiRoProject

#endif
