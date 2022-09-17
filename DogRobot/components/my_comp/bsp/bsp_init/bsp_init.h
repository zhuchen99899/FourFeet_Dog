#ifndef  __BSP_INIT_H__
#define __BSP_INIT_H__
#include <stdio.h>          //添加头文件 用于调用串口输出函数printf
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "wifi.h"
#include "iic.h"
#include "pca9685.h"
#include "motion_invers.h"
#include "bsp_config.h"
#include "trot.h"



void bsp_init(void);

void Dog_struct_init(void);



#endif