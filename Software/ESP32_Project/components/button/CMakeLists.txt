set(button_srcs
    "src/button.c"
    "src/bsp_button.c"
    )

idf_component_register(SRCS "${button_srcs}"
                       INCLUDE_DIRS "include"
                       )
