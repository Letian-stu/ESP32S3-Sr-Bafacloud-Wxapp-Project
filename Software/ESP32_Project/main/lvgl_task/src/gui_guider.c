/*
 * @Author: StuTian
 * @Date: 2022-09-05 14:07
 * @LastEditors: letian
 * @LastEditTime: 2023-01-18 17:25
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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gui_guider.h"
#include "gui_anim.h"

#define TAG "UI"

void setup_bg_screen(lv_ui *ui)
{
    ui->bg = lv_obj_create(lv_scr_act());
    lv_obj_align(ui->bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui->bg, 280, 240);
    lv_obj_set_style_bg_opa(ui->bg, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(ui->bg, lv_color_make(0xff, 0xff, 0xff), 0);
    lv_obj_set_scrollbar_mode(ui->bg, LV_SCROLLBAR_MODE_OFF);

}

static void set_temp(void *bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

void setup_boot_screen(lv_ui *ui)
{
    ui->boot = lv_obj_create(ui->bg);
    lv_obj_set_size(ui->boot, 280, 240);
    lv_obj_align(ui->boot, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->boot, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_img_src(&style_indic, &img_skew_strip);
    lv_style_set_bg_img_tiled(&style_indic, true);
    lv_style_set_bg_img_opa(&style_indic, LV_OPA_30);

    ui->user_img = lv_img_create(ui->boot);
    lv_obj_set_size(ui->user_img, 200, 60);
    lv_obj_align(ui->user_img, LV_ALIGN_CENTER, 0, -40);
    lv_img_set_src(ui->user_img, &_StuLeTian_200x60);

    ui->bar = lv_bar_create(ui->boot);
    lv_obj_add_style(ui->bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(ui->bar, 250, 20); // 15
    lv_obj_align(ui->bar, LV_ALIGN_CENTER, 0, 40);
    lv_bar_set_mode(ui->bar, LV_BAR_MODE_RANGE);
    lv_bar_set_value(ui->bar, 0, LV_ANIM_ON);

    ui->bar_img = lv_img_create(ui->boot);
    lv_obj_set_size(ui->bar_img, 60, 35);
    lv_obj_align_to(ui->bar_img, ui->bar, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
    lv_img_set_src(ui->bar_img, &_rocket_60x35);

    obj_set_anim(ui->bar, 0, 100, 3000, (uint32_t)NULL, (lv_anim_exec_xcb_t)set_temp, NULL, lv_anim_path_ease_in);
    obj_set_anim(ui->bar_img, 0, 280 - 30 - 60, 3000, (uint32_t)NULL, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, lv_anim_path_ease_in);

    lv_obj_del_delayed(ui->boot, 3000);
}

void setup_home_screen(lv_ui *ui)
{
    ui->home = lv_obj_create(ui->bg);
    lv_obj_set_size(ui->home, 280, 240);
    lv_obj_align(ui->home, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->home, LV_SCROLLBAR_MODE_OFF);

    ui->panel = lv_obj_create(ui->home);
    lv_obj_align(ui->panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui->panel, 280, 240);
    lv_obj_set_scroll_snap_x(ui->panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_align(ui->panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_flex_flow(ui->panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_scrollbar_mode(ui->panel, LV_SCROLLBAR_MODE_OFF);

    ui->clock_btn = lv_btn_create(ui->panel);
    lv_obj_remove_style_all(ui->clock_btn);//移除样式
    lv_obj_set_size(ui->clock_btn, 80, 80);//设置大小
    lv_obj_set_style_radius(ui->clock_btn, 10, 0);//设置倒圆角
    lv_obj_set_style_bg_color(ui->clock_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);//设置背景颜色
    lv_obj_set_style_bg_opa(ui->clock_btn, LV_OPA_0, 0);//背景透明度
    lv_obj_set_style_bg_color(ui->clock_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED);//设置被编码器聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->clock_btn, LV_OPA_20, LV_STATE_FOCUSED);//被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->clock_btn, LV_OPA_50, LV_STATE_PRESSED);//被按下时候的背景透明度
    ui->clock = lv_img_create(ui->clock_btn);
    lv_obj_set_size(ui->clock, 80, 80);
    lv_img_set_src(ui->clock, &_clock_80x80);

    ui->camera_btn = lv_btn_create(ui->panel);
    lv_obj_set_size(ui->camera_btn, 80, 80);
    lv_obj_remove_style_all(ui->camera_btn);//移除样式
    lv_obj_set_size(ui->camera_btn, 80, 80);//设置大小
    lv_obj_set_style_radius(ui->camera_btn, 10, 0);//设置倒圆角
    lv_obj_set_style_bg_color(ui->camera_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);//设置背景颜色
    lv_obj_set_style_bg_opa(ui->camera_btn, LV_OPA_0, 0);//背景透明度
    lv_obj_set_style_bg_color(ui->camera_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED);//设置被编码器聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->camera_btn, LV_OPA_20, LV_STATE_FOCUSED);//被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->camera_btn, LV_OPA_50, LV_STATE_PRESSED);//被按下时候的背景透明度
    ui->camera = lv_img_create(ui->camera_btn);
    lv_obj_set_size(ui->camera, 80, 80);
    lv_img_set_src(ui->camera, &_camera_80x80);

    ui->weather_btn = lv_btn_create(ui->panel);
    lv_obj_set_size(ui->weather_btn, 80, 80);
    lv_obj_remove_style_all(ui->weather_btn);//移除样式
    lv_obj_set_size(ui->weather_btn, 80, 80);//设置大小
    lv_obj_set_style_radius(ui->weather_btn, 10, 0);//设置倒圆角
    lv_obj_set_style_bg_color(ui->weather_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);//设置背景颜色
    lv_obj_set_style_bg_opa(ui->weather_btn, LV_OPA_0, 0);//背景透明度
    lv_obj_set_style_bg_color(ui->weather_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED);//设置被编码器聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->weather_btn, LV_OPA_20, LV_STATE_FOCUSED);//被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->weather_btn, LV_OPA_50, LV_STATE_PRESSED);//被按下时候的背景透明度
    ui->weather = lv_img_create(ui->weather_btn);
    lv_obj_set_size(ui->weather, 80, 80);
    lv_img_set_src(ui->weather, &_weather_80x80);

    ui->set_btn = lv_btn_create(ui->panel);
    lv_obj_set_size(ui->set_btn, 80, 80);
    lv_obj_remove_style_all(ui->set_btn);//移除样式
    lv_obj_set_size(ui->set_btn, 80, 80);//设置大小
    lv_obj_set_style_radius(ui->set_btn, 10, 0);//设置倒圆角
    lv_obj_set_style_bg_color(ui->set_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);//设置背景颜色
    lv_obj_set_style_bg_opa(ui->set_btn, LV_OPA_0, 0);//背景透明度
    lv_obj_set_style_bg_color(ui->set_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED);//设置被编码器聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->set_btn, LV_OPA_20, LV_STATE_FOCUSED);//被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->set_btn, LV_OPA_50, LV_STATE_PRESSED);//被按下时候的背景透明度
    ui->set = lv_img_create(ui->set_btn);
    lv_obj_set_size(ui->set, 80, 80);
    lv_img_set_src(ui->set, &_set_80x80);

    lv_group_add_obj(guider_ui.group, ui->clock_btn);
    lv_group_add_obj(guider_ui.group, ui->camera_btn);
    lv_group_add_obj(guider_ui.group, ui->weather_btn);
    lv_group_add_obj(guider_ui.group, ui->set_btn);

    obj_set_anim(ui->home, -240, 0, 500, 3000, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, lv_anim_path_bounce);
}

void test_sreen(lv_ui *ui)
{
    ui->home = lv_obj_create(ui->bg);
    lv_obj_set_size(ui->home, 280, 240);
    lv_obj_align(ui->home, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->home, LV_SCROLLBAR_MODE_OFF);

    ui->panel = lv_obj_create(ui->home);
    lv_obj_align(ui->panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui->panel, 280, 240);
    lv_obj_set_scroll_snap_x(ui->panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_align(ui->panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_flex_flow(ui->panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_scrollbar_mode(ui->panel, LV_SCROLLBAR_MODE_OFF);

    ui->clock_btn = lv_btn_create(ui->panel);
    lv_obj_remove_style_all(ui->clock_btn);//移除样式
    lv_obj_set_size(ui->clock_btn, 80, 80);//设置大小
    lv_obj_set_style_radius(ui->clock_btn, 10, 0);//设置倒圆角
    lv_obj_set_style_bg_color(ui->clock_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);//设置背景颜色
    lv_obj_set_style_bg_opa(ui->clock_btn, LV_OPA_0, 0);//背景透明度
    lv_obj_set_style_bg_color(ui->clock_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED);//设置被编码器聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->clock_btn, LV_OPA_20, LV_STATE_FOCUSED);//被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->clock_btn, LV_OPA_50, LV_STATE_PRESSED);//被按下时候的背景透明度
    ui->clock = lv_img_create(ui->clock_btn);
    lv_obj_set_size(ui->clock, 80, 80);
    lv_img_set_src(ui->clock, &_clock_80x80);

    ui->camera_btn = lv_btn_create(ui->panel);
    lv_obj_set_size(ui->camera_btn, 80, 80);
    lv_obj_remove_style_all(ui->camera_btn);//移除样式
    lv_obj_set_size(ui->camera_btn, 80, 80);//设置大小
    lv_obj_set_style_radius(ui->camera_btn, 10, 0);//设置倒圆角
    lv_obj_set_style_bg_color(ui->camera_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);//设置背景颜色
    lv_obj_set_style_bg_opa(ui->camera_btn, LV_OPA_0, 0);//背景透明度
    lv_obj_set_style_bg_color(ui->camera_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED);//设置被编码器聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->camera_btn, LV_OPA_20, LV_STATE_FOCUSED);//被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->camera_btn, LV_OPA_50, LV_STATE_PRESSED);//被按下时候的背景透明度
    ui->camera = lv_img_create(ui->camera_btn);
    lv_obj_set_size(ui->camera, 80, 80);
    lv_img_set_src(ui->camera, &_camera_80x80);

    ui->weather_btn = lv_btn_create(ui->panel);
    lv_obj_set_size(ui->weather_btn, 80, 80);
    lv_obj_remove_style_all(ui->weather_btn);//移除样式
    lv_obj_set_size(ui->weather_btn, 80, 80);//设置大小
    lv_obj_set_style_radius(ui->weather_btn, 10, 0);//设置倒圆角
    lv_obj_set_style_bg_color(ui->weather_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);//设置背景颜色
    lv_obj_set_style_bg_opa(ui->weather_btn, LV_OPA_0, 0);//背景透明度
    lv_obj_set_style_bg_color(ui->weather_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED);//设置被编码器聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->weather_btn, LV_OPA_20, LV_STATE_FOCUSED);//被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->weather_btn, LV_OPA_50, LV_STATE_PRESSED);//被按下时候的背景透明度
    ui->weather = lv_img_create(ui->weather_btn);
    lv_obj_set_size(ui->weather, 80, 80);
    lv_img_set_src(ui->weather, &_weather_80x80);

    ui->set_btn = lv_btn_create(ui->panel);
    lv_obj_set_size(ui->set_btn, 80, 80);
    lv_obj_remove_style_all(ui->set_btn);//移除样式
    lv_obj_set_size(ui->set_btn, 80, 80);//设置大小
    lv_obj_set_style_radius(ui->set_btn, 10, 0);//设置倒圆角
    lv_obj_set_style_bg_color(ui->set_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);//设置背景颜色
    lv_obj_set_style_bg_opa(ui->set_btn, LV_OPA_0, 0);//背景透明度
    lv_obj_set_style_bg_color(ui->set_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED);//设置被编码器聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->set_btn, LV_OPA_20, LV_STATE_FOCUSED);//被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->set_btn, LV_OPA_50, LV_STATE_PRESSED);//被按下时候的背景透明度
    ui->set = lv_img_create(ui->set_btn);
    lv_obj_set_size(ui->set, 80, 80);
    lv_img_set_src(ui->set, &_set_80x80);

    lv_group_add_obj(guider_ui.group, ui->clock_btn);
    lv_group_add_obj(guider_ui.group, ui->camera_btn);
    lv_group_add_obj(guider_ui.group, ui->weather_btn);
    lv_group_add_obj(guider_ui.group, ui->set_btn);
}

void setup_ui(lv_ui *ui)
{
    setup_bg_screen(ui);
    setup_boot_screen(ui);
    setup_home_screen(ui);
    //test_sreen(ui);
}
