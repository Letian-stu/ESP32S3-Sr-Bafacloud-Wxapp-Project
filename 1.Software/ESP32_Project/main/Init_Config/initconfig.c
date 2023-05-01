/*
 * @Author: letian
 * @Date: 2022-11-29 13:57
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-05-01 10:51
 * @FilePath: /ESP32_Project/main/Init_Config/initconfig.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "BaseConfig.h"

#define TAG "init"

void Init_Config(void)
{
    ESP_LOGI(TAG,"Start Init");
    Event_Init();
    Sem_Init();
    Queue_Init();
    Times_Init();
    nvs_flash_init();
    sdcard_init(ESP_SD_FS_PATH);
    speech_recognition_init();
    xTaskCreatePinnedToCore(appguiTask, "App_Gui",  1024 * 8, NULL, 7, NULL, 1);
    wifi_ap_sta_init();
    mount_storage(ESP_FS_PATH);
    Tasks_Init();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG,"Init Succrss");
}


