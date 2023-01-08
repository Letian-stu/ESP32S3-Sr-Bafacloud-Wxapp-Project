/*
 * @Author: letian
 * @Date: 2022-11-30 15:38
 * @LastEditors: letian
 * @LastEditTime: 2022-12-31 16:28
 * @FilePath: \ESP32_Project\main\app_task\app_timer.c
 * @Description:
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */

#include "app_task.h"

#define TAG "timer"

TimerHandle_t Wifi_Config_Time_Handle;

static void Wifi_Config_Time_Callback(TimerHandle_t xTimer)
{
    xEventGroupSetBits(Event_Group, WIFITIMEOVER);
    ESP_LOGI(TAG, "Time Over");
    xTimerDelete(Wifi_Config_Time_Handle, 0);
}

void Times_Init(void)
{
    ESP_LOGI(TAG, "Init Timer");
    Wifi_Config_Time_Handle = xTimerCreate("Delaytime", pdMS_TO_TICKS(10000), pdFALSE, (void *)0, Wifi_Config_Time_Callback);
    xTimerStart(Wifi_Config_Time_Handle, 0);
}
