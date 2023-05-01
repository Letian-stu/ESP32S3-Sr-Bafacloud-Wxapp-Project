/*
 * @Author: StuTian
 * @Date: 2022-09-03 22:14
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-05-01 10:15
 * @FilePath: /ESP32_Project/main/lvgl_task/src/lvgl_init.c
 * @Description:
 * Copyright (c) 2022 by StuTian 1656733975@qq.com, All Rights Reserved.
 */
#include "lvgl_init.h"
#include "lv_fs_if.h"

/******************************************************************
 *  STATIC PROTOTYPES
 ******************************************************************/
static const char *TAG = "lvgl";

lv_ui guider_ui;

void lv_tick_task(void *arg)
{
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

#define HOWMUCH 80

void appguiTask(void *p)
{
    lv_init();
    lvgl_driver_init();
    lv_fs_if_init();
    lv_color_t *buf1 = heap_caps_malloc(LV_HOR_RES_MAX * HOWMUCH, MALLOC_CAP_DMA);
    assert(buf1 != NULL);
    lv_color_t *buf2 = heap_caps_malloc(LV_HOR_RES_MAX * HOWMUCH, MALLOC_CAP_DMA);
    assert(buf2 != NULL);
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, LV_HOR_RES_MAX * HOWMUCH / 2);
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.offset_x = 20;
    // disp_drv.offset_y = ;
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"};
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
    lv_port_indev_init();
    ESP_LOGI(TAG, "Start APP UI Init");
    setup_ui(&guider_ui);
    while (1)
    {
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
