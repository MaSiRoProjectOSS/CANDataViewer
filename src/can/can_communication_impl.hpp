/**
 * @file can_communication_impl.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CAN_COMMUNICATION_IMPL_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_COMMUNICATION_IMPL_HPP

#include "can_data_viewer_info.hpp"
#include "driver/driver_can_abstract.hpp"

#include <SPI.h>
#include <vector>

namespace MaSiRoProject
{
namespace CAN
{
class CanCommunicationImpl {
    friend class CanCommunication;

    /////////////////////////////////
    // Constructor
    /////////////////////////////////
public:
    CanCommunicationImpl();
    ~CanCommunicationImpl();

    /////////////////////////////////
    // Setup
    /////////////////////////////////
public:
    bool begin();
    bool loop();

    CAN_CTRL_STATE get_mode();

    /////////////////////////////////
    // Callback
    /////////////////////////////////
public:
    bool set_callback_message(MessageFunction callback);
    bool set_callback_changed_mode(ChangedModeFunction callback);
    bool set_callback_received(GetReceivedFunction callback);
    bool set_callback_sendable(SendEventFunction callback);

private:
    MessageFunction callback_message;
    ChangedModeFunction callback_changed_mode;
    GetReceivedFunction callback_received;
    SendEventFunction callback_sendable;

    /////////////////////////////////
    // Request
    /////////////////////////////////
public:
    void request_start();
    void request_run();
    void request_resume();
    void request_suspend();
    void request_stop();
    void request_pause();

private:
    void request_sleep();

    /////////////////////////////////
    // Receive
    /////////////////////////////////
public:
    bool interrupt();

    /////////////////////////////////
    // Send
    /////////////////////////////////
public:
    bool data_sendable(CAN_CTRL_STATE state);

public:
    bool add_one_shot(CanData data);
    bool add_loop_shot(CanData data, int interval);
    bool clear_loop_shot();
    bool clear_resume();
    bool delete_loop_shot(unsigned long Id);
    bool delete_resume(unsigned long Id);
    std::vector<CanData> get_loop_shot();
    std::vector<CanData> get_received();
    std::vector<CanData> get_resume();
    CanDeviceInfo get_device_info();

private:
    bool send(CanData data);
    bool send_one_shot();
    bool send_loop_shot();
    std::vector<CanData> send_one_shot_list;
    std::vector<CanData> send_loop_list;
    std::vector<CanData> received_list;
    std::vector<CanData> send_resume;
    bool add_resume(CanData data);

    /////////////////////////////////
    // Happened
    /////////////////////////////////
private:
    void happened_changed_mode(CAN_CTRL_STATE mode);
    void happened_message(bool is_error, const char *message);
    void happened_received(CanData data);

    /////////////////////////////////
    // member
    /////////////////////////////////
private:
    DriverCanAbstract *can;
    CanDeviceInfo device_info;

    CAN_CTRL_STATE mode_current;
    CAN_CTRL_STATE mode_request;
    bool flag_request_pause = false;

    bool initialized;
};
} // namespace CAN
} // namespace MaSiRoProject
#endif