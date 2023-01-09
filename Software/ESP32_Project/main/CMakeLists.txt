idf_component_register(
SRCS        "main.c"
            "Init_Config/initconfig.c"  
            "Init_Config/wificonfig.c"
            "Init_Config/spiffsconfig.c"

            "web_file/web_config.c"
            "mqtt_bafa/tcp_mqtt.c"
			"speech_rec/speech_rec.c"

            "app_task/app_task.c"
            "app_task/app_timer.c"
            "app_task/app_sem.c"
            "app_task/app_queue.c"
            "app_task/app_event.c"
            
            "lvgl_task/src/lvgl_init.c" 
            "lvgl_task/src/gui_guider.c" 
            "lvgl_task/src/lv_port_indev.c" 
            "lvgl_task/img/img_skew_strip.c" 
            "lvgl_task/img/_StuLeTian_200x60.c" 
            "lvgl_task/img/_rocket_60x35.c"

INCLUDE_DIRS    "."
                "Init_Config/include"
                "web_file/include"
                "app_task/include"
                "mqtt_bafa/include"
				"speech_rec/include"
                "lvgl_task/include"

EMBED_FILES "../spiffs/config.html"
)

spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)


