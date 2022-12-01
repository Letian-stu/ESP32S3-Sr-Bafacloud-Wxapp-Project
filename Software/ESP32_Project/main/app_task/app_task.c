/*
 * @Author: letian
 * @Date: 2022-11-30 15:38
 * @LastEditors: letian
 * @LastEditTime: 2022-12-01 18:39
 * @FilePath: \ESP32_Project\main\app_task\app_task.c
 * @Description:
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */
#include "app_task.h"

#define TAG "task"

TaskHandle_t while_Handle;
TaskHandle_t Mqtt_Handle;

void While_Task(void *p)
{
    static EventBits_t EventValue;
    esp_err_t err;
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
            mqtt_app_start();
            ESP_LOGI(TAG, "Time over ap to sta form nvsdata");
            break;
        case CONFIGWIFIOK:
            httpd_stop(server);
            esp_wifi_stop();
            esp_wifi_deinit();
            wifi_init_sta(1);
            mqtt_app_start();
            ESP_LOGI(TAG, "Time over ap to sta form webconfig");
            break;
        case CONFIGWIFITIMEDEL:

            err = xTimerDelete(Wifi_Config_Time_Handle, 0);
            if (err == pdFALSE)
            {
                ESP_LOGE(TAG, "Timer del err");
            }
            // xTimerDelete(Wifi_Config_Time_Handle,0);
            ESP_LOGI(TAG, "config wifi stop time");
            break;
        default:
            break;
        }
    }
}

void Mqtt_Task(void *p)
{
    BaseType_t err;
    while (1)
    {
        err = xSemaphoreTake(Reav_Mqtt_Buff_Handle, portMAX_DELAY);
        if (err == pdTRUE)
        {
            ESP_LOGI(TAG, "=======================");
            ESP_LOGI(TAG, " topic:len=%d data=%s", mqtt_buff.topiclen,mqtt_buff.topic);
            ESP_LOGI(TAG, " data:len=%d data=%s", mqtt_buff.datalen, mqtt_buff.data);
            ESP_LOGI(TAG, "=======================");
        }
        if (!strcmp( mqtt_buff.topic, TOPIC_LED))
        {
            if (!strncmp(mqtt_buff.data, "on", mqtt_buff.datalen))
            {
                ESP_LOGI(TAG, "recv led buff on");
            }
            else if (!strncmp(mqtt_buff.data, "off", mqtt_buff.datalen))
            {
                ESP_LOGI(TAG, "recv led buff off");
            }
            else
            {
                ESP_LOGI(TAG, "buff err");
            }
        }
        if (!strcmp(mqtt_buff.topic, TOPIC_FAN))
        {
            if (!strncmp(mqtt_buff.data, "on", mqtt_buff.datalen))
            {
                ESP_LOGI(TAG, "recv fan buff on");
            }
            else if (!strncmp(mqtt_buff.data, "off", mqtt_buff.datalen))
            {
                ESP_LOGI(TAG, "recv fan buff off");
            }
            else
            {
                ESP_LOGI(TAG, "buff err");
            }
        }
        if (!strcmp(mqtt_buff.topic, TOPIC_KEY))
        {
            if (!strncmp(mqtt_buff.data, "on", mqtt_buff.datalen))
            {
                ESP_LOGI(TAG, "recv key buff on");
            }
            else if (!strncmp(mqtt_buff.data, "off", mqtt_buff.datalen))
            {
                ESP_LOGI(TAG, "recv key buff off");
            }
            else
            {
                ESP_LOGI(TAG, "buff err");
            }
        }
    }
}

void Tasks_Init(void)
{
    ESP_LOGI(TAG, "Init Task");
    xTaskCreate(While_Task, "while", 1024 * 4, (void *)NULL, 1, &while_Handle);
    xTaskCreate(Mqtt_Task, "Mqtt", 1024 * 4, (void *)NULL, 1, &Mqtt_Handle);
}
