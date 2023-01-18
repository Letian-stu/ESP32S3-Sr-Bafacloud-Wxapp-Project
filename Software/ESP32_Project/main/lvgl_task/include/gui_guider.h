/*
 * @Author: StuTian
 * @Date: 2022-09-05 14:07
 * @LastEditors: letian
 * @LastEditTime: 2023-01-18 14:56
 * @FilePath: \ESP32_Project\main\lvgl_task\include\gui_guider.h
 * @Description:
 * Copyright (c) 2022 by StuTian 1656733975@qq.com, All Rights Reserved.
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H

#include "lvgl/lvgl.h"

#define COLOR_SPRING_GREEN   0x00cd66
#define COLOR_FOREST_GREEN   0x22b822
#define COLOR_DODGER_BLUE    0x1e90ff
#define COLOR_DARK_ORANGE    0xffa500
#define COLOR_RED            0xee0000
#define COLOR_PURPLE         0xa020f0

typedef struct
{
	lv_group_t *group;
	lv_obj_t *bg;
		//1
		lv_obj_t *boot;
			lv_obj_t *user_img;
			lv_obj_t *bar;
			lv_obj_t *bar_img;
		//2
		lv_obj_t *home;
		lv_obj_t *panel;
			lv_obj_t *clock_btn;
			lv_obj_t *clock;
			lv_obj_t *camera_btn;
			lv_obj_t *camera;
			lv_obj_t *weather_btn;
			lv_obj_t *weather;
			lv_obj_t *set_btn;
			lv_obj_t *set;
		//3
} lv_ui;

extern lv_ui guider_ui;
void setup_ui(lv_ui *ui);
void setup_bg_screen(lv_ui *ui);
void setup_boot_screen(lv_ui *ui);
void setup_home_screen(lv_ui *ui);

LV_IMG_DECLARE(img_skew_strip);
LV_IMG_DECLARE(_StuLeTian_200x60);
LV_IMG_DECLARE(_rocket_60x35);

LV_IMG_DECLARE(_clock_80x80);
LV_IMG_DECLARE(_camera_80x80);
LV_IMG_DECLARE(_weather_80x80);
LV_IMG_DECLARE(_set_80x80);



#endif