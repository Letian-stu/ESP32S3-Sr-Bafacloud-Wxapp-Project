idf_component_register(
SRCS        "main.c"
            "Init_Config/initconfig.c"  
            "Init_Config/wificonfig.c"
            "Init_Config/spiffsconfig.c"

            "web_file/web_config.c"

INCLUDE_DIRS    "."
                "Init_Config/include/."
                "web_file/include/."

EMBED_FILES "../spiffs/config.html"
EMBED_FILES "../spiffs/success.html"
)

spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)


