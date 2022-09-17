#ifndef  __GAIT_PROTOCOL_H
#define  __GAIT_PROTOCOL_H
#include "esp_log.h"
#include <stdio.h>
//状态机参数
#include "fsm.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <OS_AppObjCreate.h>

#include <string.h>
#include "protocol_cmd.h"
#include "trot.h"

    enum gait{
        leave_gait=0,
        trot,
        walk,
    };

    enum gait_dir_set{
        stop=0,
        forward,
        backward,
        left,
        right
    };
int tcp_rec_gait_mode_ctrl(uint8_t *frame_data);
trot_set_t tcp_rec_trot_set_ctrl(uint8_t *frame_data);
void tcp_rec_gait_dir_set_ctrl(uint8_t *frame_data);

#endif // ! __GAIT_PROTOCOL