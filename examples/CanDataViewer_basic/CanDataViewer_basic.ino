/**
 * @file CanDataViewer_basic.ino
 * @brief CANデータビューアの基本的なサンプルコードです。
 *        M5StackまたはM5Atom Liteコントローラで動作し、CANデータの受信を開始します。
 * @version 0.2
 * @date 2022-12-25
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 * このファイルは、CANDataViewerライブラリを使用してCANバスデータを取得するための
 * 基本的なセットアップ例です。コントローラの種類（M5StackまたはAtom Lite）に応じて
 * 必要なライブラリやピン設定を切り替えています。
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

#if LIB_CAN_DRIVER == 0
// ESP32CAN does not require Interrupt Pin and Chip select Pin
CanDataViewer can_data_viewer;
#else
// MCP2515 should be Pin set.
#ifdef CONTROLLERS_M5STACK
CanDataViewer can_data_viewer(G15, G12);
#else
// CONTROLLERS_ATOM_LITE
CanDataViewer can_data_viewer(G25, G19);
#endif
#endif

void setup()
{
    can_data_viewer.begin();
}

void loop()
{
    (void)delay(100);
}
