#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include "protocol_cmd.h"
#include "crc16.h"
#include "Align_Dog_protocol.h"
#include "Gait_protocol.h"
//状态机参数
#include "fsm.h"

//freertos相关
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <OS_AppObjCreate.h>
#include "trot.h"
//定义环形缓冲区总长度
#define PROT_FRAME_LEN_RECV 256









int32_t protocol_init(void);

void protocol_data_recv(uint8_t *data,uint16_t data_len);
int8_t receiving_process(void);

#endif