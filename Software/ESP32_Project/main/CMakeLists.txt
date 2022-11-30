idf_component_register(
SRCS        "main.c"
            "Init_Config/initconfig.c"  
            "Init_Config/wificonfig.c"
            "Init_Config/spiffsconfig.c"

            "web_file/web_config.c"

            "app_task/app_task.c"
            "app_task/app_timer.c"
            "app_task/app_sem.c"
            "app_task/app_queue.c"
            "app_task/app_event.c"

            
INCLUDE_DIRS    "."
                "Init_Config/include"
                "web_file/include"
                "app_task/include"

EMBED_FILES "../spiffs/config.html"
EMBED_FILES "../spiffs/success.html"
)

spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)


