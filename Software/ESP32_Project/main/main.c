/*
 * @Author: letian
 * @Date: 2022-11-29 14:19
 * @LastEditors: letian
 * @LastEditTime: 2023-01-27 19:07
 * @FilePath: \ESP32_Project\main\main.c
 * @Description:
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "BaseConfig.h"

#define TAG "main"

void app_main(void)
{
    ESP_LOGI(TAG, "HELLO TIAN");
    Init_Config();
    // 获取剩余的dram大小
    size_t dram = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    ESP_LOGE(TAG, "DRAM=%dkb  ", dram/1024);
}
