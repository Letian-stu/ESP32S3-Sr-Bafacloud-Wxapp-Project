/*
 * @Author: letian
 * @Date: 2022-11-29 13:57
 * @LastEditors: letian
 * @LastEditTime: 2023-02-02 19:25
 * @FilePath: \ESP32_Project\main\Init_Config\initconfig.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "BaseConfig.h"
#include "cam_task.h"

#define TAG "init"

uint8_t readnamelen = 0;
char readwifiname[32]={'\0'};
uint8_t readssidlen = 0;
char readwifissid[32]={'\0'};

#define READMODE(p) (p == WIFI_MODE_STA) ? "STA" : ( (p == WIFI_MODE_AP) ? "AP" : "APSTA")

static int get_ip(int argc, char **argv)
{
    static tcpip_adapter_ip_info_t local_ip;
    static wifi_mode_t wifimode;
    uint8_t ret = esp_wifi_get_mode(&wifimode);
    if (ret == ESP_OK)
    {
        ESP_LOGI("cmd", "wifi mode is %s", READMODE(wifimode) );
    }
    ret = tcpip_adapter_get_ip_info( !(wifimode % 2)  , &local_ip);
    if (ret == ESP_OK)
    {
        ESP_LOGI("cmd", "GET_IP:" IPSTR "", IP2STR(&local_ip.ip));
    }

    return 0;
}

static void register_cmd_ip(void)
{
    const esp_console_cmd_t cmd_ip = {
        .command = "ip",
        .help = "Get if addr",
        .hint = NULL,
        .func = &get_ip,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd_ip) );
}

static void Init_Cmd(void)
{
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    /* Prompt to be printed before each line.
     * This can be customized, made dynamic, etc.
     */
    repl_config.prompt = CONFIG_IDF_TARGET ">";
    repl_config.max_cmdline_length = 1024;
    /* Register commands */
    esp_console_register_help_command();
    register_system();
    register_wifi();
    register_nvs();
    register_cmd_ip();
    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
    ESP_LOGI(TAG, "console register success");
}

static esp_err_t Read_wifi_from_nvs(void)
{
    esp_err_t err = nvs_open("nvs", NVS_READWRITE, &wifi_config);
    if (err != ESP_OK)
    {
        return ESP_FAIL;
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        //read
        ESP_LOGI(TAG, "=========nvs read data==========");
        err = nvs_get_u8(wifi_config, WIFINAMELEN, &readnamelen);
        if(err != ESP_OK)
            ESP_LOGE(TAG, "read len Failed!");
        err = nvs_get_str(wifi_config, WIFINAME, readwifiname, (size_t*)(&readnamelen));
        if(err != ESP_OK)
            ESP_LOGE(TAG, "read name Failed!");
        err = nvs_get_u8(wifi_config, WIFISSIDLEN, &readssidlen);
        if(err != ESP_OK)
        
            ESP_LOGE(TAG, "read len Failed!");
        err = nvs_get_str(wifi_config, WIFISSID, readwifissid, (size_t*)(&readssidlen));
        if(err != ESP_OK)
            ESP_LOGE(TAG, "read ssid Failed!");
        ESP_LOGI(TAG, "wifiname:len=%d,data=%s",readnamelen,readwifiname);
        ESP_LOGI(TAG, "wifissid:len=%d,data=%s",readssidlen,readwifissid);
        ESP_LOGI(TAG, "=======================================");
        // Close
        nvs_close(wifi_config);
    }
    return ESP_OK;
}

void Init_Config(void)
{
    ESP_LOGI(TAG,"Start Init");

    Event_Init();
    Sem_Init();
    Queue_Init();
    Times_Init();

    nvs_flash_init();
    Read_wifi_from_nvs();
    
    speech_recognition_init();
    cam_config_init(); 
    xTaskCreatePinnedToCore(appguiTask, "App_Gui",  1024 * 8, NULL, 2, NULL, 1);

    wifi_init_softap();  
    mount_storage(ESP_FS_PATH);
    start_wifi_config_server(ESP_FS_PATH);

    Tasks_Init();

    ESP_LOGI(TAG,"Start Succrss");
}


