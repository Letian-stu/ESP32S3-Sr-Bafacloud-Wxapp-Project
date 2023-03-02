/*
 * @Author: letian
 * @Date: 2022-12-04 17:10
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-03-01 20:21
 * @FilePath: \ESP32_Project\main\mqtt_bafa\tcp_mqtt.h
 * @Description: 
 * Copyright (c) 2023 by letian 1656733975@qq.com, All Rights Reserved. 
 */
/*
 * @Author: letian
 * @Date: 2022-12-01 09:26
 * @LastEditors: letian
 * @LastEditTime: 2022-12-02 17:59
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

#define OPEN 1
#define CLONE 0

typedef struct
{
    int topiclen;
    char topic[32];
    int datalen;
    char data[32];
}mqtt_data_t;

typedef struct
{
    bool led;
    bool fan;
    bool key;
}LEDFANKEY_t;

extern esp_mqtt_client_handle_t mqtt_client;
extern mqtt_data_t mqtt_buff;
extern LEDFANKEY_t Driver_state;
void mqtt_app_start(void);



#endif
