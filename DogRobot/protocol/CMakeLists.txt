

set(protocol_srcs 
    "protocol.c",
    "Align_Dog_protocol.c",
    "Gait_protocol.c"

    )


idf_component_register(SRCS "${protocol_srcs}"
                    INCLUDE_DIRS "")


#.c与.h文件引用



