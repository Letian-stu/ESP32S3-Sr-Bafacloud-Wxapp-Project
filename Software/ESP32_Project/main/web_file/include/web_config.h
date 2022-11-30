/*
 * @Author: letian
 * @Date: 2022-11-30 09:21
 * @LastEditors: letian
 * @LastEditTime: 2022-11-30 10:42
 * @FilePath: \project\main\web_file\include\web_config.h
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#ifndef _WEB_CONFIG
#define _WEB_CONFIG

#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include "esp_err.h"
#include "esp_log.h"

#include "cJSON.h"

#include "nvs_flash.h"
#include "nvs.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_http_server.h"

#define WIFINAMELEN "WIFINAMELEN"
#define WIFINAME    "WIFINAME"
#define WIFISSIDLEN "WIFISSIDLEN"
#define WIFISSID    "WIFISSID"

extern nvs_handle_t wifi_config;
esp_err_t start_wifi_config_server(const char *base_path);

#endif

