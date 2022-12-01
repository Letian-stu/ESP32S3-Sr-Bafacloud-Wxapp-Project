/*
 * @Author: letian
 * @Date: 2022-12-01 09:26
 * @LastEditors: letian
 * @LastEditTime: 2022-12-01 17:11
 * @FilePath: \ESP32_Project\main\mqtt_bafa\include\tcp_mqtt.h
 * @Description: 
 * Copyright (c) 2022 by letian 1656733965@qq.com, All Rights Reserved. 
 */
#ifndef _TCP_MQTT_H
#define _TCP_MQTT_H

#include <stdio.h>
#include <stdint.h>
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"
#include "mqtt_client.h"

#include "app_task.h"

#define TOPIC_LED "DriverLED002"
#define TOPIC_FAN "DriverFAN003"
#define TOPIC_KEY "DriverKEY006"
#define TOPIC_AHT "DriverAHT004"

typedef struct
{
    int topiclen;
    char topic[32];
    int datalen;
    char data[32];
}mqtt_data;

extern mqtt_data mqtt_buff;

void mqtt_app_start(void);



#endif
