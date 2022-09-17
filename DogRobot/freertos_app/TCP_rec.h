#ifndef __TCP_REC_H
#define __TCP_REC_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "sys/socket.h" //rec头文件
#include "string.h" //memset 支持
#include "wifi.h"
#include "protocol.h"
#include "stdlib.h" //atoi 字符串转换10进制支持
void Task_TCP_rec (void *pvPar);




#endif 
