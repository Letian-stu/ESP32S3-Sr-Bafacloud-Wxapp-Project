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




//task
void Tasks_Init(void);

//timer
extern TimerHandle_t Wifi_Config_Time_Handle;
void Times_Init(void);

//sem

//queue

//event
extern EventGroupHandle_t   Event_Group;
#define WIFITIMEOVER (1<<0)
#define CONFIGWIFITIMEDEL (1<<1)
#define CONFIGWIFIOK (1<<2)
#define ALLEVENT ( WIFITIMEOVER | CONFIGWIFITIMEDEL | CONFIGWIFIOK)
void Event_Init(void);

#endif

