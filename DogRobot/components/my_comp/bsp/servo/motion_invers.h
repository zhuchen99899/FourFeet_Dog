#ifndef _MOTION_INVERS_H
#define _MOTION_INVERS_H
#include "esp_log.h"

//使用math库中的pi变量等
#define _USE_MATH_DEFINES
#include "math.h"
#include "servo.h"
#include "bsp_config.h"

typedef enum {
    type_eight_freedom = 0,
    type_twelve_freedom
}Dog_type_t; 

typedef struct Dog_body_s
{
Dog_type_t myDog_type;
int harm_l1;
int shank_l2;
}Dog_body_t;

void Dog_struct_init(void);
int motion_invers(Dog_body_t Mydog_body,uint8_t Leg_ID, float X,float Y);
#endif 


