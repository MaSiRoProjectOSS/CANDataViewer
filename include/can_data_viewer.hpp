/**
 * @file can_data_viewer.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-20
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_CAN_DATA_VIEWER_HPP
#define MASIRO_PROJECT_CAN_DATA_VIEWER_HPP

#include <can_data_viewer_info.hpp>
#include <cushy_web_server.hpp>

class CanDataViewer {
    ////////////////////////////////////////////////
    // standard function
    ////////////////////////////////////////////////
public:
    /**
     * @brief Construct a new Can Data Viewer object
     *
     */
    CanDataViewer(const uint8_t interrupt = 0, const uint8_t cs = 0);
    /**
     * @brief Destroy the Can Data Viewer object
     *
     */
    ~CanDataViewer();
    /**
     * @brief
     *
     * @param ssid      Pointer to the SSID string.
     * @param pass      Passphrase. Valid characters in a passphrase must be between ASCII 32-126 (decimal).
     * @param ap_mode   true: Access Point mode / false: Station mode
     * @return true     Successfully
     * @return false    Failed
     */
    bool begin();

    ////////////////////////////////////////////////
    // setup function
    ////////////////////////////////////////////////
public:
    /**
     * @brief Set the callback changed mode function
     *
     * @param callback  Callback function to be called
     * @return true     Successfully
     * @return false    Failed
     */
    bool set_callback_changed_mode(ChangedModeFunction callback);
    /**
     * @brief Set the callback received function
     *
     * @param callback  Callback function to be called
     * @return true     Successfully
     * @return false    Failed
     */
    bool set_callback_received(GetReceivedFunction callback);
    /**
     * @brief Set the callback sendable interrupt function
     *
     * @param callback  Callback function to be called
     * @return true     Successfully
     * @return false    Failed
     */
    bool set_callback_sendable_interrupt(SendEventFunction callback);
    /**
     * @brief Set the callback setting default function
     *
     * @param callback  Callback function to be called
     * @return true     Successfully
     * @return false    Failed
     */
    bool set_callback_setting_default(SettingDefaultFunction callback);

    ////////////////////////////////////////////////
    // control function
    ////////////////////////////////////////////////
public:
    /**
     * @brief Set the mode object
     *
     * @param mode      Specify the mode you want to change. On/Off is switched when unspecified.
     * @return true     Successfully
     * @return false    Failed
     */
    bool set_mode(CAN_CTRL_STATE mode = CAN_CTRL_STATE::MODE_UNKNOW);
    /**
     * @brief Clears the list of CanData displayed in the browser.
     *
     * @return true     Successfully
     * @return false    Failed
     */
    bool clear_resume(void);
    /**
     * @brief Clears CAN data that is sent at regular intervals.
     *
     * @return true     Successfully
     * @return false    Failed
     */
    bool clear_loop_shot(void);
    /**
     * @brief Add CAN data to be sent only once.
     *
     * @param data          CanData to be set
     * @return true     Successfully
     * @return false    Failed
     */
    bool add_one_shot(CanData data);
    /**
     * @brief Add CAN data to be sent at regular intervals.
     *
     * @param data          CanData to be set
     * @param interval      regular interval
     * @return true     Successfully
     * @return false    Failed
     */
    bool add_loop_shot(CanData data, int interval);
    /**
     * @brief Add to the list displayed on the browser.
     *
     * @param data
     * @return true     Successfully
     * @return false    Failed
     */
    bool add_resume(CanData data);

    ////////////////////////////////////////////////
    // debug function
    ////////////////////////////////////////////////
#if DEBUG_MODE
public:
    /**
     * @brief Get the stack high water mark on can thread
     *
     * @return UBaseType_t  The smallest amount of free stack space
     */
    UBaseType_t get_stack_high_water_mark_can();
    /**
     * @brief Get the stack size on can thread
     *
     * @return UBaseType_t  The size of the task stack specified as the number of bytes.
     */
    UBaseType_t get_stack_size_can();

    /**
     * @brief Get the stack high water mark on server thread
     *
     * @return UBaseType_t  The smallest amount of free stack space
     */
    UBaseType_t get_stack_high_water_mark_server();
    /**
     * @brief Get the stack size on server thread
     *
     * @return UBaseType_t  The size of the task stack specified as the number of bytes.
     */
    UBaseType_t get_stack_size_server();
#endif
};

#endif
