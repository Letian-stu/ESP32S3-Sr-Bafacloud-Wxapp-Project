idf_component_register(
SRCS        "main.c"
            "Init_Config/initconfig.c"  
            "Init_Config/wificonfig.c"
            "Init_Config/spiffsconfig.c"

INCLUDE_DIRS    "."
                "Init_Config/include"
                "web_file/include"
)

spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)


