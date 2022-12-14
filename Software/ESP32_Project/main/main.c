/*
 * @Author: letian
 * @Date: 2022-11-29 14:19
 * @LastEditors: letian
 * @LastEditTime: 2023-01-11 16:50
 * @FilePath: \ESP32_Project\main\main.c
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

void Menu_Switch(void)
{
    xQueueSend(Key_Num_Queue, &Button_Value, 30);
    Button_Value = 0;
}

void app_main(void)
{
    ESP_LOGI(TAG, "HELLO TIAN");
    Init_Config();
    while(1)
    {
        Menu_Switch();
    }
}
