/*
 * @Author: letian
 * @Date: 2022-11-30 22:22
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-05-01 10:35
 * @FilePath: /ESP32_Project/main/app_task/app_sem.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "app_task.h"

#define TAG "sem"

SemaphoreHandle_t KeyreadMutex;

void Sem_Init(void)
{
    KeyreadMutex = xSemaphoreCreateMutex();
}