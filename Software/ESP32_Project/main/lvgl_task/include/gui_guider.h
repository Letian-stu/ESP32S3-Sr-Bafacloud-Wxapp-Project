/*
 * @Author: StuTian
 * @Date: 2022-09-05 14:07
 * @LastEditors: letian
 * @LastEditTime: 2022-12-26 18:23
 * @FilePath: \lvgl_demo\main\lvgl_task\include\gui_guider.h
 * @Description:
 * Copyright (c) 2022 by StuTian 1656733975@qq.com, All Rights Reserved.
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H

#include "lvgl/lvgl.h"

typedef struct
{
	lv_group_t *group;
	lv_obj_t *bg;
    lv_obj_t * sw;


	// lv_obj_t *label;
	// lv_obj_t *btn1;
	// lv_obj_t *btn2;
	// lv_obj_t *btn3;
	// lv_obj_t *btn4;
} lv_ui;

extern lv_ui guider_ui;
void setup_ui(lv_ui *ui);
void setup_scr_screen(lv_ui *ui);

#endif