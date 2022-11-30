/*
 * @Author: letian
 * @Date: 2022-11-30 15:38
 * @LastEditors: letian
 * @LastEditTime: 2022-11-30 22:19
 * @FilePath: \project\main\app_task\app_timer.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */

#include "app_task.h"

#define TAG "timer"

TimerHandle_t Wifi_Config_Time_Handle;

static void Wifi_Config_Time_Callback(TimerHandle_t xTimer)
{
    static uint8_t timer = 20;
    if (timer > 0)
    {
        timer--;
        ESP_LOGI(TAG, "ap mode ramain tims is %d s",timer);
        if(timer <= 0)
        {
            xEventGroupSetBits(Event_Group, WIFITIMEOVER);
        }
    }
}

void Times_Init(void)
{
    ESP_LOGI(TAG, "Init Timer");
    Wifi_Config_Time_Handle = xTimerCreate("Delaytime", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, Wifi_Config_Time_Callback);
    xTimerStart(Wifi_Config_Time_Handle,0);
}
