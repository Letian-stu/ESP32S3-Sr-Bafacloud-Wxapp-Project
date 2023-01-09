/*
 * @Author: StuTian
 * @Date: 2022-09-05 14:07
 * @LastEditors: letian
 * @LastEditTime: 2023-01-09 21:58
 * @FilePath: \ESP32_Project\main\lvgl_task\src\gui_guider.c
 * @Description:
 * Copyright (c) 2022 by StuTian 1656733975@qq.com, All Rights Reserved.
 */
/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include "string.h"
#include "lvgl/lvgl.h"
#include "gui_guider.h"

#define TAG "UI"

static void set_temp(void *bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

void setup_scr_screen(lv_ui *ui)
{
    ui->bg = lv_obj_create(NULL);
    lv_obj_set_pos(ui->bg, 0, 0);
    lv_obj_set_size(ui->bg, 240, 320);

    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_img_src(&style_indic, &img_skew_strip);
    lv_style_set_bg_img_tiled(&style_indic, true);
    lv_style_set_bg_img_opa(&style_indic, LV_OPA_30);

    ui->user_img = lv_img_create(ui->bg);
    lv_obj_set_size(ui->user_img, 200, 60);
    lv_obj_align(ui->user_img, LV_ALIGN_CENTER, 0, -40);
    lv_img_set_src(ui->user_img, &_StuLeTian_200x60);

    ui->bar = lv_bar_create(ui->bg);
    lv_obj_add_style(ui->bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(ui->bar, 260, 20);
    lv_obj_align(ui->bar, LV_ALIGN_CENTER, 0, 40);
    lv_bar_set_mode(ui->bar, LV_BAR_MODE_RANGE);
    lv_bar_set_value(ui->bar, 90, LV_ANIM_ON);

    ui->bar_img = lv_img_create(ui->bg);
    lv_obj_set_size(ui->bar_img, 60, 35);
    lv_obj_align_to(ui->bar_img, ui->bar,LV_ALIGN_OUT_TOP_LEFT, 0, 0);
    lv_img_set_src(ui->bar_img, &_rocket_60x35);

    lv_anim_t bar_img_anim;
    lv_anim_init(&bar_img_anim);
    lv_anim_set_time(&bar_img_anim, 3000);
    lv_anim_set_var(&bar_img_anim, ui->bar_img);
    lv_anim_set_exec_cb(&bar_img_anim, (lv_anim_exec_xcb_t)lv_obj_set_x);
	lv_anim_set_values(&bar_img_anim, 30, 230);
	lv_anim_set_path_cb(&bar_img_anim, lv_anim_path_linear);

    lv_anim_t bar_anim;
    lv_anim_init(&bar_anim);
    lv_anim_set_time(&bar_anim, 3000);
    lv_anim_set_var(&bar_anim, ui->bar);
    lv_anim_set_values(&bar_anim, 0, 100);
    lv_anim_set_exec_cb(&bar_anim, set_temp);

    lv_anim_start(&bar_img_anim);
    lv_anim_start(&bar_anim);
    
    lv_obj_del_delayed(ui->user_img, 3100);
    lv_obj_del_delayed(ui->bar_img, 3100);
    lv_obj_del_delayed(ui->bar, 3100);
}

void setup_ui(lv_ui *ui)
{
    setup_scr_screen(ui);
    lv_scr_load(ui->bg);
}
