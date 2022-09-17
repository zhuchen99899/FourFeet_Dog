
#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "trot.h"
/********************消息队列定义***********************/



/********************事件标志组定义***********************/

/*事件组允许每个事件使用多个位，但我们只关心两个事件：
*我们通过IP连接到AP
*我们在最大重试次数后连接失败*/
#define WIFI_CONNECTED_BIT 0x00000001
#define WIFI_FAIL_BIT     0x00000002



enum Align_mode_t{
Align_mode_Servor=0,
Align_mode_Leg

};

typedef struct Align_Servor_ctrl_s{
uint8_t paraGroup_num;
float Servor_ID0;
float Angle0;
float Servor_ID1;
float Angle1;
float Servor_ID2;
float Angle2;
float Servor_ID3;
float Angle3;
float Servor_ID4;
float Angle4;
float Servor_ID5;
float Angle5;
float Servor_ID6;
float Angle6;
float Servor_ID7;
float Angle7;
}Align_Servor_ctrl_t;


typedef struct Align_Leg_ctrl_s{
uint8_t paraGroup_num;
float Leg_ID0;
float X0;
float Y0;
float Leg_ID1;
float X1;
float Y1;
float Leg_ID2;
float X2;
float Y2;
float Leg_ID3;
float X3;
float Y3;
}Align_Leg_ctrl_t;


void OS_AppObjCreate(void);



#endif

