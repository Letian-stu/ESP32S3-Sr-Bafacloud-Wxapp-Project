/*
 * @Author: StuTian
 * @Date: 2022-09-05 14:07
 * @LastEditors: letian
 * @LastEditTime: 2023-01-16 21:12
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
    lv_obj_set_pos(ui->bg, 0, 0);
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
    lv_obj_set_size(ui->bar, 250, 20); //15
    lv_obj_align(ui->bar, LV_ALIGN_CENTER, 0, 40);
    lv_bar_set_mode(ui->bar, LV_BAR_MODE_RANGE);
    lv_bar_set_value(ui->bar, 0, LV_ANIM_ON);

    ui->bar_img = lv_img_create(ui->boot);
    lv_obj_set_size(ui->bar_img, 60, 35);
    lv_obj_align_to(ui->bar_img, ui->bar,LV_ALIGN_OUT_TOP_LEFT, 0, 0);
    lv_img_set_src(ui->bar_img, &_rocket_60x35);

    obj_set_anim(ui->bar, 0, 100, 3000, (uint32_t)NULL, (lv_anim_exec_xcb_t)set_temp, NULL, lv_anim_path_ease_in);
    obj_set_anim(ui->bar_img, 0, 280-15-60, 3000, (uint32_t)NULL, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, lv_anim_path_ease_in);
    
    lv_obj_del_delayed(ui->boot, 3100);
}

static void scroll_event_cb(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if(diff_y >= r) {
            x = r;
        } else {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);   /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}

void setup_home_screen(lv_ui *ui)
{
    ui->home = lv_obj_create(ui->bg);
    lv_obj_set_size(ui->home, 280, 240);
    lv_obj_align(ui->home, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->home, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t * cont = lv_obj_create(ui->home);
    lv_obj_set_size(cont, 240, 240);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_event_cb(cont, scroll_event_cb, LV_EVENT_SCROLL, NULL);
    lv_obj_set_style_radius(cont, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(cont, true, 0);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

    uint32_t i;
    for(i = 0; i < 10; i++) {
        lv_obj_t * btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 170, 50);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "Button %d", i);
        lv_group_add_obj(guider_ui.group,btn);
    }

    lv_event_send(cont, LV_EVENT_SCROLL, NULL);
    lv_obj_scroll_to_view(lv_obj_get_child(cont, 0), LV_ANIM_OFF);

    obj_set_anim(ui->home, -280, 0, 1000, 4000, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, lv_anim_path_ease_in);
}

void setup_ui(lv_ui *ui)
{
    setup_bg_screen(ui);
    setup_boot_screen(ui);
    setup_home_screen(ui);
}


