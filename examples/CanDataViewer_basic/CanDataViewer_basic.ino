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
#include <Arduino.h>
#include <M5Atom.h>
#include <can_data_viewer.hpp>

#define SETTING_WIFI_MODE_AP false
#define SETTING_WIFI_SSID    "(wifi ssid)"
#define SETTING_WIFI_PASS    "(wifi password)"

CanDataViewer can_data_viewer;

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
