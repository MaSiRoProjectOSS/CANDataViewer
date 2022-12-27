/**
 * @file CanDataViewer_basic.ino
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.2
 * @date 2022-12-25
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */

//#define CONTROLLERS_M5STACK
#define CONTROLLERS_ATOM_LITE

#include <Arduino.h>
#ifdef CONTROLLERS_M5STACK
#include <M5Stack.h>
#else
// CONTROLLERS_ATOM_LITE
#include <M5Atom.h>
#endif

#include <can_data_viewer.hpp>

#define SETTING_WIFI_MODE_AP false
#define SETTING_WIFI_SSID    "(wifi ssid)"
#define SETTING_WIFI_PASS    "(wifi password)"

#ifdef CONTROLLERS_M5STACK
CanDataViewer can_data_viewer(G15, G12);
#else
// CONTROLLERS_ATOM_LITE
CanDataViewer can_data_viewer(G25, G19);
#endif

void setup()
{
    can_data_viewer.begin();

    // You can specify LAN connection information when this system boots.
    // can_data_viewer.begin(SETTING_WIFI_SSID, SETTING_WIFI_PASS, SETTING_WIFI_MODE_AP);
}

void loop()
{
    (void)delay(100);
}
