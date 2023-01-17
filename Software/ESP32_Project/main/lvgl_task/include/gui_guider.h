/*
 * @Author: StuTian
 * @Date: 2022-09-05 14:07
 * @LastEditors: letian
 * @LastEditTime: 2023-01-16 18:59
 * @FilePath: \ESP32_Project\main\lvgl_task\include\gui_guider.h
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
		lv_obj_t *boot;
			lv_obj_t *user_img;
			lv_obj_t *bar;
			lv_obj_t *bar_img;
		lv_obj_t *home;

} lv_ui;

extern lv_ui guider_ui;
void setup_ui(lv_ui *ui);
void setup_bg_screen(lv_ui *ui);
void setup_boot_screen(lv_ui *ui);
void setup_home_screen(lv_ui *ui);

LV_IMG_DECLARE(img_skew_strip);
LV_IMG_DECLARE(_StuLeTian_200x60);
LV_IMG_DECLARE(_rocket_60x35);


#endif