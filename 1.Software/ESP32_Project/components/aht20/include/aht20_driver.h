/**
  * @file           aht20_driver.h
  * @version        1.0
  * @date           2021-5-27
  */

#ifndef __AHT20_DRIVER_H__
#define __AHT20_DRIVER_H__

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../../i2c_master/include/i2c_master.h"

#define	AHT20_STATUS_REG		        (0x71)
#define	AHT20_INIT_REG			        (0xBE)
#define	AHT20_SoftReset			        (0xBA)
#define	AHT20_TrigMeasure_REG	        (0xAC)

typedef struct{
	uint8_t flag;				// Read/calculate error flag bit. 0: Reading/calculating data is normal; 1: Reading/calculating device fails
	uint32_t HT[2];				// Humidity, temperature, the value of the original sensor, 20Bit
}Aht20_data_t;

typedef struct{
    I2cMaster_handle_t i2c_handle;
    uint8_t i2c_addr;
    Aht20_data_t aht20_data;
}AHT20_t;
typedef AHT20_t *AHT20_handle_t;

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
AHT20_handle_t AHT20_Init(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr);

/**
  * @brief  AHT20 deinitialization.
  * @param[in]  aht20_handle  aht20 operation handle pointer.
  * @retval 
  *         - ESP_OK    successful.
  *         - ESP_FAIL  failed.
  */
esp_err_t AHT20_Deinit(AHT20_handle_t* aht20_handle);

/**
  * @brief  AHT20 Read the raw data of the device.(20 bit)
  * @param[in]  aht20_handle  aht20 operation handle pointer.
  * @retval 
  *         - ESP_OK    successful.
  *         - ESP_FAIL  failed.
  */
esp_err_t AHT20_GetRawData(AHT20_handle_t aht20_handle);

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
esp_err_t AHT20_StandardUnitCon(AHT20_handle_t aht20_handle, float* RH, float* Temp);

#endif /* __AHT20_DRIVER__ */