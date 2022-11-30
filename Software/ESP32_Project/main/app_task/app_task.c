/*
 * @Author: letian
 * @Date: 2022-11-30 15:38
 * @LastEditors: letian
 * @LastEditTime: 2022-11-30 21:53
 * @FilePath: \project\main\app_task\app_task.c
 * @Description:
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */
#include "app_task.h"

#define TAG "task"

TaskHandle_t while_Handle;
TaskHandle_t Change_Sta_Task;

void While_Task(void *p)
{
    static EventBits_t EventValue;
    while (1)
    {
        EventValue = xEventGroupWaitBits(Event_Group, ALLEVENT, pdTRUE, pdFALSE, portMAX_DELAY);
        switch (EventValue)
        {
        case WIFITIMEOVER:
            httpd_stop(server);
            esp_wifi_stop();
            esp_wifi_deinit();
            wifi_init_sta(0);
            ESP_LOGI(TAG, "Time over ap to sta form nvsdata");
            break;
        case CONFIGWIFIOK:
            httpd_stop(server);
            esp_wifi_stop();
            esp_wifi_deinit();
            wifi_init_sta(1);
            ESP_LOGI(TAG, "Time over ap to sta form webconfig");
            break;
        case CONFIGWIFITIMEDEL:
            xTimerStop(Wifi_Config_Time_Handle,0);
            ESP_LOGI(TAG, "config wifi stop time");
            break;
        default:
            break;
        }
    }
}


void Tasks_Init(void)
{
    ESP_LOGI(TAG, "Init Task");
    xTaskCreate(While_Task, "while", 1024 * 4, (void *)NULL, 1, &while_Handle);
}
