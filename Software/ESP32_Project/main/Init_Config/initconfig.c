/*
 * @Author: letian
 * @Date: 2022-11-29 13:57
 * @LastEditors: letian
 * @LastEditTime: 2022-11-29 15:07
 * @FilePath: \sample_project\main\Init_Config\InitConfig.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "BaseConfig.h"

#define TAG "INIT"

void Init_Config(void)
{
    ESP_LOGI(TAG,"Start Init");
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_softap();

    mount_storage(ESP_FS_PATH);

    ESP_LOGI(TAG,"Start Succrss");
}


