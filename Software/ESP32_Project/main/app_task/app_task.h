/*
 * @Author: letian
 * @Date: 2022-11-30 22:22
 * @LastEditors: letian
 * @LastEditTime: 2023-01-29 20:58
 * @FilePath: \ESP32_Project\main\app_task\include\app_task.h
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#ifndef _APP_TASK_H
#define _APP_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_wifi.h"
#include "esp_log.h"

//add by tian
#include "BaseConfig.h"
#include "web_config.h"
#include "tcp_mqtt.h"
#include "speech_if.h"
#include "i2c_master.h"
#include "aht20_driver.h"
#include "button.h"
#include "lvgl_init.h"
#include "cam_task.h"


#define AHT20_ADDR 0x38

//task
extern TaskHandle_t Cam_Handle;
extern TaskHandle_t Mqtt_Handle;
extern TaskHandle_t AHT_Handle;
void Tasks_Init(void);

//timer
// extern TimerHandle_t Wifi_Config_Time_Handle;
void Times_Init(void);

//sem
extern SemaphoreHandle_t Reav_Mqtt_Buff_Handle;
extern SemaphoreHandle_t SpeechMutex;
extern SemaphoreHandle_t KeyreadMutex;
void Sem_Init(void);
//queue

void Queue_Init(void);
//event
extern EventGroupHandle_t   Event_Group;
#define WIFITIMEOVER (1<<0)
#define CONFIGWIFITIMEDEL (1<<1)
#define CONFIGWIFIOK (1<<2)
#define ALLEVENT ( WIFITIMEOVER | CONFIGWIFITIMEDEL | CONFIGWIFIOK)
void Event_Init(void);

#endif

