/*
 * @Author: letian
 * @Date: 2022-11-29 13:57
 * @LastEditors: letian
 * @LastEditTime: 2022-12-02 17:29
 * @FilePath: \ESP32_Project\main\Init_Config\initconfig.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "BaseConfig.h"

#define TAG "init"

uint8_t readnamelen = 0;
char readwifiname[32]={'\0'};
uint8_t readssidlen = 0;
char readwifissid[32]={'\0'};

void Init_Config(void)
{
    ESP_LOGI(TAG,"Start Init");
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_err_t err = nvs_open("nvs", NVS_READWRITE, &wifi_config);
    if (err != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        //read
        ESP_LOGI(TAG, "=========nvs read data==========");
        err = nvs_get_u8(wifi_config, WIFINAMELEN, &readnamelen);
        if(err != ESP_OK)
            ESP_LOGE(TAG, "read len Failed!\n");
        err = nvs_get_str(wifi_config, WIFINAME, readwifiname, (size_t*)(&readnamelen));
        if(err != ESP_OK)
            ESP_LOGE(TAG, "read name Failed!\n");
        err = nvs_get_u8(wifi_config, WIFISSIDLEN, &readssidlen);
        if(err != ESP_OK)
            ESP_LOGE(TAG, "read len Failed!\n");
        err = nvs_get_str(wifi_config, WIFISSID, readwifissid, (size_t*)(&readssidlen));
        if(err != ESP_OK)
            ESP_LOGE(TAG, "read ssid Failed!\n");
        ESP_LOGI(TAG, "wifiname:len=%d,data=%s",readnamelen,readwifiname);
        ESP_LOGI(TAG, "wifissid:len=%d,data=%s",readssidlen,readwifissid);
        ESP_LOGI(TAG, "=======================================");
        // Close
        nvs_close(wifi_config);
    }

    wifi_init_softap();
    mount_storage(ESP_FS_PATH);
    start_wifi_config_server(ESP_FS_PATH);
    
    //start os task
    Event_Init();
    Sem_Init();
    Queue_Init();
    Times_Init();
    Tasks_Init();
    speech_recognition_init();

    ESP_LOGI(TAG,"Start Succrss");
}


