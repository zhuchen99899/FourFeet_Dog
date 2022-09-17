#ifndef  __INCLUDEDS_H__
#define __INCLUDEDS_H__

#include <stdio.h>          //添加头文件 用于调用串口输出函数printf
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_system.h"

/***********设置**********/
#include "bsp_config.h" //驱动设置

/***********组件**********/
#include "bsp_init.h" //驱动初始化
#include "my_comp.h"
#include "wifi.h"
#include "iic.h"

/**********通讯协议缓冲**********/
#include "protocol.h"

/**********四足状态机************/
#include "motion_invers.h"//运动学逆解库


/***********freertos 任务**********/
//通讯机制创建
#include "OS_AppObjCreate.h"

//freertos_app

#include "TCP_rec.h"
#include "TCP_connect.h"
#include "Task_FSM_ctrl.h"
#endif 
