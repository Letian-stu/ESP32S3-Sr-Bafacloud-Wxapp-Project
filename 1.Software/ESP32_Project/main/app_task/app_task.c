/*
 * @Author: letian
 * @Date: 2022-12-04 17:10
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-03-11 23:16
 * @FilePath: \ESP32_Project\main\app_task\app_task.c
 * @Description:
 * Copyright (c) 2023 by letian 1656733975@qq.com, All Rights Reserved.
 */
#include "app_task.h"

#define TAG "task"

TaskHandle_t KeyScan_Handle;

void KEYScan_Task(void *p)
{
    Button_Init();
    while (1)
    {
        if (pdTRUE == xSemaphoreTake(KeyreadMutex, portMAX_DELAY))
        {
            Button_Process();
            xSemaphoreGive(KeyreadMutex);
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}


void Tasks_Init(void)
{
    xTaskCreatePinnedToCore(KEYScan_Task,   "Key",   1024 * 8, NULL, 8, &KeyScan_Handle, 1);
}
