<!--
 * @Author: letian
 * @Date: 2022-12-04 17:09
 * @LastEditors: letian
 * @LastEditTime: 2022-12-04 22:41
 * @FilePath: \ESP32S3-Sr-Bafacloud-Wxapp-Project\README.md
 * @Description: 
 * Copyright (c) 2022 by letian 1656733975@qq.com, All Rights Reserved. 
-->
# a esp32s3 project

## 1.项目介绍

这是我的毕业设计，虽然还没没有毕业，为明年做准备吧！

- 22/11/29	  创建项目，确定基本目标
- 22/11/30	  nvs存储上次配网信息，web配网
- 22/12/01    连接巴法云，数据同步，米家同步巴法云
- 22/12/02    语音识别（会误触）
- 22/12/03    添加cmd组件
- 22/12/21    绘制好pcb，开始打样
- 23/01/14    修修改改第三版打印成功了，各个外设正常工作，可以开始调代码了

## 2.希望实现功能

- [x] WIFI配网	通过ap在浏览器输入ip配置WiFi账户和密码，将密码存储在nvs中
- [x] 语音识别    基本可以识别
- [x] 连接巴法云    使用mqtt接入
- [x] 小程序控制    修改bafa的demo
- [ ] 播放音频(预计回家开始准备)  //pcb中未加
- [ ] 加入屏幕 然后使用lvgl做一些界面
- [ ] 是否需要搞个分布式 再搞个从设备 上传个啥数据到云上 
- [ ] 暂时确定搞个esp32c3的采集温湿度(主要是想画个小板子)

- [ ] 搞个摄像头玩玩 
- [ ] sd卡需要有 存储照片 
- [ ] 发现好玩的网站 https://www.tianapi.com/ 添加语音获取新闻 热点 笑话 舔狗文案 (添加字库)

## 3.开发环境及其他

- vscode+espidf4.4.2
- esp32s3-mini-n4r2

## 4.图片

<img src="Other\pcb1.jpg" alt="背面"  />

<img src="Other\pcb2.jpg" alt="正面"  />
