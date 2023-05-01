/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "xtensa/core-macros.h"
#include "esp_partition.h"
#include "driver/i2s.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_wn_iface.h"
#include "esp_wn_models.h"
#include "dl_lib_coefgetter_if.h"
#include "esp_mn_iface.h"
#include "esp_mn_models.h"
#include "speech_if.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "lvgl.h"

static const char *TAG = "sr";

/**
 * @description:ledc config
 */
#define LEDC_GPIO (36)
#define LEDC_MAX_DUTY (8000)
#define LEDC_MIN_DUTY (0)
#define LEDC_FADE_TIME (1000)

ledc_channel_config_t ledc_channel = {
    .channel = LEDC_CHANNEL_0,
    .duty = 0,
    .gpio_num = LEDC_GPIO,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .hpoint = 0,
    .timer_sel = LEDC_TIMER_1,
    .flags.output_invert = 0
};

void LEDC_init(void)
{
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_LOW_SPEED_MODE,    // timer mode
        .timer_num = LEDC_TIMER_1,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,             // Auto select the source clock
    };
    ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);
    ledc_fade_func_install(0);

}

/**
 * @description:sr config
 */
#define BOARD_DMIC_I2S_SCK 16
#define BOARD_DMIC_I2S_SDO 17
#define BOARD_DMIC_I2S_WS 18

static const esp_mn_iface_t *g_multinet = &MULTINET_MODEL;
static model_iface_data_t *g_model_mn_data = NULL;
static const model_coeff_getter_t *g_model_mn_coeff_getter = &MULTINET_COEFF;
static func_t g_sr_callback_func[SR_CB_TYPE_MAX] = {0};
static TaskHandle_t g_breath_light_task_handle = NULL;

void i2s_init(void)
{
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX,             // the mode must be set according to DSP configuration
        .sample_rate = 16000,                              // must be the same as DSP configuration
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,      // must be the same as DSP configuration
        .bits_per_sample = 32,                             // must be the same as DSP configuration
        .communication_format = I2S_COMM_FORMAT_STAND_I2S, // I2S_COMM_FORMAT_I2S
        .dma_buf_count = 3,
        .dma_buf_len = 300,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = BOARD_DMIC_I2S_SCK, // IIS_SCLK
        .ws_io_num = BOARD_DMIC_I2S_WS,   // IIS_LCLK
        .data_out_num = -1,               // IIS_DSIN
        .data_in_num = BOARD_DMIC_I2S_SDO // IIS_DOUT
    };
    i2s_driver_install(1, &i2s_config, 0, NULL);
    i2s_set_pin(1, &pin_config);
    i2s_zero_dma_buffer(1);
}

void recsrcTask(void *arg)
{
    g_model_mn_data = g_multinet->create(g_model_mn_coeff_getter, 4000);
    int audio_mn_chunksize = g_multinet->get_samp_chunksize(g_model_mn_data);
    int mn_num = g_multinet->get_samp_chunknum(g_model_mn_data);
    int mn_sample_rate = g_multinet->get_samp_rate(g_model_mn_data);
    ESP_LOGI(TAG, "keywords_num = %d , sample_rate = %d, chunksize = %d, sizeof_uint16 = %d", mn_num, mn_sample_rate, audio_mn_chunksize, sizeof(int16_t));

    int size = audio_mn_chunksize;
    int *buffer = (int *)malloc(size * 2 * sizeof(int));
    bool enable_wn = true;
    uint32_t mn_count = 0;
    i2s_init();
    LEDC_init();
    size_t read_len = 0;
    while (1)
    {
        i2s_read(1, buffer, size * 2 * sizeof(int), &read_len, portMAX_DELAY);
        for (int x = 0; x < size * 2 / 4; x++)
        {
            int s1 = ((buffer[x * 4] + buffer[x * 4 + 1]) >> 13) & 0x0000FFFF;
            int s2 = ((buffer[x * 4 + 2] + buffer[x * 4 + 3]) << 3) & 0xFFFF0000;
            buffer[x] = s1 | s2;
        }
#define CHANGE_WAKE
#ifdef CHANGE_WAKE
        if (enable_wn)
        {
            int command_id = g_multinet->detect(g_model_mn_data, (int16_t *)buffer);
            if (command_id == 0)
            {
                ESP_LOGI(TAG, "Wake up id: %d", command_id);
                if (NULL != g_sr_callback_func[SR_CB_TYPE_WAKE].fn)
                {
                    g_sr_callback_func[SR_CB_TYPE_WAKE].fn(g_sr_callback_func[SR_CB_TYPE_WAKE].args);
                }
                enable_wn = false;
            }
        }
        else
        {
            mn_count++;
            int command_id = g_multinet->detect(g_model_mn_data, (int16_t *)buffer);
            if (command_id > -1) 
            {
                ESP_LOGI(TAG, "MN test successfully, Commands ID: %d", command_id);
                if (NULL != g_sr_callback_func[SR_CB_TYPE_CMD].fn)
                {
                    if (NULL != g_sr_callback_func[SR_CB_TYPE_CMD].args)
                    {
                        g_sr_callback_func[SR_CB_TYPE_CMD].fn(g_sr_callback_func[SR_CB_TYPE_CMD].args);
                    }
                    else
                    {
                        g_sr_callback_func[SR_CB_TYPE_CMD].fn((void *)command_id);
                    }
                }
                enable_wn = true;
                mn_count = 0;
            }
            else
            {
            }
            if (mn_count == mn_num)
            {
                ESP_LOGW(TAG, "stop multinet, Listening time over");
                if (NULL != g_sr_callback_func[SR_CB_TYPE_CMD_EXIT].fn)
                {
                    g_sr_callback_func[SR_CB_TYPE_CMD_EXIT].fn(g_sr_callback_func[SR_CB_TYPE_CMD_EXIT].args);
                }
                enable_wn = true;
                mn_count = 0;
            }
        }
#else
        int command_id = g_multinet->detect(g_model_mn_data, (int16_t *)buffer);
        if (command_id > -1) 
        {
            ESP_LOGI(TAG, "MN test successfully, Commands ID: %d", command_id);
            if (NULL != g_sr_callback_func[SR_CB_TYPE_CMD].fn)
            {
                if (NULL != g_sr_callback_func[SR_CB_TYPE_CMD].args)
                {
                    g_sr_callback_func[SR_CB_TYPE_CMD].fn(g_sr_callback_func[SR_CB_TYPE_CMD].args);
                }
                else
                {
                    g_sr_callback_func[SR_CB_TYPE_CMD].fn((void *)command_id);
                }
            }
            enable_wn = true;
            mn_count = 0;
        }
        else
        {
        }
#endif
    }
    vTaskDelete(NULL);
}

esp_err_t sr_handler_install(sr_cb_type_t type, sr_cb_t handler, void *args)
{
    switch (type)
    {
    case SR_CB_TYPE_WAKE:
        g_sr_callback_func[SR_CB_TYPE_WAKE].fn = handler;
        g_sr_callback_func[SR_CB_TYPE_WAKE].args = args;
        break;

    case SR_CB_TYPE_CMD:
        g_sr_callback_func[SR_CB_TYPE_CMD].fn = handler;
        g_sr_callback_func[SR_CB_TYPE_CMD].args = args;
        break;

    case SR_CB_TYPE_CMD_EXIT:
        g_sr_callback_func[SR_CB_TYPE_CMD_EXIT].fn = handler;
        g_sr_callback_func[SR_CB_TYPE_CMD_EXIT].args = args;
        break;

    default:
        return ESP_ERR_INVALID_ARG;
        break;
    }

    return ESP_OK;
}

esp_err_t speech_recognition_init(void)
{
    xTaskCreatePinnedToCore(recsrcTask, "recsrcTask", 8 * 1024, NULL, 10, NULL, 1);

    sr_handler_install(SR_CB_TYPE_WAKE, sr_wake, NULL);
    sr_handler_install(SR_CB_TYPE_CMD, sr_cmd, NULL);
    sr_handler_install(SR_CB_TYPE_CMD_EXIT, sr_cmd_exit, NULL);

    return ESP_OK;
}



static void breath_light_task(void *arg)
{
    ESP_LOGI(TAG, "hi, my friend!");
    while (1)
    {
        // printf("2. LEDC fade down to duty = %d\n", LEDC_MIN_DUTY);
        ledc_set_fade_with_time(ledc_channel.speed_mode, ledc_channel.channel, LEDC_MIN_DUTY, LEDC_FADE_TIME);
        ledc_fade_start(ledc_channel.speed_mode, ledc_channel.channel, LEDC_FADE_NO_WAIT);

        // printf("1. LEDC fade up to duty = %d\n", LEDC_MAX_DUTY);
        ledc_set_fade_with_time(ledc_channel.speed_mode, ledc_channel.channel, LEDC_MAX_DUTY, LEDC_FADE_TIME);
        ledc_fade_start(ledc_channel.speed_mode, ledc_channel.channel, LEDC_FADE_NO_WAIT);
    }
}

// lv_obj_t *imggif;

void sr_wake(void *arg)
{
    /**< Turn on the breathing light */
    xTaskCreate(breath_light_task, "breath_light_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, &g_breath_light_task_handle);
}

void sr_cmd(void *arg)
{
    int msg_id = 0;
    if (g_breath_light_task_handle != NULL)
    {
        vTaskDelete(g_breath_light_task_handle);
    }
    int32_t cmd_id = (int32_t)arg;
    switch (cmd_id)
    {

    case 1:
        
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        // printf("open led\n");
        break;
    case 2:

        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        // printf("close led\n");
        break;
    case 3:

        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        // printf("open fan\n");
        break;
    case 4:

        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        // printf("close fan\n");
        break;
    case 5:
    
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        // printf("open button\n");
        break;
    case 6:

        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        // printf("close button\n");
        break;
    case 7:

        break;
    case 8:

        break;
    case 9:

        break;
    case 10:
        printf(" tui xia ba\n");

        break;    
    default:
        printf(" speeech recognition err\n");
        break;
    }
}

void sr_cmd_exit(void *arg)
{
    if (g_breath_light_task_handle != NULL)
    {
        vTaskDelete(g_breath_light_task_handle);
    }
}
