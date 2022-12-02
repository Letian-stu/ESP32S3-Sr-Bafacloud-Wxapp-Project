/*
 * @Author: letian
 * @Date: 2022-11-29 13:57
 * @LastEditors: letian
 * @LastEditTime: 2022-12-02 16:08
 * @FilePath: \ESP32_Project\main\Init_Config\include\BaseConfig.h
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#ifndef _INITCONFIG_H
#define _INITCONFIG_H

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_vfs_fat.h"
#include "esp_spiffs.h"
#include "driver/spi_common.h"

//add by tian
#include "web_config.h"
#include "app_task.h"
#include "speech_if.h"


#define ESP_FS_PATH        "/fs"

#define ESP_WIFI_SSID      "WIFI_Config"
#define ESP_WIFI_PASS      "wificonfig"
#define ESP_WIFI_CHANNEL   1
#define MAX_STA_CONN       4

extern char readwifiname[32];
extern char readwifissid[32];

void Init_Config(void);
void wifi_init_softap(void);
void wifi_init_sta(_Bool Datafrom);
esp_err_t mount_storage(const char *base_path);

#endif
