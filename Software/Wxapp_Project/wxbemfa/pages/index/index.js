// index.js
// 获取应用实例
const app = getApp()
import mqtt from '../../utils/js/mqtt.min.js';//加载mqtt库

Page({
  data: {
    uid:"6bf959ef20e34a08a876c765108576a8",//用户密钥，巴法云控制台获取
    E2Wtopic:"DriverAHT004",//传输温湿度的主题，控制台创建
    device_status:"离线",// 显示led是否在线的字符串，默认离线
    wendu:0,//温度值，默认为空
    shidu:0,//湿度值，默认为空
    client: null,//mqtt客户端，默认为空
    //add by tian
    W2ELed1topic:"DriverLED002",
    SWLED1:false,
    W2ELed2topic:"DriverFAN003",
    SWLED2:false,
    W2ELed3topic:"DriverKEY006",
    SWLED3:false,
  },

  mqttConnect(){
    var that = this
    //MQTT连接的配置
    var options= {
      keepalive: 60, //60s ，表示心跳间隔
      clean: true, //cleanSession不保持持久会话
      protocolVersion: 4, //MQTT v3.1.1
      clientId:this.data.uid
    }
    //初始化mqtt连接
     this.data.client = mqtt.connect('wxs://bemfa.com:9504/wss',options)
     // 连接mqtt服务器
     this.data.client.on('connect', function () {
      console.log('连接服务器成功')
      //订阅dht11温湿度主题
      that.data.client.subscribe(that.data.E2Wtopic, function (err) {
        if (err) {
            console.log(err)
        }
      })
      that.data.client.subscribe(that.data.W2ELed1topic, function (err) {
        if (err) {
            console.log(err)
        }
      })
      that.data.client.subscribe(that.data.W2ELed2topic, function (err) {
        if (err) {
            console.log(err)
        }
      })
      that.data.client.subscribe(that.data.W2ELed3topic, function (err) {
        if (err) {
            console.log(err)
        }
      })
    })
    //接收消息
    that.data.client.on('message', function (topic, message) {
      console.log(topic)
      var  msg = message.toString()
      if(topic == that.data.W2ELed1topic){
        //console.log(msg)
        if(msg == "on"){
            console.log("led on")
            that.setData({ 
                SWLED1: true,
            });
        }
        if(msg == "off"){
            console.log("led off")
            that.setData({ 
                SWLED1: false,
            });
        }       
      }
      if(topic == that.data.W2ELed2topic){
        //console.log(msg)
        if(msg == "on"){
            console.log("fan on")
            that.setData({ 
                SWLED2: true,
            });
        }
        if(msg == "off"){
            console.log("fan off")
            that.setData({ 
                SWLED2: false,
            });
        }       
      }
      if(topic == that.data.W2ELed3topic){
        //console.log(msg)
        if(msg == "on"){
            console.log("key on")
            that.setData({ 
                SWLED3: true,
            });
        }
        if(msg == "off"){
            console.log("key off")
            that.setData({ 
                SWLED3: false   ,
            });
        }       
      }
      if(topic == that.data.E2Wtopic){//如果是温湿度主题的消息
        //假设上传的数据为#23#45#off，其中温度是23，湿度45，led为关闭状态
        if(msg.indexOf("#") != -1){//如果数据里包含#号，表示获取的是传感器值，因为单片机上传数据的时候用#号进行了包裹
          //如果有#号就进行字符串分割
            var all_data_arr = msg.split("#"); //分割数据，并把分割后的数据放到数组里。
            console.log(all_data_arr)//打印数组
            // if(all_data_arr[3] != undefined){//判断是否上传了led状态
            //     if(all_data_arr[3] == "on")
            //     {//如果单片机处于打开状态
            //         that.setData({ //数据赋值给变量
            //             ledOnOff:"打开",//赋值led状态
            //         })
            //     }else{
            //         that.setData({ //数据赋值给变量
            //             ledOnOff:"关闭",//赋值led状态
            //         })
            //     }
            // }
            that.setData({ //数据赋值给变量
            wendu:all_data_arr[1],//赋值温度
            shidu:all_data_arr[2], //赋值湿度
          })
        }
      }
      //打印消息
      console.log('收到消息：'+msg)
    })
    //断线重连
    this.data.client.on("reconnect", function () {
      console.log("重新连接")
    });
    },
    //屏幕打开时执行的函数
    onLoad() {
    //连接mqtt
    this.mqttConnect()
    //检查设备是否在线
    this.getOnline()
    //检查设备是打开还是关闭
    this.getOnOff()
    //获取服务器上现在存储的dht11数据
    this.getdht11()
    },
    //控制三个led的开关
    SWLED1Handler({ detail }){
        this.setData({ 
            SWLED1: detail,
        });
        if(detail == true){//如果是打开操作
            this.data.client.publish(this.data.W2ELed1topic, 'on')//mqtt推送on
            //this.setData({ ledicon: "/utils/img/lighton.png",});//设置led图片为on
        }else{
            this.data.client.publish(this.data.W2ELed1topic, 'off')//mqtt推送off
            //this.setData({ ledicon: "/utils/img/lightoff.png",});//设置led图片为off
        }
    },
    SWLED2Handler({ detail }){
        this.setData({ 
            SWLED2: detail,
        });
        if(detail == true){//如果是打开操作
            this.data.client.publish(this.data.W2ELed2topic, 'on')//mqtt推送on
            //this.setData({ ledicon: "/utils/img/lighton.png",});//设置led图片为on
        }else{
            this.data.client.publish(this.data.W2ELed2topic, 'off')//mqtt推送off
            //this.setData({ ledicon: "/utils/img/lightoff.png",});//设置led图片为off
        }
    },
    SWLED3Handler({ detail }){
        this.setData({ 
            SWLED3: detail,
        });
        if(detail == true){//如果是打开操作
            this.data.client.publish(this.data.W2ELed3topic, 'on')//mqtt推送on
            //this.setData({ ledicon: "/utils/img/lighton.png",});//设置led图片为on
        }else{
            this.data.client.publish(this.data.W2ELed3topic, 'off')//mqtt推送off
            //this.setData({ ledicon: "/utils/img/lightoff.png",});//设置led图片为off
        }
    },

    getOnline(){
    var that = this
    //请求设备状态,检查设备是否在线
    //api 接口详细说明见巴法云接入文档
    wx.request({
        url: 'https://api.bemfa.com/mqtt/status/', //状态api接口，详见巴法云接入文档
        data: {
        uid: that.data.uid,
        topic: that.data.E2Wtopic,
    },
    header: {
    'content-type': "application/x-www-form-urlencoded"
    },
    success (res) {
        console.log(res.data)
        if(res.data.status === "online"){
            that.setData({
            device_status:"在线"
        })
        }else{
            that.setData({
            device_status:"离线"
        })
        }
        console.log(that.data.device_status)
    }
    })    
    },
    getOnOff(){
    //获取3个led的状态
    var that = this
    wx.request({
      url: 'https://api.bemfa.com/api/device/v1/data/3/get/', //状态api接口，详见巴法云接入文档
      data: {
        uid: that.data.uid,
        topic: that.data.W2ELed1topic,
        num:1
      },
      header: {
        'content-type': "application/x-www-form-urlencoded"
      },
      success (res) {
        console.log(res)
        if("undefined" != typeof res.data.data){//判断是否获取到温湿度
          console.log(res.data.data[0].msg)
          if(res.data.data[0].msg == "on"){
            that.setData({
              SWLED1:true,
              //ledOnOff:"打开",
              //ledicon: "/utils/img/lighton.png",
            })
          }else{
            that.setData({
              SWLED1:false,
              //ledOnOff:"关闭",
              //ledicon: "/utils/img/lightoff.png",
            })
          }
        }

      }
    }) 
    wx.request({
        url: 'https://api.bemfa.com/api/device/v1/data/3/get/', //状态api接口，详见巴法云接入文档
        data: {
          uid: that.data.uid,
          topic: that.data.W2ELed2topic,
          num:1
        },
        header: {
          'content-type': "application/x-www-form-urlencoded"
        },
        success (res) {
          console.log(res)
          if("undefined" != typeof res.data.data){//判断是否获取到温湿度
            console.log(res.data.data[0].msg)
            if(res.data.data[0].msg == "on"){
              that.setData({
                SWLED2:true,
                //ledOnOff:"打开",
                //ledicon: "/utils/img/lighton.png",
              })
            }else{
              that.setData({
                SWLED2:false,
                //ledOnOff:"关闭",
                //ledicon: "/utils/img/lightoff.png",
              })
            }
          }
  
        }
    }) 
    wx.request({
        url: 'https://api.bemfa.com/api/device/v1/data/3/get/', //状态api接口，详见巴法云接入文档
        data: {
          uid: that.data.uid,
          topic: that.data.W2ELed3topic,
          num:1
        },
        header: {
          'content-type': "application/x-www-form-urlencoded"
        },
        success (res) {
          console.log(res)
          if("undefined" != typeof res.data.data){//判断是否获取到温湿度
            console.log(res.data.data[0].msg)
            if(res.data.data[0].msg == "on"){
              that.setData({
                SWLED3:true,
                //ledOnOff:"打开",
                //ledicon: "/utils/img/lighton.png",
              })
            }else{
              that.setData({
                SWLED3:false,
                //ledOnOff:"关闭",
                //ledicon: "/utils/img/lightoff.png",
              })
            }
          }
        }
    })    
    },
    getdht11(){
    //获取温湿度值，屏幕初始化时，未订阅收到温湿度时，先去主动获取值
    //api 接口详细说明见巴法云接入文档
    var that = this
    wx.request({
      url: 'https://api.bemfa.com/api/device/v1/data/3/get/', //状态api接口，详见巴法云接入文档
      data: {
        uid: that.data.uid,
        topic: that.data.E2Wtopic,
        num:1
      },
      header: {
        'content-type': "application/x-www-form-urlencoded"
      },
      success (res) {
        console.log(res)
        if("undefined" != typeof res.data.data){//判断是否获取到温湿度
          console.log(res.data.data[0].msg)
          if(res.data.data[0].msg.indexOf("#") != -1){//如果数据里包含#号，表示获取的是传感器值，因为单片机上传数据的时候用#号进行了包裹
            //如果有#号就进行字符串分割
            var all_data_arr = res.data.data[0].msg.split("#"); //分割数据，并把分割后的数据放到数组里。
            console.log(all_data_arr)//打印数组
            that.setData({ //数据赋值给变量
              wendu:all_data_arr[1],//赋值温度
              shidu:all_data_arr[2], //赋值湿度
            })
          }
        }
      }
    })    
  }
})
