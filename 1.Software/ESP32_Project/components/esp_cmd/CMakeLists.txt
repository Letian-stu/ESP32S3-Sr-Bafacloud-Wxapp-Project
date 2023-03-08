set(srcs
    "cmd_nvs/cmd_nvs.c"
    "cmd_system/cmd_system.c"
    "cmd_wifi/cmd_wifi.c"
    "cmd_init.c"
    )

idf_component_register(
SRCS    "${srcs}"

INCLUDE_DIRS    "cmd_nvs"
                "cmd_system"
                "cmd_wifi"
                "."

REQUIRES        console 
                nvs_flash
)
