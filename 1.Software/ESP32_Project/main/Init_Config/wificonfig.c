/*
 * @Author: letian
 * @Date: 2022-11-29 14:28
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-05-01 10:44
 * @FilePath: /ESP32_Project/main/Init_Config/wificonfig.c
 * @Description:
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */
#include "BaseConfig.h"

#define TAG "ap_sta"

read_wifi_buf_t read_wifi_buf;

sta_ap_wifi_config_t sta_ap_wifi_config = {
    .sta = {
        .ssid = "home",
        .password = "1656733975",
    },
    .ap = {
        .ssid = "Wifi_Config",
        .password = "wificonfig",
        .max_connection = 1,
        .authmode = WIFI_AUTH_WPA_WPA2_PSK,
    },
};

static void wifi_sta_ap_cb(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) 
    {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",MAC2STR(event->mac), event->aid);
    } 
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) 
    {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",MAC2STR(event->mac), event->aid);
    }
    else if(event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP IPv4 address: %d.%d.%d.%d", IP2STR(&event->ip_info.ip));
    }
    else if(event_id == WIFI_EVENT_STA_DISCONNECTED)   
    {
        wifi_event_sta_disconnected_t *disconnected_event = (wifi_event_sta_disconnected_t *)event_data;
        ESP_LOGE(TAG, "Disconnect reason :%d ",disconnected_event->reason);
        static int count = 10;
        esp_err_t err;
        ESP_LOGI(TAG, "Wi-Fi disconnected, trying to reconnect...");
        if (count > 0) 
        {
            err = esp_wifi_connect();
            if (err == ESP_ERR_WIFI_NOT_STARTED) {
                return;
            }
            count--;
        } 
        else 
        {
            ESP_LOGE(TAG, "try max connect count, but failed");
        }
    }
}

esp_err_t wifi_ap_sta_init(void)
{
    static tcpip_adapter_ip_info_t local_ip;
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));

    esp_netif_config_t netif_config =  {                                                
        .base = ESP_NETIF_BASE_DEFAULT_WIFI_AP,      
        .driver = NULL,                              
        .stack = ESP_NETIF_NETSTACK_DEFAULT_WIFI_AP, 
    };
    esp_netif_t *netif = esp_netif_new(&netif_config);
    esp_netif_attach_wifi_ap(netif);

    netif_config.base = ESP_NETIF_BASE_DEFAULT_WIFI_STA;
    netif_config.stack = ESP_NETIF_NETSTACK_DEFAULT_WIFI_STA;
    netif = esp_netif_new(&netif_config);
    esp_netif_attach_wifi_station(netif);

    esp_wifi_set_default_wifi_sta_handlers(); 

    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_AP_STACONNECTED, &wifi_sta_ap_cb, NULL);
    esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_AP_STADISCONNECTED, &wifi_sta_ap_cb, NULL);
    esp_wifi_set_config(ESP_IF_WIFI_AP, (wifi_config_t*)&sta_ap_wifi_config.ap);

    esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_sta_ap_cb, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_sta_ap_cb, NULL);
    esp_wifi_set_config(ESP_IF_WIFI_STA, (wifi_config_t*)&sta_ap_wifi_config.sta);

    esp_wifi_connect();
    uint8_t ret = tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &local_ip);
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "wificonfig IP:" IPSTR "", IP2STR(&local_ip.ip));
    }
    return ESP_OK;
}
