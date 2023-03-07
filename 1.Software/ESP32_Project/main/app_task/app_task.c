/*
 * @Author: letian
 * @Date: 2022-12-04 17:10
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-03-06 13:51
 * @FilePath: \ESP32_Project\main\app_task\app_task.c
 * @Description:
 * Copyright (c) 2023 by letian 1656733975@qq.com, All Rights Reserved.
 */
#include "app_task.h"

#define TAG "task"

TaskHandle_t Mqtt_Handle;
TaskHandle_t AHT_Handle;
TaskHandle_t KeyScan_Handle;
TaskHandle_t Cam_Handle;

void AHT_Task(void *p)
{
    float H, T;
    static int msg_id = 0;
    char Send_mqtt_buff[20];
    I2cMaster_handle_t i2c_0 = I2cMaster_Init(I2C_NUM_0, 41, 42, 400000);
    AHT20_handle_t aht20 = AHT20_Init(i2c_0, AHT20_ADDR);
    int count = 10;
    while (1)
    {
        vTaskDelay(6000 / portTICK_PERIOD_MS);
        vTaskDelay(6000 / portTICK_PERIOD_MS);
        vTaskDelay(6000 / portTICK_PERIOD_MS);
        vTaskDelay(6000 / portTICK_PERIOD_MS);
        vTaskDelay(6000 / portTICK_PERIOD_MS);
        AHT20_GetRawData(aht20);
        AHT20_StandardUnitCon(aht20, &H, &T);
        ESP_LOGI(TAG, "====aht20 send mqtt buff====");
        // ESP_LOGI(TAG, "temperature = %d humidity = %d \n", (uint16_t)T, (uint16_t)H);
        sprintf(Send_mqtt_buff, "#%d#%d", (uint16_t)T, (uint16_t)H);
        if (guider_ui.labeltemp != NULL && guider_ui.labelhumi != NULL)
        {
            lv_label_set_text_fmt(guider_ui.labeltemp, "温度\n%d C", (uint16_t)T);
            lv_label_set_text_fmt(guider_ui.labelhumi, "湿度\n%dRH", (uint16_t)H);
        }

        ESP_LOGI(TAG, "send mqtt buff:%s", Send_mqtt_buff);
        msg_id = esp_mqtt_client_publish(mqtt_client, "DriverAHT004", Send_mqtt_buff, 0, 1, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        ESP_LOGI(TAG, "============================");
    }
}

void Mqtt_Task(void *p)
{
    BaseType_t err;
    mqtt_app_start();
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
                if(guider_ui.ledbtn != NULL)
                {
                    lv_obj_add_state(guider_ui.ledbtn, LV_STATE_CHECKED);
                }
                Driver_state.led = 1;
                // ESP_LOGI(TAG, "recv led buff on");
            }
            else if (!strncmp(mqtt_buff.data, "off", mqtt_buff.datalen))
            {
                if(guider_ui.ledbtn != NULL)
                {
                    lv_obj_clear_state(guider_ui.ledbtn, LV_STATE_CHECKED);
                }
                Driver_state.led = 0;
                // ESP_LOGI(TAG, "recv led buff off");
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
                if(guider_ui.fanbtn != NULL)
                {
                    lv_obj_add_state(guider_ui.fanbtn, LV_STATE_CHECKED);
                }                
                Driver_state.fan = 1;
                // ESP_LOGI(TAG, "recv fan buff on");
            }
            else if (!strncmp(mqtt_buff.data, "off", mqtt_buff.datalen))
            {
                if(guider_ui.fanbtn != NULL)
                {
                    lv_obj_clear_state(guider_ui.fanbtn, LV_STATE_CHECKED);
                }                   
                Driver_state.fan = 0;
                // ESP_LOGI(TAG, "recv fan buff off");
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
                if(guider_ui.keybtn != NULL)
                {
                    lv_obj_add_state(guider_ui.keybtn, LV_STATE_CHECKED);
                }   
                Driver_state.key = 1;
                // ESP_LOGI(TAG, "recv key buff on");
            }
            else if (!strncmp(mqtt_buff.data, "off", mqtt_buff.datalen))
            {
                if(guider_ui.keybtn != NULL)
                {
                    lv_obj_clear_state(guider_ui.keybtn, LV_STATE_CHECKED);
                }   
                Driver_state.key = 0;
                // ESP_LOGI(TAG, "recv key buff off");
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
    xTaskCreate(KEYScan_Task, "Key_Scan", 1024 * 8, NULL, 5, &KeyScan_Handle);
    xTaskCreate(cam_show_task, "cam_task", 1024 * 8, NULL, 4, &Cam_Handle);
    vTaskSuspend(Cam_Handle);

    xTaskCreate(AHT_Task, "AHT", 1024 * 4, NULL, 1, &AHT_Handle);
    vTaskSuspend(AHT_Handle);
    xTaskCreate(Mqtt_Task, "Mqtt", 1024 * 4, NULL, 3, &Mqtt_Handle);
    vTaskSuspend(Mqtt_Handle);
}
