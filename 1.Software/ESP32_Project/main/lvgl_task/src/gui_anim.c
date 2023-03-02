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

    lv_anim_init(&a);//创建动画
    lv_anim_set_var(&a, obj);//动画对象
    lv_anim_set_values(&a, start, end);//动画的起始位置
    lv_anim_set_time(&a, time);//动画完成的时间
    lv_anim_set_delay(&a, delay);//延时多久
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)exec_cb);//从那个方向开始动画
    lv_anim_set_path_cb(&a, path_cb);//效果函数
    lv_anim_start(&a);//动画开始
}




