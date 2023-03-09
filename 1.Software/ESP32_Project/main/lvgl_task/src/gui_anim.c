/*
 * @Author: letian
 * @Date: 2023-01-10 10:51
 * @LastEditors: letian
 * @LastEditTime: 2023-01-20 21:52
 * @FilePath: \ESP32_Project\main\lvgl_task\src\gui_anim.c
 * @Description: 
 * Copyright (c) 2023 by letian 1656733975@qq.com, All Rights Reserved. 
 */
#include <stdio.h>
#include "string.h"
#include "gui_anim.h"


void page_screen_anim(lv_obj_t *obj, int32_t start, int32_t end, uint32_t time, uint32_t delay, lv_anim_exec_xcb_t exec_cb,lv_anim_path_cb_t path_cb)
{
    lv_anim_t a;

    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, start, end);
    lv_anim_set_time(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)exec_cb);
    lv_anim_set_path_cb(&a, path_cb);
    lv_anim_start(&a);
}




