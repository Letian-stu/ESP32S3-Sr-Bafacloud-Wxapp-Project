/*
 * @Author: letian
 * @Date: 2023-01-18 20:43
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-03-05 22:44
 * @FilePath: \ESP32_Project\main\lvgl_task\src\page_sd.c
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
#include "esp_spiffs.h"
#include "esp_log.h"
#include "esp_wifi.h"

#define TAG "PAGE_SD"

void setup_sd_screen(lv_ui *ui, uint32_t time, uint32_t delay)
{
    ui->page = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui->page, 280, 240);
    lv_obj_align(ui->page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->page, LV_SCROLLBAR_MODE_OFF);

    ui->back_btn = lv_btn_create(ui->page);
    lv_obj_remove_style_all(ui->back_btn); // 移除样式
    lv_obj_set_size(ui->back_btn, 40, 40); // 设置大小
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

    ui->label = lv_label_create(ui->page);
    lv_obj_align(ui->label, LV_ALIGN_TOP_MID, 0, 10);
    lv_label_set_text(ui->label, "RAM Used");

    ui->dram = lv_arc_create(ui->page);
    lv_obj_set_size(ui->dram, 100, 100);
    lv_obj_remove_style(ui->dram, NULL, LV_PART_KNOB);
    lv_obj_align(ui->dram, LV_ALIGN_CENTER, -40 - 20, 0);
    lv_arc_set_rotation(ui->dram, 135);
    lv_arc_set_bg_angles(ui->dram, 0, 270);
    size_t dram = heap_caps_get_free_size(MALLOC_CAP_DMA) / 512;
    lv_arc_set_value(ui->dram, (uint16_t)(100-(dram/5.12)) );
    ui->label = lv_label_create(ui->dram);
    lv_obj_align(ui->label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_align(ui->label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text_fmt(ui->label, "dram\n%d%%",(uint16_t)(100-(dram/5.12)));

    ui->psram = lv_arc_create(ui->page);
    lv_obj_set_size(ui->psram, 100, 100);
    lv_obj_remove_style(ui->psram, NULL, LV_PART_KNOB);
    lv_obj_align(ui->psram, LV_ALIGN_CENTER, 40 + 20, 0);
    lv_arc_set_rotation(ui->psram, 135);
    lv_arc_set_bg_angles(ui->psram, 0, 270);

    size_t psram = heap_caps_get_free_size(MALLOC_CAP_8BIT) / 1024;
    lv_arc_set_value(ui->psram, (uint16_t)(100-(psram/20.48)) );
    ui->label = lv_label_create(ui->psram);
    lv_obj_align(ui->label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_align(ui->label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text_fmt(ui->label, "psram\n%d%%",(uint16_t)(100-(psram/20.48)));

    tcpip_adapter_ip_info_t ipaddr;
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA,&ipaddr);
    ui->iplabel = lv_label_create(ui->page);
    lv_obj_align(ui->iplabel, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_label_set_text_fmt(ui->iplabel, "get web from %d.%d.%d.%d", IP2STR(&ipaddr.ip));

    page_screen_anim(ui->page, -240, 0, time, delay, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}
