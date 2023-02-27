/*
 * @Author: letian
 * @Date: 2023-01-18 20:41
 * @LastEditors: letian
 * @LastEditTime: 2023-02-07 20:08
 * @FilePath: \ESP32_Project\main\lvgl_task\src\page_camera.c
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
#include "cam_task.h"
#include "app_task.h"


#define TAG "page_cam"

void lv_btn_takepic_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
        ESP_LOGI(TAG, "Taking picture...");
        xSemaphoreGive(takepic_Handle);
        break;
    case LV_EVENT_FOCUSED:
    default:
        break;
    }
}

void setup_camera_screen(lv_ui *ui, uint32_t time, uint32_t delay)
{
    ui->page = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(ui->page);  
    lv_obj_set_size(ui->page, 280, 240);
    lv_obj_align(ui->page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(ui->page, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(ui->page, lv_color_hex(COLOR_DARK_ORANGE), 0);
    lv_obj_set_scrollbar_mode(ui->page, LV_SCROLLBAR_MODE_OFF);

    ui->img_cam = lv_img_create(ui->page);
    lv_obj_remove_style_all(ui->img_cam);  
    lv_obj_align(ui->img_cam, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui->img_cam, 280, 240);

    ui->back_btn = lv_btn_create(ui->img_cam);
    lv_obj_remove_style_all(ui->back_btn);                                                      // 移除样式
    lv_obj_set_size(ui->back_btn, 40, 40);                                                      // 设置大小
    lv_obj_align(ui->back_btn, LV_ALIGN_TOP_LEFT, 10, 10);
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

    ui->takepic_btn = lv_btn_create(ui->img_cam);
    lv_obj_remove_style_all(ui->takepic_btn);                                                      // 移除样式
    lv_obj_set_size(ui->takepic_btn, 50, 40);                                                      // 设置大小
    lv_obj_align(ui->takepic_btn, LV_ALIGN_RIGHT_MID, -20, 0);
    lv_obj_set_style_radius(ui->takepic_btn, 5, 0);                                                // 设置倒圆角
    lv_obj_set_style_bg_color(ui->takepic_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);                // 设置背景颜色
    lv_obj_set_style_bg_opa(ui->takepic_btn, LV_OPA_0, 0);                                         // 背景透明度
    lv_obj_set_style_bg_color(ui->takepic_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED); // 设置被聚焦时候的状态颜色
    lv_obj_set_style_bg_opa(ui->takepic_btn, LV_OPA_50, LV_STATE_FOCUSED);                         // 被聚焦时候的透明度，从而让人眼区分
    lv_obj_set_style_bg_opa(ui->takepic_btn, LV_OPA_80, LV_STATE_PRESSED);                         // 被按下时候的背景透明度
    lv_obj_add_event_cb(ui->takepic_btn, lv_btn_takepic_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui->takepic_btn, lv_btn_takepic_event_cb, LV_EVENT_FOCUSED, NULL);
    
    ui->takepic = lv_img_create(ui->takepic_btn);
    lv_obj_set_size(ui->takepic, 50, 50);
    lv_obj_align(ui->takepic, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(ui->takepic, &_takepic_50x50);

    vTaskResume(Cam_Handle);

    page_screen_anim(ui->page, -240, 0, time, delay, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}
