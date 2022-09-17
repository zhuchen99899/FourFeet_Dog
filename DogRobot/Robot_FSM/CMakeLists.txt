

set(fsm_srcs 
    "fsm.c",
    "state_align.c",
    "state_stop.c",
    "state_gait.c"
    )


idf_component_register(SRCS "${fsm_srcs}"
                    INCLUDE_DIRS "")


#.c与.h文件引用



