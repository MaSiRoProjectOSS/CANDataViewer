/**
 * @file private_setting.h
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-26
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef __PRIVATE_SETTING_H
#define __PRIVATE_SETTING_H

#ifdef SETTING_SYSTEM_NAME
#undef SETTING_SYSTEM_NAME
#endif
#ifndef SETTING_SYSTEM_NAME
#define SETTING_SYSTEM_NAME "CAN Data Viewer"
#endif

#ifdef SETTING_WIFI_MODE_AP
#undef SETTING_WIFI_MODE_AP
#endif
#define SETTING_WIFI_MODE_AP false

#ifdef SETTING_WIFI_SSID
#undef SETTING_WIFI_SSID
#endif
#ifdef SETTING_WIFI_PASS
#undef SETTING_WIFI_PASS
#endif

#if SETTING_WIFI_MODE_AP
#define SETTING_WIFI_SSID "AtomAP"
#else
#define SETTING_WIFI_SSID "GL-MT1300-272-2G"
#endif
#define SETTING_WIFI_PASS "Kawaiiy0"

#endif