/**
 * @file can_data_viewer.hpp
 * @brief CANデータビューアのクラス定義ファイル
 * @details CAN通信データの管理・送受信・表示を行うためのクラスを定義します。
 *          コールバック関数による各種イベント処理や、デバッグ用のスタック情報取得機能も備えています。
 * @version 0.1
 * @date 2022-12-20
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 */
#ifndef MASIRO_PROJECT_CAN_DATA_VIEWER_HPP
#define MASIRO_PROJECT_CAN_DATA_VIEWER_HPP

#include <can_data_viewer_info.hpp>
#include <cushy_web_server.hpp>

/**
 * @class CanDataViewer
 * @brief CANデータの表示および制御を行うクラス
 *
 * このクラスはCANデータの送受信、表示、モード変更、コールバック設定などの機能を提供します。
 * デバッグモードでは、各スレッドのスタック情報取得も可能です。
 *
 * 標準関数:
 *   - CanDataViewer: コンストラクタ。割り込み番号とCS番号を指定可能。
 *   - ~CanDataViewer: デストラクタ。
 *   - begin: 初期化処理を行う。
 *
 * セットアップ関数:
 *   - set_callback_changed_mode: モード変更時のコールバックを設定。
 *   - set_callback_received: CANデータ受信時のコールバックを設定。
 *   - set_callback_sendable_interrupt: 送信可能割り込み時のコールバックを設定。
 *   - set_callback_setting_default: デフォルト設定時のコールバックを設定。
 *
 * 制御関数:
 *   - set_mode: モードを設定または切り替える。
 *   - clear_resume: ブラウザ表示用CANデータリストをクリア。
 *   - clear_loop_shot: 定期送信CANデータをクリア。
 *   - add_one_shot: 1回のみ送信するCANデータを追加。
 *   - add_loop_shot: 定期送信するCANデータを追加。
 *   - add_resume: ブラウザ表示用リストにCANデータを追加。
 *
 * デバッグ関数 (DEBUG_MODE時):
 *   - get_stack_high_water_mark_can: CANスレッドのスタック使用量取得。
 *   - get_stack_size_can: CANスレッドのスタックサイズ取得。
 *   - get_stack_high_water_mark_server: サーバースレッドのスタック使用量取得。
 *   - get_stack_size_server: サーバースレッドのスタックサイズ取得。
 *   - get_stack_size_wifi: WiFiスレッドのスタックサイズ取得。
 *   - get_stack_high_water_mark_wifi: WiFiスレッドのスタック使用量取得。
 */
class CanDataViewer {
    ////////////////////////////////////////////////
    // standard function
    ////////////////////////////////////////////////
public:
    /**
     * @brief Construct a new Can Data Viewer object
     *
     */
#if LIB_CAN_DRIVER == 1
    CanDataViewer(const uint8_t interrupt = 0, const uint8_t cs = 0);
#else
    CanDataViewer(const uint8_t rx, const uint8_t tx);
#endif
    /**
     * @brief Destroy the Can Data Viewer object
     *
     */
    ~CanDataViewer();
    /**
     * @brief 初期化処理を行う。
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
    /**
     * @brief Get the stack high water mark on wifi thread
     *
     * @return UBaseType_t  The smallest amount of free stack space
     */
    UBaseType_t get_stack_size_wifi();
    /**
     * @brief Get the stack high water mark on wifi thread
     *
     * @return UBaseType_t  The smallest amount of free stack space
     */
    UBaseType_t get_stack_high_water_mark_wifi();

#endif
};

#endif
