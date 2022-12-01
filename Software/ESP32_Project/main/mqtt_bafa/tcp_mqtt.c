/*
 * @Author: letian
 * @Date: 2022-12-01 09:25
 * @LastEditors: letian
 * @LastEditTime: 2022-12-01 17:12
 * @FilePath: \ESP32_Project\main\mqtt_bafa\tcp_mqtt.c
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#include "tcp_mqtt.h"

#define TAG "mqtt"

mqtt_data mqtt_buff;

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
        msg_id = esp_mqtt_client_subscribe(client, "DriverLED002", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "DriverKEY006", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "DriverFAN003", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "DriverAHT004", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);        
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        msg_id = esp_mqtt_client_unsubscribe(client, "DriverLED002");
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);        
        msg_id = esp_mqtt_client_unsubscribe(client, "DriverKEY006");
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);        
        msg_id = esp_mqtt_client_unsubscribe(client, "DriverFAN003");
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);        
        msg_id = esp_mqtt_client_unsubscribe(client, "DriverAHT004");
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        // msg_id = esp_mqtt_client_publish(client, "DriverLED002", "SUBSCRIBED", 0, 0, 0);
        //ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        //ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        //ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        mqtt_buff.topiclen = mqtt_event->topic_len;
        mqtt_buff.datalen = mqtt_event->data_len;
        strncpy(mqtt_buff.topic,mqtt_event->topic,mqtt_event->topic_len );
        strncpy(mqtt_buff.data,mqtt_event->data,mqtt_event->data_len );

        xSemaphoreGive(Reav_Mqtt_Buff_Handle);
        // printf("==========================\r\n");
        // printf("TOPIC=%.*s\r\n", mqtt_event->topic_len, mqtt_event->topic);
        // printf("DATA=%.*s\r\n", mqtt_event->data_len, mqtt_event->data);
        // printf("==========================\r\n");

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
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}



