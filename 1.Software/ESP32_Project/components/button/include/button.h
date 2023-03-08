/*
 * @Author: StuTian
 * @Date: 2022-09-04 22:11
 * @LastEditors: letian
 * @LastEditTime: 2023-01-07 16:19
 * @FilePath: \lvgl_demo\components\button\include\button.h
 * @Description: 
 * Copyright (c) 2022 by StuTian 1656733975@qq.com, All Rights Reserved. 
 */
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "bsp_button.h"
#include "driver/gpio.h"

enum
{
    BT_NONE,
    BT1_DOWN, //1
    BT1_DOUBLE,
    BT1_LONG,
    BT1_LONGFREE,

    BT2_DOWN, //5
    BT2_DOUBLE,
    BT2_LONG,
    BT2_LONGFREE,

    BT3_DOWN, //9
    BT3_DOUBLE,
    BT3_LONG,
    BT3_LONGFREE,
};

extern uint8_t Button_Value;
void Button_Init(void);

#endif
