/**
 * @file can_data_viewer_conf.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-26
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef CAN_DATA_VIEWER_CONF_HPP
#define CAN_DATA_VIEWER_CONF_HPP

#ifndef CAN_THREAD_INTERVAL
#define CAN_THREAD_INTERVAL (5)
#endif

#ifndef THREAD_CORE_CAN
#define THREAD_CORE_CAN (0)
#endif
#ifndef THREAD_CORE_WIFI
#define THREAD_CORE_WIFI (1)
#endif

#ifndef SETTING_SYSTEM_NAME
#define SETTING_SYSTEM_NAME "CAN Data Viewer"
#endif

#ifndef STORAGE_SPI_FS
#define STORAGE_SPI_FS 1
#endif
#ifndef STORAGE_SPI_FS_INITIALIZE
#define STORAGE_SPI_FS_INITIALIZE 0
#endif
#ifndef STORAGE_SPI_FS_FORMAT
#define STORAGE_SPI_FS_FORMAT 1
#endif

#ifndef SETTING_WIFI_PORT
/**
 * @brief Specify Wifi SSID password
 *
 */
#define SETTING_WIFI_PORT 80
#endif

#ifndef SETTING_WIFI_SETTING_FILE
/**
 * @brief Specify Wifi settings
 *
 */
#define SETTING_WIFI_SETTING_FILE "/info_wifi.txt"
#endif

#ifndef SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS
/**
 * @brief Specify Wifi mode : the system transitions to AP mode
 * when If connection is not possible for a certain period of time in STA mode.
 *
 */
#define SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS 0
#endif
#ifndef SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS_TIMEOUT
/**
 * @brief Time to wait for transition
 *
 */
#define SETTING_WIFI_MODE_AP_AUTO_TRANSITIONS_TIMEOUT (60 * 1000)
#endif

#ifndef SETTING_WIFI_MODE_AP
/**
 * @brief Specify Wifi mode : Access point
 *
 */
#define SETTING_WIFI_MODE_AP true
#endif

#ifndef SETTING_WIFI_SSID_DEFAULT_AP
/**
 * @brief Specify Wifi SSID on AP
 *
 */
#define SETTING_WIFI_SSID_DEFAULT_AP "AP_CAN_DATA_VIEWER"
#endif

#ifndef SETTING_WIFI_PASS_DEFAULT_AP
/**
 * @brief Specify Wifi SSID password on AP
 *
 */
#define SETTING_WIFI_PASS_DEFAULT_AP "password"
#endif
#ifndef SETTING_WIFI_SSID_DEFAULT_STA
/**
 * @brief Specify Wifi SSID on STA
 *
 */
#define SETTING_WIFI_SSID_DEFAULT_STA "STA_CAN_DATA_VIEWER"
#endif

#ifndef SETTING_WIFI_PASS_DEFAULT_STA
/**
 * @brief Specify Wifi SSID password on STA
 *
 */
#define SETTING_WIFI_PASS_DEFAULT_STA "password"
#endif

#endif
