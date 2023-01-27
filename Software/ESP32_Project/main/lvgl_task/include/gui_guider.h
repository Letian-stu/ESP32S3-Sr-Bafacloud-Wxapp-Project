/*
 * @Author: StuTian
 * @Date: 2022-09-05 14:07
 * @LastEditors: letian
 * @LastEditTime: 2023-01-27 20:09
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

enum PAGE
{
	PAGE_HOME = 0,
	PAGE_CLOCK,
	PAGE_WEATHER,
	PAGE_CONTROL,
	PAGE_CAMERA,
	PAGE_IMAGE,
	PAGE_SDCARD
};

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
		lv_obj_t *home_label;
		lv_obj_t *panel;
			lv_obj_t *clock_btn;
			lv_obj_t *clock;
			lv_obj_t *weather_btn;
			lv_obj_t *weather;
			lv_obj_t *set_btn;
			lv_obj_t *set;			
			lv_obj_t *camera_btn;
			lv_obj_t *camera;
			lv_obj_t *image_btn;
			lv_obj_t *image;
			lv_obj_t *sd_btn;
			lv_obj_t *sd;
		//3
		lv_obj_t *page;
			lv_obj_t *back_btn;
			lv_obj_t *back;
			lv_obj_t *label;
			lv_obj_t *img_list;
			lv_obj_t *img_btn;

} lv_ui;

extern lv_ui guider_ui;

void lv_btn_back_event_cb(lv_event_t *e);

void setup_bg_screen(lv_ui *ui);
void setup_bg_screen(lv_ui *ui);
void setup_home_screen(lv_ui *ui, uint32_t delay);
void setup_ui(lv_ui *ui);

void setup_camera_screen(lv_ui *ui, uint32_t time, uint32_t delay);
void setup_clock_screen(lv_ui *ui, uint32_t time, uint32_t delay);
void setup_image_screen(lv_ui *ui, uint32_t time, uint32_t delay);
void setup_sd_screen(lv_ui *ui, uint32_t time, uint32_t delay);
void setup_set_screen(lv_ui *ui, uint32_t time, uint32_t delay);
void setup_weather_screen(lv_ui *ui, uint32_t time, uint32_t delay);

LV_IMG_DECLARE(img_skew_strip);
LV_IMG_DECLARE(_StuLeTian_200x60);
LV_IMG_DECLARE(_rocket_60x35);
LV_IMG_DECLARE(_clock_110x110);
LV_IMG_DECLARE(_weather_110x110);
LV_IMG_DECLARE(_set_110x110);
LV_IMG_DECLARE(_camera_110x110);
LV_IMG_DECLARE(_image_110x110);
LV_IMG_DECLARE(_sd_110x110);
LV_IMG_DECLARE(_back_40x40);

#endif