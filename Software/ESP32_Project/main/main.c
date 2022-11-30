/*
 * @Author: letian
 * @Date: 2022-11-29 14:19
 * @LastEditors: letian
 * @LastEditTime: 2022-11-30 16:51
 * @FilePath: \project\main\main.c
 * @Description:
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// add by tian
#include "BaseConfig.h"

#define TAG "main"

void app_main(void)
{
    ESP_LOGI(TAG, "HELLO TIAN");
    Init_Config();
}
