#ifndef  __TCP_CONNECT_H
#define __TCP_CONNECT_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "sys/socket.h"
#include "esp_log.h"

#include <OS_AppObjCreate.h>
#include "bsp_config.h"
#include "wifi.h"

void Task_TCP_connect(void *pvPar);



#endif 
