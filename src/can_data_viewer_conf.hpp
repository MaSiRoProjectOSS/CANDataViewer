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

#ifndef THREAD_CORE_CAN
#define THREAD_CORE_CAN (0)
#endif
#ifndef THREAD_CORE_WIFI
#define THREAD_CORE_WIFI (1)
#endif

#ifndef SETTING_SYSTEM_NAME
#define SETTING_SYSTEM_NAME "CAN Data Viewer"
#endif

#ifdef SETTING_HAS_PRIVATE_FILE
#if SETTING_HAS_PRIVATE_FILE
#include "private_setting.h"
#endif
#endif

#ifndef SETTING_WIFI_MODE_AP
/**
 * @brief Specify Wifi mode : Access point
 *
 */
#define SETTING_WIFI_MODE_AP false
#endif

#ifndef SETTING_WIFI_SSID
/**
 * @brief Specify Wifi SSID
 *
 */
#define SETTING_WIFI_SSID "(wifi ssid)"
#endif

#ifndef SETTING_WIFI_PASS
/**
 * @brief Specify Wifi SSID password
 *
 */
#define SETTING_WIFI_PASS "(wifi password)"
#endif

#ifndef SETTING_WIFI_PORT
/**
 * @brief Specify Wifi SSID password
 *
 */
#define SETTING_WIFI_PORT 80
#endif

#endif