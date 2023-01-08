/*
 * @Author: StuTian
 * @Date: 2022-09-05 14:07
 * @LastEditors: letian
 * @LastEditTime: 2022-12-26 19:17
 * @FilePath: \lvgl_demo\main\lvgl_task\src\gui_guider.c
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

static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_set_size(var, v, v);
}

static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t *obj = lv_event_get_target(e);

    switch (code)
    {
    case LV_EVENT_CLICKED:
        printf("CLICKED\n");

        lv_obj_t * obj = lv_obj_create(guider_ui.bg);
        lv_obj_set_style_bg_color(obj,lv_color_make(0xff, 0xff, 0xff) , 0);
        lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);
        lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
        
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_values(&a, 0, 240);
        lv_anim_set_time(&a, 200);
        lv_anim_set_exec_cb(&a, anim_size_cb);
        lv_anim_start(&a);

        break;
    default:
        break;
    }
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

void setup_scr_screen(lv_ui *ui)
{
    ui->bg = lv_obj_create(NULL);
    lv_obj_set_pos(ui->bg, 0, 0);
    lv_obj_set_size(ui->bg, 241, 241);

    lv_obj_t * cont = lv_obj_create(ui->bg);
    lv_obj_set_size(cont, 241, 241);
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
        lv_obj_add_event_cb(btn, event_handler, LV_EVENT_ALL, NULL);
        lv_obj_set_width(btn, lv_pct(90));
        lv_obj_set_height(btn, lv_pct(25));
        lv_group_add_obj(ui->group, btn);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "Button %"LV_PRIu32, i);
    }

    /*Update the buttons position manually for first*/
    lv_event_send(cont, LV_EVENT_SCROLL, NULL);
    /*Be sure the fist button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(cont, 0), LV_ANIM_OFF);

}

void setup_ui(lv_ui *ui)
{
    setup_scr_screen(ui);
    lv_scr_load(ui->bg);
}
