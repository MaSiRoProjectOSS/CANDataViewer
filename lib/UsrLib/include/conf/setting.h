/**
 * @file setting.h
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-26
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef USR_LIB_SETTING_H
#define USR_LIB_SETTING_H

///////////////////////////////////////////////////////////////////
#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#endif
///////////////////////////////////////////////////////////////////

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

#ifndef SETTING_SETUP_SLEEP_ON_STARTUP
#define SETTING_SETUP_SLEEP_ON_STARTUP 1000
#endif

#ifndef SETTING_LOOP_TIME_SLEEP_DETECT
/**
 * @brief Specify no response time after detection (unit: mm)
 *
 */
#define SETTING_LOOP_TIME_SLEEP_DETECT 1000
#endif
#ifndef SETTING_SYSTEM_NAME
#define SETTING_SYSTEM_NAME "CAN Data Viewer"
#endif
#ifndef SETTING_SYSTEM_VERSION_MAJOR
#define SETTING_SYSTEM_VERSION_MAJOR 1
#endif
#ifndef SETTING_SYSTEM_VERSION_MINER
#define SETTING_SYSTEM_VERSION_MINER 0
#endif
#ifndef SETTING_SYSTEM_ID
#define SETTING_SYSTEM_ID 0
#endif

#endif