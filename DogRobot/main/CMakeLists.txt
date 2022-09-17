
set(srcs 
    "user_app_main.c"
    "bsp_init.c"
    )

set(includes "."

    )

idf_component_register(SRCS "srcs"
                    INCLUDE_DIRS "includes")
