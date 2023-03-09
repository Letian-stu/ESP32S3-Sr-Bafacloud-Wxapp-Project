<!--
 * @Author: letian
 * @Date: 2022-12-04 17:10
 * @LastEditors: Letian-stu
 * @LastEditTime: 2023-03-09 16:34
 * @FilePath: /ESP32_Project/README.md
 * @Description: 
 * Copyright (c) 2023 by letian 1656733975@qq.com, All Rights Reserved. 
-->
# a esp32s3 project

## 1.项目介绍

无

## 2.开发环境及其他

- vscode+espidf4.4.2
- esp32s3-mini-n4r2

## 3.遇到问题

-   ~~size_t dram = heap_caps_get_free_size(MALLOC_CAP_DMA);~~
-   ~~dma ram 申请失败 lvgl和cam和sr的初始化需要注意~~

```
# ESP-IDF Partition Table
# Name  ,  Type  , SubType , Offset ,  Size     , Flags
nvs     ,  data  , nvs     ,        ,  50K     ,
storage ,  data  , spiffs  ,        ,  50K     ,
factory ,  app   , factory ,        ,  3M       ,
font    ,  data  , 0x40    ,        ,  600K     ,
(如上设置会编译不通过)
# ESP-IDF Partition Table
# Name  ,  Type  , SubType , Offset ,  Size     , Flags
nvs     ,  data  , nvs     ,        ,  100K     ,
storage ,  data  , spiffs  ,        ,  100K     ,
factory ,  app   , factory ,        ,  3M       ,
font    ,  data  , 0x40    ,        ,  600K     ,
(如上设置会编译通过)
我不理解
注意:在中文显示时，编码设置为utf-8
esptool.py --chip esp32s3 --port COM3 --baud 115200 write_flash -z 0x9f000 lv_font.bin
esptool.py --chip esp32s3 --port /dev/ttyUSB0 --baud 115200 write_flash -z 0x340000  lv_font.bin
```