/*
 * @Author: letian
 * @Date: 2022-11-30 22:22
 * @LastEditors: letian
 * @LastEditTime: 2022-12-01 10:30
 * @FilePath: \ESP32_Project\main\app_task\app_queue.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "app_task.h"

#define TAG "queue"

QueueHandle_t Key_Num_Queue;

void Queue_Init(void)
{
    Key_Num_Queue = xQueueCreate(2, sizeof(uint8_t));    
}

