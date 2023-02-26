/*
 * @Author: letian
 * @Date: 2022-11-30 22:22
 * @LastEditors: letian
 * @LastEditTime: 2023-02-07 20:24
 * @FilePath: \ESP32_Project\main\app_task\app_sem.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "app_task.h"

#define TAG "sem"

SemaphoreHandle_t SpeechMutex;
SemaphoreHandle_t KeyreadMutex;
SemaphoreHandle_t Reav_Mqtt_Buff_Handle;
SemaphoreHandle_t takepic_Handle;

void Sem_Init(void)
{
    SpeechMutex = xSemaphoreCreateMutex();
    KeyreadMutex = xSemaphoreCreateMutex();
    Reav_Mqtt_Buff_Handle = xSemaphoreCreateBinary();
    takepic_Handle = xSemaphoreCreateBinary();
}