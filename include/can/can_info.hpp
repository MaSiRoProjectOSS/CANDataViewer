/**
 * @file can_info.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */

#ifndef MASIRO_PROJECT_TOY_BOX_CAN_INFO_HPP
#define MASIRO_PROJECT_TOY_BOX_CAN_INFO_HPP

#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#endif

#include "can_config.h"

#include <Arduino.h>

namespace MaSiRoProject
{
namespace ToyBox
{
namespace CAN
{
class CanData {
public:
    unsigned long Id            = 0;
    byte Length                 = 0;
    byte ExtFlag                = 0;
    byte Data[CAN_MESSAGE_SIZE] = { 0 };

public:
    int loop_interval  = 0;
    unsigned long time = millis();

    static bool compar_Id(const CanData &left, const CanData &right) { return left.Id < right.Id; }
    static bool compar_Time(const CanData &left, const CanData &right) { return left.time > right.time; }
};
////////////////////////////////////////////////////////////////
typedef enum can_ctrl_state
{
    MODE_UNKNOW = 0,
    MODE_NOT_INITIALIZE,
    MODE_READY,
    MODE_RUNNING,
    MODE_INACTIVE,
    MODE_STOPPING,
    MODE_ABORT,
    MODE_PAUSE,
    MODE_FINISHED
} CAN_CTRL_STATE;
////////////////////////////////////////////////////////////////
class CanDeviceInfo {
public:
    int can_type            = 0; /*! 0:std / 1:ext / 2:any */
    char can_speed_txt[255] = "";
    int can_speed           = 0;
    can_ctrl_state mode     = CAN_CTRL_STATE::MODE_NOT_INITIALIZE;
    char mode_text[255]     = "--";
};

////////////////////////////////////////////////////////////////
typedef std::function<void(CAN_CTRL_STATE, const char *)> CanCommunicationChangedModeFunction;
typedef std::function<void(CanData)> CanCommunicationGetReceivedFunction;
typedef std::function<bool(CanData *)> CanCommunicationSendEventFunction;

////////////////////////////////////////////////////////////////
} // namespace CAN
} // namespace ToyBox
} // namespace MaSiRoProject
#endif