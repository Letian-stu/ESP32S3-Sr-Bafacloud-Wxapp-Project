/**
  * @file           aht20_driver.c
  * @version        1.0
  * @date           2021-5-27
  */

#include "aht20_driver.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "AHT20";

#define AHT20_HANDLE_CHECK(a, ret)  if (NULL == a) {                             \
        ESP_LOGE(TAG, "%s (%d) driver handle is NULL.", __FUNCTION__, __LINE__); \
        return (ret);                                                            \
        }

/**
  * @brief  get device status word.
  * @param  aht20_handle  aht20 operation handle pointer.
  * @retval status word value.
  */
static uint8_t aht20_ReadStatuCmd(AHT20_handle_t aht20_handle)
{
    uint8_t tmp = 0x00;
	  I2cMaster_ReadReg(aht20_handle->i2c_handle, aht20_handle->i2c_addr, 
                      AHT20_STATUS_REG, &tmp, 1);
	return tmp;
}

/**
  * @brief  read calibration enable bit.
  * @param  aht20_handle  aht20 operation handle pointer.
  * @retval  - 1  calibrated
  *          - 0  not calibrated
  */
static inline uint8_t aht20_ReadCalEnableCmd(AHT20_handle_t aht20_handle)
{
	  uint8_t tmp = 0;
	  tmp = aht20_ReadStatuCmd(aht20_handle);
	  return (tmp>>3)&0x01;
}

/**
  * @brief  read device busy bit.
  * @param  aht20_handle  aht20 operation handle pointer.
  * @retval  - 1  device busy.
  *          - 0  device not busy.
  */
static inline uint8_t aht20_ReadBusyCmd(AHT20_handle_t aht20_handle)
{
	  uint8_t tmp = 0;
	  tmp = aht20_ReadStatuCmd(aht20_handle);
	  return (tmp>>7)&0x01;
}

/**
  * @brief  AHT20 send initialized command.
  * @param  aht20_handle  aht20 operation handle pointer.
  * @retval void
  */
static inline void aht20_IcInitCmd(AHT20_handle_t aht20_handle)
{
	  uint8_t tmp[2] = {0};
	  tmp[0] = 0x08;
	  tmp[1] = 0x00;
    I2cMaster_WriteReg(aht20_handle->i2c_handle, aht20_handle->i2c_addr, 
                       AHT20_INIT_REG, tmp, 2);
}

/**
  * @brief  AHT20 send begin messure command.
  * @param  aht20_handle  aht20 operation handle pointer.
  * @retval void
  */
static inline void aht20_TrigMeasureCmd(AHT20_handle_t aht20_handle)
{
	  uint8_t tmp[2] = {0};
	  tmp[0] = 0x33;
	  tmp[1] = 0x00;
    I2cMaster_WriteReg(aht20_handle->i2c_handle, aht20_handle->i2c_addr, 
                       AHT20_TrigMeasure_REG, tmp, 2);
}

/**
  * @brief  AHT20 send soft reset command.
  * @param  aht20_handle  aht20 operation handle pointer.
  * @retval void
  */
static inline void aht20_SoftResetCmd(AHT20_handle_t aht20_handle)
{
	  uint8_t tmp = 0x00;
    I2cMaster_WriteReg(aht20_handle->i2c_handle, aht20_handle->i2c_addr, 
                       AHT20_SoftReset, &tmp, 1);
}   

/**
  * @brief  AHT20 Initialize calibration.
  * @param  aht20_handle  aht20 operation handle pointer.
  * @retval 
  *         - ESP_OK    successful.
  *         - ESP_FAIL  failed. 
  *                     Maybe the device does not exist or the device is damaged.
  */
static esp_err_t aht20_calibration(AHT20_handle_t aht20_handle)
{
    uint8_t rcnt = 2+1;                                 //soft reset cmd, 2 chances.
    uint8_t icnt = 2+1;                                 //init cmd, 2 chances.
    while(--rcnt){
        icnt = 2+1;
        vTaskDelay(40 / portTICK_PERIOD_MS);
      // Before reading the temperature and humidity, first check whether the [Calibration Enable Bit] is 1
        while((!aht20_ReadCalEnableCmd(aht20_handle)) && (--icnt)){ // 2 chances
            vTaskDelay(10 / portTICK_PERIOD_MS);
            // If it is not 1, send the initialization command.
            aht20_IcInitCmd(aht20_handle);
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
        if(icnt){                                       //Calibration is normal
            break;
        }else{                                          //Calibration fail.
          aht20_SoftResetCmd(aht20_handle);
          vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }	
    if(rcnt){
        vTaskDelay(200 / portTICK_PERIOD_MS);
        return ESP_OK;
    }else{
        return ESP_FAIL;
    }
}

/**
  * @brief  Initialize the AHT20 and obtain an operation handle.
  * @param[in]  i2c_handle  i2c master operation handle.
  * @param[in]  i2c_addr  i2c slave address(7bit).
  *                       Under normal circumstances it is 0X38.
  * @retval  
  *         successful  aht20 operation handle.
  *         failed      NULL.
  * @note  Initialize and obtain calibration data.
  * @note  Use AHT20_Deinit() to release it.
  */
AHT20_handle_t AHT20_Init(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr)
{
    esp_err_t err = ESP_OK;

    if (NULL == i2c_handle) {
        ESP_LOGE(TAG, "%s (%d) i2c handle is not initialized.", __FUNCTION__, __LINE__);
        return NULL;
    }
    if (i2c_handle->i2c_clk > 400000) {
        ESP_LOGE(TAG, "%s (%d) AHT20 I2C supports up to 400Kbit/s", __FUNCTION__, __LINE__);
        return NULL;
    }
    if (false == I2CMaster_CheckDeviceAlive(i2c_handle, i2c_addr)) {
        ESP_LOGE(TAG, "%s (%d) device not alive.", __FUNCTION__, __LINE__);
        return NULL;
    }

    AHT20_handle_t aht20_handle = malloc(sizeof(AHT20_t));
    if (NULL == aht20_handle) {
        ESP_LOGE(TAG, "%s (%d) driver handle malloc failed.", __FUNCTION__, __LINE__);
        return NULL;
    }
    aht20_handle->i2c_handle = i2c_handle;
    aht20_handle->i2c_addr = i2c_addr;

    err = aht20_calibration(aht20_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "%s (%d) device calibration failed.", __FUNCTION__, __LINE__);
        goto AHT20_INIT_FAILED;
    }
    ESP_LOGI(TAG, "%s (%d) aht20 init ok.", __FUNCTION__, __LINE__);
    return aht20_handle;

AHT20_INIT_FAILED:
    free(aht20_handle);
    return NULL;
}

/**
  * @brief  AHT20 deinitialization.
  * @param[in]  aht20_handle  aht20 operation handle pointer.
  * @retval 
  *         - ESP_OK    successful.
  *         - ESP_FAIL  failed.
  */
esp_err_t AHT20_Deinit(AHT20_handle_t* aht20_handle)
{
    AHT20_HANDLE_CHECK(*aht20_handle, ESP_FAIL);

    free(*aht20_handle);
    *aht20_handle = NULL;
    ESP_LOGI(TAG, "%s (%d) aht20 handle deinit ok.", __FUNCTION__, __LINE__);
    return ESP_OK;
}

/**
  * @brief  AHT20 Read the raw data of the device.(20 bit)
  * @param[in]  aht20_handle  aht20 operation handle pointer.
  * @retval 
  *         - ESP_OK    successful.
  *         - ESP_FAIL  failed.
  */
esp_err_t AHT20_GetRawData(AHT20_handle_t aht20_handle)
{
    uint8_t cnt=3+1;
    uint8_t tmp[6] = {0,0,0,0,0,0};
    uint32_t RetuData = 0;

    AHT20_HANDLE_CHECK(aht20_handle, ESP_FAIL);
    
    aht20_TrigMeasureCmd(aht20_handle);

    // Wait for the measurement to complete.
    do {
        vTaskDelay(75 / portTICK_PERIOD_MS);              
    } while(aht20_ReadBusyCmd(aht20_handle) && (--cnt));
    // If the device is idle, you can read the data.
    if (cnt) {    
        vTaskDelay(5 / portTICK_PERIOD_MS);
        // 读温湿度数据
        I2cMaster_ReadReg(aht20_handle->i2c_handle, aht20_handle->i2c_addr, 
                          AHT20_STATUS_REG, tmp, 6);

        // Calculate the relative humidity RH. The original value, not calculated as the standard unit %.
        RetuData = 0;
        RetuData = (RetuData|tmp[1]) << 8;
        RetuData = (RetuData|tmp[2]) << 8;
        RetuData = (RetuData|tmp[3]);
        RetuData = RetuData >> 4;
        aht20_handle->aht20_data.HT[0] = RetuData;
        // Calculate the temperature T. Original value, not calculated as the standard unit °C.
        RetuData = 0;
        RetuData = (RetuData|tmp[3]) << 8;
        RetuData = (RetuData|tmp[4]) << 8;
        RetuData = (RetuData|tmp[5]);
        RetuData = RetuData&0xfffff;
        aht20_handle->aht20_data.HT[1] = RetuData;
        return ESP_OK;
    }                                             
    return ESP_FAIL;
}

/**
  * @brief  AHT20 Temperature and humidity signal conversion 
  *         (from 20Bit original data, converted to standard unit RH=%, T=°C)
  * @param[in]  aht20_handle  aht20 operation handle pointer.
  * @param[out]  RH  Humidity value.
  * @param[out]  Temp  Temperature value.
  * @retval 
  *         - ESP_OK    successful.
  *         - ESP_FAIL  failed.
  */
esp_err_t AHT20_StandardUnitCon(AHT20_handle_t aht20_handle, float* RH, float* Temp)
{
    AHT20_HANDLE_CHECK(aht20_handle, ESP_FAIL);

    Aht20_data_t* aht = &(aht20_handle->aht20_data);
    *RH = (double)aht->HT[0] *100 / 1048576;		//2^20=1048576 // Original：(double)aht->HT[0] / 1048576 *100
    *Temp = (double)aht->HT[1] *200 / 1048576 -50;
    // RH=0~100%; Temp=-40~85°C
    if((*RH >=0)&&(*RH <=100) && (*Temp >=-40)&&(*Temp <=85)){
        aht->flag = 0;
        return ESP_OK;
    }else{
        aht->flag = 1;
        return ESP_FAIL;
    }
}

