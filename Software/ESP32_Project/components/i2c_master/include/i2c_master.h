/*****************************************************************************
 *                                                                           *
 *  Copyright 2021 upahead PTE LTD                                           *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *                                                                           *
 *****************************************************************************/
/**
  * @file           i2c_master.h
  * @version        1.0
  * @date           2021-5-19
  */

#ifndef __I2C_MASTER_H_
#define __I2C_MASTER_H_

#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_err.h"

typedef struct{
    i2c_port_t i2c_port;
    uint32_t i2c_clk;
}I2cMaster_t;
typedef I2cMaster_t *I2cMaster_handle_t;

/**
  * @brief  Initialize the I2C master and obtain an operation handle.
  * @param[in]  I2c_port  esp32 i2c port number, i2c0 or i2c1.
  * @param[in]  scl_pin  scl pin num.
  * @param[in]  sda_pin  sda pin num.
  * @param[in]  i2c_clk  The transmission speed of I2C is generally 400000 or 100000.
  * @retval 
  *         - successful  I2C operation handle.
  *         - failed      NULL.
  * @note  Use I2cMaster_Deinit() to release it.
  */
I2cMaster_handle_t I2cMaster_Init(i2c_port_t I2c_port, gpio_num_t scl_num, gpio_num_t sda_num, 
                                  uint32_t i2c_clk);

/**
  * @brief  I2C master deinitialization.
  *         Release the resources occupied by I2C master.
  * @param[in]  i2c_handle  i2c master operation handle pointer.
  * @retval 
  *         - ESP_OK    successful.
  *         - ESP_FAIL  failed.
  *                     May be caused by no initialization.
  * @note  Can only be used to release the handle obtained by I2cMaster_Init().
  */
esp_err_t I2cMaster_Deinit(I2cMaster_handle_t* i2c_handle);

/**
  * @brief  Get an I2cMaster operation handle of the initialized i2c port.
  *         This function does not initialize the I2C device, but only provides 
  *         an operation handle compatible with this library.
  * @param[in]  I2c_port  esp32 i2c port number, i2c0 or i2c1.
  *                      Make sure that this port has been initialized.
  * @param[in]  i2c_clk  The transmission speed.
  *                     Make sure that this value is the same as the initialization.
  * @retval 
  *         - successful  I2C operation handle.
  *         - failed      NULL.
  * @note  Use I2cMaster_DeleteHandleNoInit() to release it.
  * @note  This function is provided for compatibility with this library without modifying 
  *        the original code. It is used when the I2C port has been initialized, otherwise 
  *        it is recommended to use I2cMaster_Init() in any other situations.
  */        
I2cMaster_handle_t I2cMaster_GetHandleNoInit(i2c_port_t I2c_port, uint32_t i2c_clk);

/**
  * @brief  Delete uninitialized i2c master handle.
  * @param[in]  i2c_handle  i2c master operation handle pointer.
  * @retval 
  *         - ESP_OK    successful.
  *         - ESP_FAIL  failed.
  * @note  Can only be used to release the handle obtained by I2cMaster_GetHandleNoInit().
  */ 
esp_err_t I2cMaster_DeleteHandleNoInit(I2cMaster_handle_t* i2c_handle);

/**
  * @brief  Check if the I2C slave alive.
  * @param[in]  i2c_handle  i2c master operation handle.
  * @param[in]  i2c_addr  i2c slave address(7bit).
  * @retval  
  *         - true    alive
  *         - false   not alive
  * @note  Wait for response by sending i2c slave address signal to detect whether
  *        the device exists.
  */
bool I2CMaster_CheckDeviceAlive(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr);

/**
  * @brief  Print I2C transmission execution result.
  * @param[in]  val  error number.
  * @retval void
  * @note  This function is generally only used in debugging.
  */
void I2cMaster_PrintErr(esp_err_t val);

/* Here we provide some commonly used communication format interfaces for I2C devices, 
   of course you don’t have to use it. */

/**
  * @brief  I2C master writes data to the slave register.
  * @param[in]  i2c_handle  i2c master operation handle.
  * @param[in]  i2c_addr  i2c slave address(7bit).
  * @param[in]  reg_addr  i2c slave register address.
  * @param[in]  data_buf  Data pointer.
  * @param[in]  data_len  Data length.
  * @retval  reference esp_err_t.
  */
esp_err_t I2cMaster_WriteReg(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr, uint8_t reg_addr, 
                             uint8_t* data_buf, uint32_t data_len);

/**
  * @brief  I2C master read slave register data.
  * @param[in]  i2c_handle  i2c master operation handle.
  * @param[in]  i2c_addr  i2c slave address(7bit).
  * @param[in]  reg_addr  i2c slave register address.
  * @param[out] data_buf  Data pointer.
  * @param[in]  data_len  Data length.
  * @retval  reference esp_err_t.
  */
esp_err_t I2cMaster_ReadReg(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr, uint8_t reg_addr, 
                            uint8_t* data_buf, uint32_t data_len);

/**
  * @brief  I2C master write data continuously.
  *         This function cannot be used to manipulate the registers of I2C slave devices, 
  *         otherwise please use I2cMaster_WriteReg().
  * @param[in]  i2c_handle  i2c master operation handle.
  * @param[in]  i2c_addr  i2c slave address(7bit).
  * @param[in]  data_buf  Data pointer.
  * @param[in]  data_len  Data length.
  * @retval  reference esp_err_t.
  * @note  Most I2C slave devices use register operations, this function is not commonly used.
  */
esp_err_t I2cMaster_WriteData(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr, uint8_t* data_buf, 
                              uint32_t data_len);

/**
  * @brief  I2C master read data continuously.
  *         This function cannot be used to manipulate the registers of I2C slave devices, 
  *         otherwise please use I2cMaster_ReadReg().
  * @param[in]  i2c_handle  i2c master operation handle.
  * @param[in]  i2c_addr  i2c slave address(7bit).
  * @param[out] data_buf  Data pointer.
  * @param[in]  data_len  Data length.
  * @retval reference esp_err_t.
  * @note  Most I2C slave devices use register operations, this function is not commonly used.
  */
esp_err_t I2cMaster_ReadData(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr, uint8_t* data_buf, 
                             uint32_t data_len);

/**
  * @brief  I2C master writes bits to the slave register.
  * @param[in]  i2c_handle  i2c master operation handle.
  * @param[in]  i2c_addr  i2c slave address(7bit). 
  * @param[in]  reg_addr  i2c slave register address.
  * @param[in]  bit_num  bit number(0~7).
  * @param[in]  bit_val  bit value, Do not exceed the maximum number of digits of bit_len.
  * @param[in]  bit_len  bit length(1~8).
  * @retval  reference esp_err_t.
  * @note  When using this function, you should ensure that the operating register is readable.
  * @note  example: 
  *        register CR[7:0]
  *        before:
  *                 BIT     7    6    5    4    3    2    1    0 
  *                 VAL     0    1    0    0    0    1    1    0
  *        Modify the BIT[4:2] of the CR register to [0 1 0].
  *        ```
  *        I2cMaster_WriteRegBit(xxx, xxx, CR, 2, 0b010, 3);
  *        ```
  *        after:
  *                 BIT     7    6    5    4    3    2    1    0 
  *                 VAL     0    1    0    0    1    0    1    0  
  */
esp_err_t I2cMaster_WriteRegBit(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr, uint8_t reg_addr, 
                                uint8_t bit_num, uint8_t bit_val, uint8_t bit_len);

/**
  * @brief  I2C master read bits from the slave register.
  * @param[in]  i2c_handle  i2c master operation handle.
  * @param[in]  i2c_addr  i2c slave address(7bit). 
  * @param[in]  reg_addr  i2c slave register address.
  * @param[in]  bit_num  bit number(0~7).
  * @param[out] bit_val  bit value.
  * @param[in]  bit_len  bit length(1~8).
  * @retval  reference esp_err_t.
  * @note  When using this function, you should ensure that the operating register is readable.
  * @note  example: 
  *        register CR[7:0]
  *                 BIT     7    6    5    4    3    2    1    0 
  *                 VAL     0    1    0    0    0    1    1    0
  *        read CR register bit[6:2].
  *        ```
  *        uint8_t bits;
  *        I2cMaster_ReadRegBit(xxx, xxx, CR, 2, &bits, 5);
  *        ```
  *        bits == 0b10001 == 0x11.
  */
esp_err_t I2cMaster_ReadRegBit(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr, uint8_t reg_addr, 
                                uint8_t bit_num, uint8_t *bit_val, uint8_t bit_len);

#endif /* __I2C_MASTER_H_ */