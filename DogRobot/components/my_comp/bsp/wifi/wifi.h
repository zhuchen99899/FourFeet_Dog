#ifndef  __WIFI_H__
#define __WIFI_H__
//freertos 相关
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


//esp 硬件库
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

//lwip 相关
#include "lwip/err.h"
#include "lwip/sys.h"
#include "sys/socket.h"
//驱动配设
#include "bsp_config.h"


//freertos 通讯机制建立相关
#include <OS_AppObjCreate.h>

void wifi_connect_STA(void);

int get_socket_error_code(int socket);
int show_socket_error_reason(const char *str, int socket);

#endif