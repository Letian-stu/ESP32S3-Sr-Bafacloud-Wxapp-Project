/*
 * @Author: letian
 * @Date: 2022-11-29 14:19
 * @LastEditors: Letian-stu
 * @Description:
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */ 
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "BaseConfig.h"

#define TAG "main"

/**
 * @description: main
 * @return {*}
 */
void app_main(void)
{
    ESP_LOGI(TAG, "HELLO TIAN");
    Init_Config();
    vTaskDelay(1000/portTICK_PERIOD_MS);
    size_t ram = heap_caps_get_free_size(MALLOC_CAP_DMA);
    ESP_LOGW(TAG, "dram=%dkb", ram/1024);
    ram = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    ESP_LOGW(TAG, "psram=%dkb", ram/1024);
}

