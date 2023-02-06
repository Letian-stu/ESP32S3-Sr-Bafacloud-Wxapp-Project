idf_component_register(
SRCS        "main.c"
            "Init_Config/initconfig.c"  
            "Init_Config/wificonfig.c"
            "Init_Config/spiffsconfig.c"

            "web_file/web_config.c"
            "mqtt_bafa/tcp_mqtt.c"
			"speech_rec/speech_rec.c"
            "cam_task/cam_task.c"

            "app_task/app_task.c"
            "app_task/app_timer.c"
            "app_task/app_sem.c"
            "app_task/app_queue.c"
            "app_task/app_event.c"
            
            "lvgl_task/src/lvgl_init.c" 
            "lvgl_task/src/gui_anim.c" 
            "lvgl_task/src/gui_guider.c" 
            "lvgl_task/src/lv_port_indev.c" 

            "lvgl_task/src/page_clock.c" 
            "lvgl_task/src/page_weather.c" 
            "lvgl_task/src/page_set.c" 
            "lvgl_task/src/page_camera.c" 
            "lvgl_task/src/page_image.c" 
            "lvgl_task/src/page_sd.c" 
            
            "lvgl_task/img/img_skew_strip.c" 
            "lvgl_task/img/_StuLeTian_200x60.c" 
            "lvgl_task/img/_rocket_60x35.c"
            "lvgl_task/img/_camera_110x110.c"
            "lvgl_task/img/_clock_110x110.c"
            "lvgl_task/img/_image_110x110.c"
            "lvgl_task/img/_sd_110x110.c"
            "lvgl_task/img/_set_110x110.c"
            "lvgl_task/img/_weather_110x110.c"
            "lvgl_task/img/_back_40x40.c"
            "lvgl_task/img/_takepic_50x50.c"
            

INCLUDE_DIRS    "."
                "Init_Config"
                "cam_task"
                "web_file"
                "app_task"
                "mqtt_bafa"
				"speech_rec"
                "lvgl_task/include"

EMBED_FILES "../spiffs/config.html"
)

spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)


