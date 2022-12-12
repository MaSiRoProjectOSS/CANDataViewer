/**
 * @file controller_can.cpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#include "controller_can.hpp"

#ifndef SAMPLE_CAN_SEND_READY
#define SAMPLE_CAN_SEND_READY 0
#endif
#ifndef SAMPLE_CAN_SEND_RUNNING
#define SAMPLE_CAN_SEND_RUNNING 0
#endif
#ifndef SAMPLE_CAN_SEND_RUNNING_LOOP_SHOT
#define SAMPLE_CAN_SEND_RUNNING_LOOP_SHOT 0
#endif
#ifndef SAMPLE_CAN_SEND_STOPPING
#define SAMPLE_CAN_SEND_STOPPING 0
#endif
#ifndef SAMPLE_CAN_PUSH_CHANGE_MODE
#define SAMPLE_CAN_PUSH_CHANGE_MODE 1
#endif

using namespace MaSiRoProject::ToyBox;

void ControllerCan::push(CAN::CAN_CTRL_STATE mode)
{
#if SAMPLE_CAN_PUSH_CHANGE_MODE
    this->change_mode(mode);
#else
    static int counter = 0;
    ///////////////////////////////////////////////////////
    CAN::CanData data;
    data.Id      = 0x90 + counter;
    data.Length  = 0x08;
    data.Data[0] = counter;
    data.Data[1] = 0x20 + random(1, 9);
    data.Data[2] = 0x30 + random(1, 9);
    data.Data[3] = 0x40 + random(1, 9);
    data.Data[4] = 0x50 + random(1, 9);
    data.Data[5] = 0x60 + random(1, 9);
    data.Data[6] = 0x70 + random(1, 9);
    data.Data[7] = 0x80 + random(1, 9);
    add_one_shot(data);
    ///////////////////////////////////////////////////////
    counter++;
    if (counter > 0x0F) {
        counter = 0;
    }
#endif
}
////////////////////////////////////////////////////////////////////////////////////////
bool ControllerCan::send_for_ready(CAN::CanData *data)
{
    bool result = true;
#if SAMPLE_CAN_SEND_READY
    static int cnt = 1;
    ////////////////////////////////////
    data->Id      = 0x0C;
    data->Length  = cnt;
    data->Data[0] = cnt;
    data->Data[1] = 0x20;
    data->Data[2] = 0x30;
    data->Data[3] = 0x40;
    data->Data[4] = 0x50;
    data->Data[5] = 0x60;
    data->Data[6] = 0x70;
    data->Data[7] = 0x80;
    ////////////////////////////////////
    if (cnt > 5) {
        cnt = 0;
        CAN::CanData data;
        int counter  = 0;
        data.Id      = 0x30 + counter;
        data.Length  = 0x08;
        data.Data[0] = 0x10 + random(1, 9);
        data.Data[1] = 0x20 + random(1, 9);
        data.Data[2] = 0x30 + random(1, 9);
        data.Data[3] = 0x40 + random(1, 9);
        data.Data[4] = 0x50 + random(1, 9);
        data.Data[5] = 0x60 + random(1, 9);
        data.Data[6] = 0x70 + random(1, 9);
        data.Data[7] = counter++;
        add_one_shot(data);
        data.Id      = 0x30 + counter;
        data.Data[7] = counter++;
        add_one_shot(data);
    } else {
        result = false;
    }
    cnt++;
#endif
    ////////////////////////////////////
    return result;
}

bool ControllerCan::send_for_running(CAN::CanData *data)
{
    bool result = true;
#if SAMPLE_CAN_SEND_RUNNING
    static int counter = 0;
    byte ra            = random(1, 9);
    switch (counter) {
        case (0):
            data->Id      = 0x0E;
            data->Length  = 0x08;
            data->Data[0] = 0x10 + random(1, 9);
            data->Data[1] = 0x20 + random(1, 9);
            data->Data[2] = 0x30 + random(1, 9);
            data->Data[3] = 0x40 + random(1, 9);
            data->Data[4] = 0x50 + random(1, 9);
            data->Data[5] = 0x60 + random(1, 9);
            data->Data[6] = 0x70 + random(1, 9);
            data->Data[7] = counter;
            break;
        case (50):
            data->Id      = 0x0D;
            data->Length  = 0x08;
            data->Data[0] = 0x10 + random(1, 9);
            data->Data[1] = 0x20;
            data->Data[2] = 0x30;
            data->Data[3] = 0x40;
            data->Data[4] = 0x50;
            data->Data[5] = 0x60;
            data->Data[6] = 0x70;
            data->Data[7] = counter;
            break;
        default:
            break;
    }
    counter++;
    if (counter >= 100) {
        counter = 0;
    }
#endif
    return result;
}

bool ControllerCan::send_for_stopping(CAN::CanData *data)
{
    bool result = true;
#if SAMPLE_CAN_SEND_STOPPING
    static int cnt = 1;
    ////////////////////////////////////
    data->Id      = 0x0A;
    data->Length  = cnt;
    data->Data[0] = cnt * 0x10;
    data->Data[1] = 0x20;
    data->Data[2] = 0x30;
    data->Data[3] = 0x40;
    data->Data[4] = 0x50;
    data->Data[5] = 0x60;
    data->Data[6] = 0x70;
    data->Data[7] = 0x80;
    ////////////////////////////////////
    if (cnt > 5) {
        cnt = 0;
    } else {
        result = false;
    }
    cnt++;
    ////////////////////////////////////
#endif
    return result;
}

bool ControllerCan::setup_default(void)
{
    bool result = clear_loop_shot();

#if SAMPLE_CAN_SEND_RUNNING_LOOP_SHOT
    CAN::CanData data;
    if (true == result) {
        ///////////
        data.Id            = 0x20;
        data.Length        = 0x08;
        data.Data[0]       = 0x10 + random(1, 9);
        data.Data[1]       = 0x20 + random(1, 9);
        data.Data[2]       = 0x30 + random(1, 9);
        data.Data[3]       = 0x40 + random(1, 9);
        data.Data[4]       = 0x50 + random(1, 9);
        data.Data[5]       = 0x60 + random(1, 9);
        data.Data[6]       = 0x70 + random(1, 9);
        data.loop_interval = 1000;
        add_loop_shot(data, data.loop_interval);
    }

    if (true == result) {
        data.Id            = 0xF0;
        data.Length        = 0x01;
        data.loop_interval = 0;
        this->set_resume(data);
    }

#endif
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////
ControllerCan::ControllerCan() : CanCommunication()
{
}
