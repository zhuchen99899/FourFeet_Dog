#ifndef _TORT_H
#define _TORT_H
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "bsp_config.h"
#include "math.h"


typedef struct gait_time_state_s{
  float t;      //步态时间
  int Ts;       //周期
  float fai;    //支撑相占空比
}gait_time_state_t;


typedef struct trot_set_s
{
  float h;        //抬腿高度
  float speed;    //步频调节
  float Xf;       //终点位置
  float Xs;       //初始位置
  float Zs;       //Z轴起点位置
}trot_set_t ;

typedef struct  Dog_gait_result_s
{
float X0;
float Y0;
float X1;
float Y1;
float X2;
float Y2;
float X3;
float Y3;
}Dog_gait_result_t;

typedef struct Dog_gait_set_dir_s
{
float direction_0;
float direction_1;
float direction_2;
float direction_3;
} Dog_gait_set_dir_t;


void trot_init(void);


void trot_gait(gait_time_state_t *time_state,trot_set_t *trot_state,Dog_gait_result_t *Dog_gait_result,Dog_gait_set_dir_t *Dog_gait_set);
#endif
