#ifndef _Align_Dog_protocol_
#define _Align_Dog_protocol_

#include "esp_log.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <OS_AppObjCreate.h>

#include <string.h>
#include "protocol_cmd.h"

         typedef union FLOAT_CONV_S
                {
                 float f;
                 uint8_t u_c[4];
                }FLOAT_CONV_t;

void tcp_rec_Dog_servor_ctrl(uint8_t *frame_data);
void tcp_rec_Dog_XY_ctrl(uint8_t *frame_data);
#endif // DEBUG



