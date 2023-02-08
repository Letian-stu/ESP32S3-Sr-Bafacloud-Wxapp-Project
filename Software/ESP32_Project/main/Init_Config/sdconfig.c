/*
 * @Author: letian
 * @Date: 2022-11-29 14:57
 * @LastEditors: letian
 * @LastEditTime: 2023-02-08 16:51
 * @FilePath: \ESP32_Project\main\Init_Config\sdconfig.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "BaseConfig.h"

#define TAG "sd"

#define PIN_NUM_MISO 40
#define PIN_NUM_MOSI 38
#define PIN_NUM_CLK 39
#define PIN_NUM_CS 37

esp_err_t sdcard_init(const char* base_path)
{
   // initialize the sdcard
    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    sdmmc_card_t *card;
    ESP_LOGI(TAG, "Initializing SD card");

    sdmmc_host_t host = {
        .flags = SDMMC_HOST_FLAG_SPI | SDMMC_HOST_FLAG_DEINIT_ARG, 
        .slot = SPI2_HOST, 
        .max_freq_khz = SDMMC_FREQ_DEFAULT, 
        .io_voltage = 3.3f, 
        .init = &sdspi_host_init, 
        .set_bus_width = NULL, 
        .get_bus_width = NULL, 
        .set_bus_ddr_mode = NULL, 
        .set_card_clk = &sdspi_host_set_card_clk, 
        .do_transaction = &sdspi_host_do_transaction, 
        .deinit_p = &sdspi_host_remove_device, 
        .io_int_enable = &sdspi_host_io_int_enable, 
        .io_int_wait = &sdspi_host_io_int_wait, 
        .command_timeout_ms = 0, 
    };
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize bus.");
        return ESP_FAIL;
    }
    sdspi_device_config_t slot_config = {
    .host_id   = SPI2_HOST,  
    .gpio_cs   = GPIO_NUM_13, 
    .gpio_cd   = SDSPI_SLOT_NO_CD, 
    .gpio_wp   = SDSPI_SLOT_NO_WP, 
    .gpio_int  = GPIO_NUM_NC, 
    };
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;
    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(base_path, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "sd card Filesystem failed");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "sd card Filesystem mounted");
    sdmmc_card_print_info(stdout, card);
    return ESP_OK;
}

