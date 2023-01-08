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
  * @file           i2c_master.c
  * @version        1.0
  * @date           2021-5-19
  */

#include "i2c_master.h"
#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "I2cMaster";

// Mark whether the I2C port is initialized.
static bool i2c_num_0_install = false;
static bool i2c_num_1_install = false;

#define I2C_MASTER_HANDLE_CHECK(a, ret)  if (NULL == a) {                        \
        ESP_LOGE(TAG, "%s (%d) driver handle is NULL.", __FUNCTION__, __LINE__); \
        return (ret);                                                            \
        }
#define I2C_MASTER_SLAVE_ADDR_CHECK(a, ret)  if (a > 0x7F) {                              \
        ESP_LOGE(TAG, "%s (%d) i2c slave address format error.", __FUNCTION__, __LINE__); \
        return (ret);                                                                     \
        }

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
                                  uint32_t i2c_clk)
{
    esp_err_t err = ESP_OK;

    if (0 == I2c_port && true == i2c_num_0_install) {
        ESP_LOGE(TAG, "%s (%d) i2c num0 has been initialized.", __FUNCTION__, __LINE__);
        return NULL;
    } else if (1 == I2c_port && true == i2c_num_1_install) {
        ESP_LOGE(TAG, "%s (%d) i2c num1 has been initialized.", __FUNCTION__, __LINE__);
        return NULL;
    }

    I2cMaster_handle_t i2c_handle = malloc(sizeof(I2cMaster_t));
    if (NULL == i2c_handle) {
        ESP_LOGE(TAG, "%s (%d) driver handle malloc failed.", __FUNCTION__, __LINE__);
        return NULL;
    }

    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_num,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = scl_num,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = i2c_clk,
    };
    err = i2c_param_config(I2c_port, &i2c_conf);
    if (ESP_OK != err) {
        goto I2C_MASTER_INIT_FAIL;
    }
    err = i2c_driver_install(I2c_port, I2C_MODE_MASTER, 0, 0, 0);
    if (ESP_OK != err) {
        goto I2C_MASTER_INIT_FAIL;
    }
        
    if (0 == I2c_port) {
        i2c_num_0_install = true;
    } else if (1 == I2c_port) {
        i2c_num_1_install = true;
    }
        
    i2c_handle->i2c_port = I2c_port;
    i2c_handle->i2c_clk = i2c_clk;
    ESP_LOGI(TAG, "%s (%d) i2c master init ok.", __FUNCTION__, __LINE__);
    return i2c_handle;

I2C_MASTER_INIT_FAIL:
    free(i2c_handle);
    ESP_LOGE(TAG, "%s (%d) i2c master init failed.", __FUNCTION__, __LINE__);
    return NULL;
}

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
esp_err_t I2cMaster_Deinit(I2cMaster_handle_t* i2c_handle)
{
    esp_err_t err = ESP_OK;

    I2C_MASTER_HANDLE_CHECK(*i2c_handle, ESP_FAIL);

    err = i2c_driver_delete((*i2c_handle)->i2c_port);
    if (ESP_OK != err) {
        return ESP_FAIL;
    }

    if(0 == (*i2c_handle)->i2c_port)
        i2c_num_0_install = false;
    else if(1 == (*i2c_handle)->i2c_port)
        i2c_num_1_install = false;

    free(*i2c_handle);
    *i2c_handle = NULL;
    ESP_LOGI(TAG, "%s (%d) i2c master deinit ok.", __FUNCTION__, __LINE__);
    return ESP_OK;
}

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
I2cMaster_handle_t I2cMaster_GetHandleNoInit(i2c_port_t I2c_port, uint32_t i2c_clk)
{
    if (0 == I2c_port && true == i2c_num_0_install) {
        ESP_LOGE(TAG, "%s (%d) i2c num0 has been initialized.", __FUNCTION__, __LINE__);
        return NULL;
    } else if (1 == I2c_port && true == i2c_num_1_install) {
        ESP_LOGE(TAG, "%s (%d) i2c num1 has been initialized.", __FUNCTION__, __LINE__);
        return NULL;
    }

    I2cMaster_handle_t i2c_handle = malloc(sizeof(I2cMaster_t));
    if (NULL == i2c_handle) {
        ESP_LOGE(TAG, "%s (%d) driver handle malloc failed.", __FUNCTION__, __LINE__);
        return NULL;
    }

    if (0 == I2c_port) {
        i2c_num_0_install = true;
    } else if (1 == I2c_port) {
        i2c_num_1_install = true;
    }
        
    i2c_handle->i2c_port = I2c_port;
    i2c_handle->i2c_clk = i2c_clk;
    ESP_LOGI(TAG, "%s (%d) i2c master get handle ok.", __FUNCTION__, __LINE__);
    return i2c_handle;
}

/**
  * @brief  Delete uninitialized i2c master handle.
  * @param[in]  i2c_handle  i2c master operation handle pointer.
  * @retval 
  *         - ESP_OK    successful.
  *         - ESP_FAIL  failed.
  * @note  Can only be used to release the handle obtained by I2cMaster_GetHandleNoInit().
  */ 
esp_err_t I2cMaster_DeleteHandleNoInit(I2cMaster_handle_t* i2c_handle)
{
    I2C_MASTER_HANDLE_CHECK(*i2c_handle, ESP_FAIL);

    if(0 == (*i2c_handle)->i2c_port)
        i2c_num_0_install = false;
    else if(1 == (*i2c_handle)->i2c_port)
        i2c_num_1_install = false;

    free(*i2c_handle);
    *i2c_handle = NULL;
    ESP_LOGI(TAG, "%s (%d) i2c master handle delete ok.", __FUNCTION__, __LINE__);
    return ESP_OK;
}

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
bool I2CMaster_CheckDeviceAlive(I2cMaster_handle_t i2c_handle, uint8_t i2c_addr)
{
    esp_err_t err = ESP_OK;

    I2C_MASTER_HANDLE_CHECK(i2c_handle, false);
    I2C_MASTER_SLAVE_ADDR_CHECK(i2c_addr, false);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1)|I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);
    err = i2c_master_cmd_begin(i2c_handle->i2c_port, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (err == ESP_OK) {
        return true;
    }
    return false;
}

/**
  * @brief  Print I2C transmission execution result.
  * @param[in]  val  error number.
  * @retval void
  * @note  This function is generally only used in debugging.
  */
void I2cMaster_PrintErr(esp_err_t val)
{
    switch(val) {
        case ESP_OK: {
            ESP_LOGI(TAG, "ESP_OK : Successful.\n");
            break;
        }  
        case ESP_ERR_INVALID_ARG: {
            ESP_LOGE(TAG, "ESP_ERR_INVALID_ARG : Invalid argument.\n");
            break;
        }   
        case ESP_FAIL: {
            ESP_LOGE(TAG, "ESP_FAIL : Error sending command. Slave did not confirm transmission.\n");
            break;
        }   
        case ESP_ERR_INVALID_STATE: {
            ESP_LOGE(TAG, "ESP_ERR_INVALID_STATE : I2C Driver not installed or not in main mode.\n");
            break;
        }   
        case ESP_ERR_TIMEOUT: {
            ESP_LOGE(TAG, "ESP_ERR_TIMEOUT : The operation timed out because the bus was busy.\n");
            break;
        }   
        default: {
            break;
        }      
    }
}

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
                             uint8_t* data_buf, uint32_t data_len)
{
    esp_err_t ret = ESP_OK;

    I2C_MASTER_HANDLE_CHECK(i2c_handle, ESP_FAIL);
    I2C_MASTER_SLAVE_ADDR_CHECK(i2c_addr, ESP_FAIL);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1)|I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_write(cmd, data_buf, data_len, true);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_handle->i2c_port, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

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
                            uint8_t* data_buf, uint32_t data_len)
{
    esp_err_t ret = ESP_OK;

    I2C_MASTER_HANDLE_CHECK(i2c_handle, ESP_FAIL);
    I2C_MASTER_SLAVE_ADDR_CHECK(i2c_addr, ESP_FAIL);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1)|I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1)|I2C_MASTER_READ, true);
    i2c_master_read(cmd, data_buf, data_len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_handle->i2c_port, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

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
                              uint32_t data_len)
{
    esp_err_t ret = ESP_OK;

    I2C_MASTER_HANDLE_CHECK(i2c_handle, ESP_FAIL);
    I2C_MASTER_SLAVE_ADDR_CHECK(i2c_addr, ESP_FAIL);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1)|I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, data_buf, data_len, true);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_handle->i2c_port, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

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
                             uint32_t data_len)
{
    esp_err_t ret = ESP_OK;

    I2C_MASTER_HANDLE_CHECK(i2c_handle, ESP_FAIL);
    I2C_MASTER_SLAVE_ADDR_CHECK(i2c_addr, ESP_FAIL);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_addr << 1)|I2C_MASTER_READ, true);
    i2c_master_read(cmd, data_buf, data_len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_handle->i2c_port, cmd, 1000/portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

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
                                uint8_t bit_num, uint8_t bit_val, uint8_t bit_len)
{
    esp_err_t err = ESP_OK;
    uint8_t data_buf = 0x00;

    if (bit_num >= 8 || bit_num+bit_len>8) {
        ESP_LOGE(TAG, "%s (%d) Maximum bit exceeded. ", __FUNCTION__, __LINE__);
        return ESP_FAIL;
    }
    // This is to make sure that bit_val and bit_len bits long are the same length.
    if (bit_val >= (0x01 << bit_len)) {
        ESP_LOGE(TAG, "%s (%d) bits value over bit max length. ", __FUNCTION__, __LINE__);
        return ESP_FAIL;
    }

    /* In order to prevent modifying other bits that do not need to be modified, 
       read the original data first. */
    err = I2cMaster_ReadReg(i2c_handle, i2c_addr, reg_addr, &data_buf, 1);
    if (err != ESP_OK) {
        return ESP_FAIL;
    }
    
    // Clear the corresponding bit, and then write a new value.
    data_buf &= ~(~(0xff << bit_len) << bit_num);
    data_buf |= (bit_val << bit_num);
    err = I2cMaster_WriteReg(i2c_handle, i2c_addr, reg_addr, &data_buf, 1);
    return err;
}   

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
                                uint8_t bit_num, uint8_t *bit_val, uint8_t bit_len)
{
    esp_err_t err = ESP_OK;
    uint8_t data_buf = 0x00;

    if (bit_num >= 8 || bit_num+bit_len>8) {
        ESP_LOGE(TAG, "%s (%d) Maximum bit exceeded.", __FUNCTION__, __LINE__);
        return ESP_FAIL;
    }

    err = I2cMaster_ReadReg(i2c_handle, i2c_addr, reg_addr, &data_buf, 1);
    if (err != ESP_OK) {
        return ESP_FAIL;
    }

    *bit_val = (data_buf>>bit_num) & ~(0xff<<bit_len);
    return ESP_OK;
}   