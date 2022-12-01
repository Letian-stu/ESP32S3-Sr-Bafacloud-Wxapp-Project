#include "app_task.h"

#define TAG "sem"

SemaphoreHandle_t Reav_Mqtt_Buff_Handle;

void Sem_Init(void)
{
    Reav_Mqtt_Buff_Handle = xSemaphoreCreateBinary();
}