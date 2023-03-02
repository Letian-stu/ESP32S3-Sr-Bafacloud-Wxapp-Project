/*
 * @Author: letian
 * @Date: 2023-01-18 20:41
 * @LastEditors: error: git config user.name & please set dead value or install git
 * @LastEditTime: 2023-02-24 12:00
 * @FilePath: \ESP32_Project\main\lvgl_task\src\page_image.c
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
#include "BaseConfig.h"
#include "dirent.h"
#include <string.h>
#include <ctype.h>

lv_obj_t *img_pic;
char *buf;
static char file_name[64];

char *toLower(char *str, size_t len)
{
    char *str_l = calloc(len+1, sizeof(char));

    for (size_t i = 0; i < len; ++i) {
        str_l[i] = tolower((unsigned char)str[i]);
    }
    return str_l;
}

void lv_btn_img_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
        buf = toLower((char *)lv_list_get_btn_text(guider_ui.img_list, obj) , strlen(lv_list_get_btn_text(guider_ui.img_list, obj)) );
        //sd path
        sprintf(file_name, "S:/%s", buf);
        lv_img_set_src(img_pic, file_name);	
        break;
    case LV_EVENT_FOCUSED:
        // if( guider_ui.back_btn == obj )
        // {
        //     lv_label_set_text_fmt(guider_ui.label, "#ff0080 Back Btn #");
        //     printf("back btn\n");
        // }
        // else
        // {
        //     lv_label_set_text_fmt(guider_ui.label, "#ff0080 focused img:%s #", lv_list_get_btn_text(guider_ui.img_list, obj));
        //     printf("img name:%s\n", lv_list_get_btn_text(guider_ui.img_list, obj));
        // }

        if(lv_obj_get_parent(obj) == guider_ui.page)
        {
            printf("back btn\n");
            lv_label_set_text(guider_ui.label, "#ff0080 BackBtn #");
        }
        else if(lv_obj_get_parent(obj) == guider_ui.img_list)
        {
            printf("img name:%s\n", lv_list_get_btn_text(guider_ui.img_list, obj));
            lv_label_set_text_fmt(guider_ui.label, "#ff0080 focused img:%s #", lv_list_get_btn_text(guider_ui.img_list, obj));
        }
        
        break;
    default:
        break;
    }
}

void setup_image_screen(lv_ui *ui, uint32_t time, uint32_t delay)
{
    ui->page = lv_obj_create(lv_scr_act());   
    lv_obj_set_size(ui->page, 285, 245);
    lv_obj_align(ui->page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->page, LV_SCROLLBAR_MODE_OFF);

    ui->back_btn = lv_btn_create(ui->page);
    lv_obj_remove_style_all(ui->back_btn);                                                      // 移除样式
    lv_obj_set_size(ui->back_btn, 40, 40);                                                      // 设置大小
    lv_obj_align(ui->back_btn, LV_ALIGN_TOP_LEFT, -10, -5);
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
    lv_obj_align(ui->label, LV_ALIGN_TOP_MID, 0, 5);
    lv_label_set_text(ui->label, "image");

    ui->img_list = lv_list_create(ui->page);
    lv_obj_set_size(ui->img_list, 280, 180);
    lv_obj_align_to(ui->img_list, ui->label, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

    // lv_fs_dir_t img_dir;
    // lv_fs_res_t res;
    // res = lv_fs_dir_open(&img_dir, "S:/picture");
    // char filename[32];
    // while(1) 
    // {
    //     res = lv_fs_dir_read(&img_dir, filename);
    //     if(res != LV_FS_RES_OK) 
    //     {
    //         printf("read filename ERR");
    //     }
    //     else
    //     {
    //         printf("%s\n", filename);
    //     }
    //     if(strlen(filename) == 0) 
    //     {
    //         break;
    //     }
    //     ui->img_btn = lv_list_add_btn(ui->img_list, LV_SYMBOL_IMAGE, filename);
    //     lv_group_add_obj(ui->group, ui->img_btn);
    // }
    // lv_fs_dir_close(&img_dir);

    //sd path
    DIR *dir = opendir(ESP_SD_FS_PATH);
    if(dir == NULL)
    {
        printf("open err\n");
    }
    struct dirent *dirp;

    while((dirp = readdir(dir)) != NULL)
    {
        ui->img_btn = lv_list_add_btn(ui->img_list, LV_SYMBOL_IMAGE, dirp->d_name);
        lv_obj_add_event_cb(ui->img_btn, lv_btn_img_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui->img_btn, lv_btn_img_event_cb, LV_EVENT_FOCUSED, NULL);
        lv_group_add_obj(ui->group, ui->img_btn);
        //printf("child_id = %d \n", lv_obj_get_child_id(ui->img_btn));
    }
    //printf("get child %d \n", lv_obj_get_child_cnt(ui->img_list));
    img_pic = lv_img_create(ui->page);
    lv_obj_remove_style_all(img_pic);
    lv_obj_align(img_pic, LV_ALIGN_CENTER, 0, 0);

    ui->label = lv_label_create(img_pic);
    lv_label_set_recolor(ui->label, true);
    lv_obj_align(ui->label, LV_ALIGN_TOP_MID, 0, 5);

    page_screen_anim(ui->page, -240, 0, time, delay, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}
