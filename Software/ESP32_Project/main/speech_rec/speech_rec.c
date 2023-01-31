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
#include "tcp_mqtt.h"

#define BOARD_DMIC_I2S_SCK 16
#define BOARD_DMIC_I2S_SDO 17
#define BOARD_DMIC_I2S_WS 18

static const esp_mn_iface_t *g_multinet = &MULTINET_MODEL;
static model_iface_data_t *g_model_mn_data = NULL;
static const model_coeff_getter_t *g_model_mn_coeff_getter = &MULTINET_COEFF;

// static const esp_wn_iface_t *g_wakenet = &WAKENET_MODEL;
// static model_iface_data_t *g_model_wn_data = NULL;
// static const model_coeff_getter_t *g_model_wn_coeff_getter = &WAKENET_COEFF;

static const char *TAG = "speeech recognition";

typedef struct
{
    sr_cb_t fn; /*!< function */
    void *args; /*!< function args */
} func_t;

static func_t g_sr_callback_func[SR_CB_TYPE_MAX] = {0};

void i2s_init(void)
{
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX,        // the mode must be set according to DSP configuration
        .sample_rate = 16000,                         // must be the same as DSP configuration
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, // must be the same as DSP configuration
        .bits_per_sample = 32,                        // must be the same as DSP configuration
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,//I2S_COMM_FORMAT_I2S
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
// #ifdef WAKE
//     Initialize NN model
//     g_model_wn_data = g_wakenet->create(g_model_wn_coeff_getter, DET_MODE_95);
//     int wn_num = g_wakenet->get_word_num(g_model_wn_data);

//     for (int i = 1; i <= wn_num; i++)
//     {
//         char *name = g_wakenet->get_word_name(g_model_wn_data, i);
//         ESP_LOGI(TAG, "keywords: %s (index = %d)", name, i);
//     }

//     float wn_threshold = 0;
//     int wn_sample_rate = g_wakenet->get_samp_rate(g_model_wn_data);
//     int audio_wn_chunksize = g_wakenet->get_samp_chunksize(g_model_wn_data);
//     ESP_LOGI(TAG, "keywords_num = %d, threshold = %f, sample_rate = %d, chunksize = %d, sizeof_uint16 = %d", wn_num, wn_threshold, wn_sample_rate, audio_wn_chunksize, sizeof(int16_t));
// #endif

    g_model_mn_data = g_multinet->create(g_model_mn_coeff_getter, 4000);
    int audio_mn_chunksize = g_multinet->get_samp_chunksize(g_model_mn_data);
    int mn_num = g_multinet->get_samp_chunknum(g_model_mn_data);
    int mn_sample_rate = g_multinet->get_samp_rate(g_model_mn_data);
    ESP_LOGI(TAG, "keywords_num = %d , sample_rate = %d, chunksize = %d, sizeof_uint16 = %d", mn_num, mn_sample_rate, audio_mn_chunksize, sizeof(int16_t));

// #ifdef WAKE
//     int size = audio_wn_chunksize;

//     if (audio_mn_chunksize > audio_wn_chunksize)
//     {
//         size = audio_mn_chunksize;
//     }
// #else
    int size = audio_mn_chunksize;
// #endif

    int *buffer = (int *)malloc(size * 2 * sizeof(int));
    bool enable_wn = true;
    uint32_t mn_count = 0;
    i2s_init();

    size_t read_len = 0;
    printf("=====INIT OK=====\n");
    while (1)
    {
        i2s_read(1, buffer, size * 2 * sizeof(int), &read_len, portMAX_DELAY);
        for (int x = 0; x < size * 2 / 4; x++)
        {
            int s1 = ((buffer[x * 4] + buffer[x * 4 + 1]) >> 13) & 0x0000FFFF;
            int s2 = ((buffer[x * 4 + 2] + buffer[x * 4 + 3]) << 3) & 0xFFFF0000;
            buffer[x] = s1 | s2;
        }
// #define CHANGE_WAKE
#ifdef CHANGE_WAKE
        if (enable_wn)
        {
            int command_id = g_multinet->detect(g_model_mn_data, (int16_t *)buffer);
            if (command_id == 0) // ID0为唤醒词
            {
                ESP_LOGI(TAG, "Wake up id: %d", command_id);

                if (NULL != g_sr_callback_func[SR_CB_TYPE_WAKE].fn)
                {
                    g_sr_callback_func[SR_CB_TYPE_WAKE].fn(g_sr_callback_func[SR_CB_TYPE_WAKE].args);
                }
                enable_wn = false;
            }
            // else
            // {
            //     printf("please wake up");
            // }
        }
        else
        {
            mn_count++;
            int command_id = g_multinet->detect(g_model_mn_data, (int16_t *)buffer);
            if (command_id > -1) //识别到口令
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
                ESP_LOGW(TAG, "stop multinet");
                ESP_LOGI(TAG, "Listening time over");
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
        if (command_id > -1) //识别到口令
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
        // vTaskDelay(10/portTICK_PERIOD_MS);
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
    xTaskCreatePinnedToCore(recsrcTask, "recsrcTask", 8 * 1024, NULL, 8, NULL, 1);  

    //安装回调函数
    sr_handler_install(SR_CB_TYPE_WAKE, sr_wake, NULL);
    sr_handler_install(SR_CB_TYPE_CMD, sr_cmd, NULL);
    sr_handler_install(SR_CB_TYPE_CMD_EXIT, sr_cmd_exit, NULL);

    return ESP_OK;
}

static TaskHandle_t g_breath_light_task_handle = NULL;

static void breath_light_task(void *arg)
{
    printf("hi, my friend!\n");
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOGI(TAG, "Listening");
    }
}

//唤醒回调
void sr_wake(void *arg)
{
    /**< Turn on the breathing light */
    xTaskCreate(breath_light_task, "breath_light_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, &g_breath_light_task_handle);
}
//命令回调
void sr_cmd(void *arg)
{
    static int msg_id = 0;
    if (NULL != g_breath_light_task_handle)
    {
        vTaskDelete(g_breath_light_task_handle);
    }

    int32_t cmd_id = (int32_t)arg;
    uint32_t sendcount = 0;
    switch (cmd_id)
    {
    case 0:
        break;
    case 1:
        msg_id = esp_mqtt_client_publish(mqtt_client, "DriverLED002", "on", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        printf("open led\n");
        break;
    case 2:
        msg_id = esp_mqtt_client_publish(mqtt_client, "DriverLED002", "off", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        printf("close led\n");
        break;
    case 3:
        msg_id = esp_mqtt_client_publish(mqtt_client, "DriverFAN003", "on", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        printf("open fan\n");
        break;
    case 4:
        msg_id = esp_mqtt_client_publish(mqtt_client, "DriverFAN003", "off", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        printf("close fan\n");
        break;
    case 5:
        msg_id = esp_mqtt_client_publish(mqtt_client, "DriverKEY006", "on", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        printf("open button\n");
        break;
    case 6:
        msg_id = esp_mqtt_client_publish(mqtt_client, "DriverKEY006", "off", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        printf("close button\n");
        break;

    case 7:
        sendcount = 5;
        //xQueueSend(Key_Num_Queue, &sendcount, 100);  
        //printf("up\n");
        break;
    case 8:
        sendcount = 1; 
        //xQueueSend(Key_Num_Queue, &sendcount, 100);    
        //printf("down\n");
        break;
    case 9:
        sendcount = 9;
        //xQueueSend(Key_Num_Queue, &sendcount, 100);  
        //printf("sel\n");
        break;
    default:
        printf(" speeech recognition err\n");
        break;
    }
}
//结束回调
void sr_cmd_exit(void *arg)
{
    if (NULL != g_breath_light_task_handle)
    {
        vTaskDelete(g_breath_light_task_handle);
    }
}
