/**
 * @file can_data_viewer_conf.hpp
 * @brief CAN Data Viewerの設定定義ファイル
 * @version 0.1
 * @date 2022-11-26
 *
 * このファイルはCAN Data Viewerの各種設定値（スレッド間隔、コア番号、システム名など）を定義します。
 * プロジェクト全体で共通して利用される定数をまとめています。
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 */
#ifndef CAN_DATA_VIEWER_CONF_HPP
#define CAN_DATA_VIEWER_CONF_HPP

#ifndef CAN_THREAD_INTERVAL
#define CAN_THREAD_INTERVAL (2)
#endif

#ifndef THREAD_CORE_CAN
#define THREAD_CORE_CAN (0)
#endif

#ifndef SETTING_SYSTEM_NAME
#define SETTING_SYSTEM_NAME "CAN Data Viewer"
#endif

#endif
