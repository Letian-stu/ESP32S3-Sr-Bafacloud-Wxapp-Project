<!--
 * @Author: letian
 * @Date: 2022-12-04 17:10
 * @LastEditors: letian
 * @LastEditTime: 2023-01-01 19:59
 * @FilePath: \ESP32_Project\README.md
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

- 语音识别需要由DIO转QIO,但是使用QIO的时候打开wifi到十分钟左右的时候会闪退

```c
I (477734) main: count:477
I (478734) main: count:478
I (479734) main: count:479
I (480734) main: count:480
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0x8 (TG1WDT_SYS_RST),boot:0x29 (SPI_FAST_FLASH_BOOT)
Saved PC:0x40378969
0x40378969: _xt_panic at D:/B_Code/espidf/esp32idf/esp-idf/components/esp_system/port/arch/xtensa/panic_handler_asm.S:30
invalid header: 0x4200490c
0x4200490c: get_local_fd at D:/B_Code/espidf/esp32idf/esp-idf/components/vfs/vfs.c:285
 (inlined by) esp_vfs_read at D:/B_Code/espidf/esp32idf/esp-idf/components/vfs/vfs.c:451
invalid header: 0x0b1cc212
invalid header: 0x7c2000a0
invalid header: 0x9200061c
invalid header: 0x80472404
invalid header: 0x69049920
```


-   size_t dram = heap_caps_get_free_size(MALLOC_CAP_DMA);
-   dma ram 申请失败 lvgl和cam和sr的初始化需要注意