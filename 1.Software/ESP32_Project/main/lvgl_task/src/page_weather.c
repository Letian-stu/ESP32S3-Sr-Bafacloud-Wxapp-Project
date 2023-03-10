/*
 * @Author: letian
 * @Date: 2023-01-18 20:41
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-03-10 22:46
 * @FilePath: /ESP32_Project/main/lvgl_task/src/page_weather.c
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

#define TAG "PAGE_WEATHER"

void setup_weather_screen(lv_ui *ui, uint32_t time, uint32_t delay)
{
    ui->page = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui->page, 280, 240);
    lv_obj_align(ui->page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->page, LV_SCROLLBAR_MODE_OFF);

    ui->back_btn = lv_btn_create(ui->page);
    lv_obj_remove_style_all(ui->back_btn);                                                      // �Ƴ���ʽ
    lv_obj_set_size(ui->back_btn, 40, 40);                                                      // ���ô�С
    lv_obj_align(ui->back_btn, LV_ALIGN_TOP_LEFT, -10, -10);
    lv_obj_set_style_radius(ui->back_btn, 20, 0);                                               // ���õ�Բ��
    lv_obj_set_style_bg_color(ui->back_btn, lv_color_hex(COLOR_DODGER_BLUE), 0);                // ���ñ�����ɫ
    lv_obj_set_style_bg_opa(ui->back_btn, LV_OPA_0, 0);                                         // ����͸����
    lv_obj_set_style_bg_color(ui->back_btn, lv_color_hex(COLOR_DODGER_BLUE), LV_STATE_FOCUSED); // ���ñ��۽�ʱ���״̬��ɫ
    lv_obj_set_style_bg_opa(ui->back_btn, LV_OPA_50, LV_STATE_FOCUSED);                         // ���۽�ʱ���͸���ȣ��Ӷ�����������
    lv_obj_set_style_bg_opa(ui->back_btn, LV_OPA_80, LV_STATE_PRESSED);                         // ������ʱ��ı���͸����
    lv_obj_add_event_cb(ui->back_btn, lv_btn_back_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui->back_btn, lv_btn_back_event_cb, LV_EVENT_FOCUSED, NULL);
    ui->back = lv_img_create(ui->back_btn);
    lv_obj_set_size(ui->back, 40, 40);
    lv_obj_align(ui->back, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(ui->back, &_back_40x40);


    ui->httplabel = lv_label_create(ui->page);
    lv_obj_set_size(ui->httplabel, 270, 30);
    lv_obj_align(ui->httplabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_long_mode(ui->httplabel, LV_LABEL_LONG_SCROLL_CIRCULAR); 
    lv_obj_set_style_text_font(ui->httplabel, &myFont, 0);
    lv_label_set_text(ui->httplabel, "NULL");

    page_screen_anim(ui->page, -240, 0, time, delay, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}
