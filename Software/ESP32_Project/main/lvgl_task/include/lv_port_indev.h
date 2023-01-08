/*
 * @Author: letian
 * @Date: 2022-12-26 11:47
 * @LastEditors: letian
 * @LastEditTime: 2022-12-26 14:25
 * @FilePath: \lvgl_demo\main\lvgl_task\include\lv_port_indev.h
 * @Description: 
 * Copyright (c) 2022 by letian 1656733975@qq.com, All Rights Reserved. 
 */
#ifndef LV_PORT_INDEV_H
#define LV_PORT_INDEV_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "lvgl_init.h"
#include "button.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_port_indev_init(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/
