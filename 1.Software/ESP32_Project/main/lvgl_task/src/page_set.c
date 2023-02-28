/*
 * @Author: letian
 * @Date: 2023-01-18 20:41
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-02-28 22:21
 * @FilePath: \ESP32_Project\main\lvgl_task\src\page_set.c
 * @Description: 
 * Copyright (c) 2023 by letian 1656733975@qq.com, All Rights Reserved. 
 */
#include <stdio.h>
#include "string.h"
#include "lvgl/lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gui_guider.h"
#include "gui_anim.h"

static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
    }
}

void setup_set_screen(lv_ui *ui, uint32_t time, uint32_t delay)
{
    ui->page = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui->page, 280, 240);
    lv_obj_align(ui->page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->page, LV_SCROLLBAR_MODE_OFF);

    ui->back_btn = lv_btn_create(ui->page);
    lv_obj_remove_style_all(ui->back_btn);                                                      // 移除样式
    lv_obj_set_size(ui->back_btn, 40, 40);                                                      // 设置大小
    lv_obj_align(ui->back_btn, LV_ALIGN_TOP_LEFT, -10, -10);
    lv_obj_set_style_radius(ui->back_btn, 20, 0);                                               // 设置倒圆角
    lv_obj_set_style_bg_color(ui->back_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);                // 设置背景颜色
    lv_obj_set_style_bg_opa(ui->back_btn, LV_OPA_0, 0);                                         // 背景透明度
    lv_obj_set_style_bg_color(ui->back_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED); // 设置被聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->back_btn, LV_OPA_50, LV_STATE_FOCUSED);                         // 被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->back_btn, LV_OPA_80, LV_STATE_PRESSED);                         // 被按下时候的背景透明度
    lv_obj_add_event_cb(ui->back_btn, lv_btn_back_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui->back_btn, lv_btn_back_event_cb, LV_EVENT_FOCUSED, NULL);
    ui->back = lv_img_create(ui->back_btn);
    lv_obj_set_size(ui->back, 40, 40);
    lv_obj_align(ui->back, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(ui->back, &_back_40x40);

    ui->imgtemp = lv_img_create(ui->page);
    lv_obj_set_pos(ui->imgtemp, 5, 40);
    lv_img_set_src(ui->imgtemp, &_temperature_70x70);
    ui->labeltemp = lv_label_create(ui->page);
    lv_obj_align_to(ui->labeltemp, ui->imgtemp, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
    lv_label_set_text_fmt(ui->labeltemp, "temp");

    ui->imghumi = lv_img_create(ui->page);
    lv_obj_set_pos(ui->imghumi, 175, 40);
    lv_img_set_src(ui->imghumi, &_humi_70x70);
    ui->labelhumi = lv_label_create(ui->page);
    lv_obj_align_to(ui->labelhumi, ui->imghumi, LV_ALIGN_OUT_LEFT_TOP, 0, 0);
    lv_label_set_text_fmt(ui->labelhumi, "humi");

    ui->imgled = lv_img_create(ui->page);
    lv_obj_set_pos(ui->imgled, 100, 120);
    lv_img_set_src(ui->imgled, &_led_50x50);

    ui->ledbtn = lv_switch_create(ui->page);
    lv_obj_align_to(ui->ledbtn, ui->imgled, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(ui->ledbtn, event_handler, LV_EVENT_ALL, NULL);

    ui->imgkey = lv_img_create(ui->page);
    lv_obj_set_pos(ui->imgkey, 30, 120);
    lv_img_set_src(ui->imgkey, &_key_50x50);
    
    ui->keybtn = lv_switch_create(ui->page);
    lv_obj_align_to(ui->keybtn, ui->imgkey, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(ui->keybtn, event_handler, LV_EVENT_ALL, NULL);

    ui->imgfan = lv_img_create(ui->page);
    lv_obj_set_pos(ui->imgfan, 170, 120);
    lv_img_set_src(ui->imgfan, &_fan_50x50);

    ui->fanbtn = lv_switch_create(ui->page);
    lv_obj_align_to(ui->fanbtn, ui->imgfan, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(ui->fanbtn, event_handler, LV_EVENT_ALL, NULL);

    page_screen_anim(ui->imgtemp, -85, 40, time, delay+800, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
    page_screen_anim(ui->imghumi, -85, 40, time, delay+800, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
    page_screen_anim(ui->imgled, 240, 120, time, delay+800, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
    page_screen_anim(ui->imgkey, -60, 30, time, delay+800, (lv_anim_exec_xcb_t)lv_obj_set_x, lv_anim_path_bounce);
    page_screen_anim(ui->imgfan, 280, 170, time, delay+800, (lv_anim_exec_xcb_t)lv_obj_set_x, lv_anim_path_bounce);
    page_screen_anim(ui->page, -240, 0, time, delay, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}

