/**
 * @file can_data_viewer_info.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_CAN_DATA_VIEWER_INFO_HPP
#define MASIRO_PROJECT_CAN_DATA_VIEWER_INFO_HPP

#include <Arduino.h>
#include <SPIFFS.h>
#include <WebServer.h>

#ifndef CAN_MESSAGE_SIZE
#define CAN_MESSAGE_SIZE (8)
#endif

typedef enum can_ctrl_state
{
    MODE_UNKNOW = 0,     //!< unknow
    MODE_NOT_INITIALIZE, //!< Not Initialize
    MODE_READY,          //!< Ready
    MODE_RUNNING,        //!< Running
    MODE_INACTIVE,       //!< Inactive
    MODE_STOPPING,       //!< Stopping
    MODE_ABORT,          //!< Abort
    MODE_PAUSE,          //!< Pause
    MODE_FINISHED        //!< Finished
} CAN_CTRL_STATE;

typedef enum output_log_level
{
    /**
     * @brief  Any logging levels that have been configured are logged at this log level.
     */
    OUTPUT_LOG_LEVEL_ALL,
    /**
     * @brief The TRACE log level records all of the application's behaviour details. Its purpose is primarily diagnostic, and it is more granular and finer than the DEBUG log level.
     */
    OUTPUT_LOG_LEVEL_TRACE,
    /**
     * @brief You are providing diagnostic information in a thorough manner with DEBUG. It's long and contains more information than you'll need when using the application.
     */
    OUTPUT_LOG_LEVEL_DEBUG,
    /**
     * @brief INFO messages are similar to how applications normally behave.
     */
    OUTPUT_LOG_LEVEL_INFO,
    /**
     * @briefWhen This log level signals operational messages
     */
    OUTPUT_LOG_LEVEL_MESSAGE,
    /**
     * @briefWhen an unexpected application issue has been identified, the WARN log level is used.  This indicates that you are unsure if the issue will recur or not. At this time, you may not notice any negative effects on your application.
     */
    OUTPUT_LOG_LEVEL_WARN,
    /**
     * @brief This log level is used when a serious issue is preventing the application's functionalities from functioning properly.
     */
    OUTPUT_LOG_LEVEL_ERROR,
    /**
     * @brief The FATAL level of logging indicates that the application's situation is critical, such as when a critical function fails.
     */
    OUTPUT_LOG_LEVEL_FATAL,
    /**
     * @brief Nothing is logged at this level of logging.
     */
    OUTPUT_LOG_LEVEL_OFF

} OUTPUT_LOG_LEVEL;
////////////////////////////////////////////////////////////////

/**
 * @brief Structure of CAN data
 *
 */
class CanData {
public:
    unsigned long Id            = 0;     //!< CAN ID
    byte ExtFlag                = 0;     //!< 0: Standard CAN 1: Extended CAN
    byte Length                 = 0;     //!< Data length
    byte Data[CAN_MESSAGE_SIZE] = { 0 }; //!< Data

public:
    int loop_interval  = 0;        //!< Loop interval
    unsigned long time = millis(); //!< Time processed

    /**
     * @brief Sort by ID
     *
     * @param left
     * @param right
     * @return true
     * @return false
     */
    static bool compar_Id(const CanData &left, const CanData &right) { return left.Id < right.Id; }
    /**
     * @brief Sort by time
     *
     * @param left
     * @param right
     * @return true
     * @return false
     */
    static bool compar_Time(const CanData &left, const CanData &right) { return left.time > right.time; }
};

class NetworkList {
public:
    String name;                 //!< SSID
    int rssi;                    //!< RSSI
    int quality;                 //!< RSSI as Quality
    wifi_auth_mode_t encryption; //!< Encryption Type

public:
    /**
     * @brief Sort by RSSI
     *
     * @param left
     * @param right
     * @return true
     * @return false
     */
    static bool compar_rssi(const NetworkList &left, const NetworkList &right) { return left.rssi > right.rssi; }
};

class CanDeviceInfo {
public:
    int can_type            = 0; /*! 0:std / 1:ext / 2:any */
    char can_speed_txt[255] = "";
    int can_speed           = 0;
    can_ctrl_state mode     = CAN_CTRL_STATE::MODE_NOT_INITIALIZE;
    char mode_text[255]     = "--";
};

////////////////////////////////////////////////////////////////
typedef std::function<void(OUTPUT_LOG_LEVEL, const char *, const char *, const char *, int)> MessageFunction;
typedef std::function<void(CAN_CTRL_STATE, const char *)> ChangedModeFunction;
typedef std::function<void(CanData)> GetReceivedFunction;
typedef std::function<bool(CAN_CTRL_STATE, CanData *)> SendEventFunction;
typedef std::function<bool(void)> SettingDefaultFunction;

///////////////////////////////////////////////////////////////////

#endif