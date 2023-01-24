/*
 * @Author: letian
 * @Date: 2022-11-30 15:38
 * @LastEditors: letian
 * @LastEditTime: 2023-01-24 21:32
 * @FilePath: \ESP32_Project\main\app_task\app_task.c
 * @Description:
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */
#include "app_task.h"

#define TAG "task"
#define AHT20_ADDR 0x38

TaskHandle_t WifiSet_Handle;
TaskHandle_t Mqtt_Handle;
TaskHandle_t AHT_Handle;
TaskHandle_t KeyScan_Handle;

void AHT_Task(void *p)
{
    float H, T;
    static int msg_id = 0;
    char Send_mqtt_buff[20];
    I2cMaster_handle_t i2c_0 = I2cMaster_Init(I2C_NUM_0, 41, 42, 400000);
    AHT20_handle_t aht20 = AHT20_Init(i2c_0, AHT20_ADDR);
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        int count = 12;
        while (count--)
        {
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
        AHT20_GetRawData(aht20);
        AHT20_StandardUnitCon(aht20, &H, &T);
        ESP_LOGI(TAG, "====aht20 send mqtt buff====");
        // ESP_LOGI(TAG, "temperature = %d humidity = %d \n", (uint16_t)T, (uint16_t)H);
        sprintf(Send_mqtt_buff, "#%d#%d", (uint16_t)T, (uint16_t)H);
        ESP_LOGI(TAG, "send mqtt buff:%s", Send_mqtt_buff);
        msg_id = esp_mqtt_client_publish(mqtt_client, "DriverAHT004", Send_mqtt_buff, 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        ESP_LOGI(TAG, "============================");
    }
}

void WifiSet_Task(void *p)
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
            ESP_LOGI(TAG, "=====MQTT Recv Buff=====");
            ESP_LOGI(TAG, " topic:len=%d data=%s", mqtt_buff.topiclen, mqtt_buff.topic);
            ESP_LOGI(TAG, " data:len=%d data=%s", mqtt_buff.datalen, mqtt_buff.data);
            ESP_LOGI(TAG, "========================");
        }
        if (!strcmp(mqtt_buff.topic, TOPIC_LED))
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
    ESP_LOGI(TAG, "Init Task");
    //xTaskCreate(AHT_Task,       "AHT",              1024 * 4, NULL, 1, &AHT_Handle);
    //xTaskCreate(WifiSet_Task,   "WifiSet",          1024 * 4, NULL, 1, &WifiSet_Handle);
    //xTaskCreate(Mqtt_Task,      "Mqtt",             1024 * 4, NULL, 1, &Mqtt_Handle);
    xTaskCreatePinnedToCore(appguiTask, "App_Gui",  1024 * 8, NULL, 4, NULL, 1);
    xTaskCreate(KEYScan_Task,   "Key_Scan",         1024 * 6, NULL, 5, &KeyScan_Handle);
}


