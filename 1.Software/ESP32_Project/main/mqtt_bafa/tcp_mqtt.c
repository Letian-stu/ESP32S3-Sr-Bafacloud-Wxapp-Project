/*
 * @Author: letian
 * @Date: 2022-12-01 09:25
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-03-06 13:17
 * @FilePath: \ESP32_Project\main\mqtt_bafa\tcp_mqtt.c
 * @Description:
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved.
 */
#include "tcp_mqtt.h"

#define TAG "mqtt"

mqtt_data_t mqtt_buff;
LEDFANKEY_t Driver_state;
esp_mqtt_client_handle_t mqtt_client;

void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0)
    {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t mqtt_event;
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event = event_data;
    esp_mqtt_client_handle_t client = mqtt_event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        esp_mqtt_client_publish(client, "DriverLED002", "off", 0, 0, 0);
        esp_mqtt_client_publish(client, "DriverKEY006", "off", 0, 0, 0);
        esp_mqtt_client_publish(client, "DriverFAN003", "off", 0, 0, 0);
        msg_id = esp_mqtt_client_subscribe(client, "DriverLED002", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "DriverKEY006", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "DriverFAN003", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "DriverAHT004", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        break;
    case MQTT_EVENT_SUBSCRIBED:
        // msg_id = esp_mqtt_client_publish(client, "DriverLED002", "SUBSCRIBED", 0, 0, 0);
        // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        // ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        // ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        mqtt_buff.topiclen = mqtt_event->topic_len;
        mqtt_buff.datalen = mqtt_event->data_len;
        strncpy(mqtt_buff.topic, mqtt_event->topic, mqtt_event->topic_len);
        strncpy(mqtt_buff.data, mqtt_event->data, mqtt_event->data_len);
        mqtt_buff.topic[mqtt_buff.topiclen] = '\0';
        mqtt_buff.data[mqtt_buff.datalen]   = '\0';

        // printf("len:%d top:%s\n", mqtt_buff.topiclen, mqtt_buff.topic);
        // printf("len:%d top:%s\n", mqtt_buff.datalen, mqtt_buff.data);

        xSemaphoreGive(Reav_Mqtt_Buff_Handle);

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (mqtt_event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            log_error_if_nonzero("reported from esp-tls", mqtt_event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", mqtt_event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno", mqtt_event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(mqtt_event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG, "Other mqtt_event id:%d", mqtt_event->event_id);
        break;
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .client_id = "6bf959ef20e34a08a876c765108576a8",
        .uri = "mqtt://bemfa.com:9501",
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}
