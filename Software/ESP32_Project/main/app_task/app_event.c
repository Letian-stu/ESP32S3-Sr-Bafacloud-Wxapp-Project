/*
 * @Author: letian
 * @Date: 2022-11-30 20:20
 * @LastEditors: letian
 * @LastEditTime: 2022-11-30 20:22
 * @FilePath: \project\main\app_task\app_event.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "app_task.h"

#define TAG "event"

EventGroupHandle_t   Event_Group;

void Event_Init(void)
{
    Event_Group = xEventGroupCreate();

}

