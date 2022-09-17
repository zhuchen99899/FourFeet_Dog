

set(freertos_app_srcs 
    "TCP_rec.c"
    "TCP_connect.c"
    "Task_FSM_ctrl.c"
    "Task_IDLE.h"
    "OS_AppObjCreate.c"
    )


idf_component_register(SRCS "${freertos_app_srcs}"
                    INCLUDE_DIRS "")



#.c与.h文件引用
