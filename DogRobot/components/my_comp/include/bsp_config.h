#ifndef  __CONFIG_H__
#define __CONFIG_H__


/***********station模式配设**********/
#define ESP_WIFI_SSID  "Mi10"  //wifi名称
#define ESP_WIFI_PASSWORD  "zc950806"  //wifi密码
#define EXAMPLE_ESP_MAXIMUM_RETRY  1000 //wifi最大重连次数


/***********TCP连接配设************/
#define TCP_PORT 8899

/***********四足机器人机械结构定义*************/
#define LEG1_id4_L 68
#define LEG1_id0_L 80

/**************步态参数初始化*********************/
#define trot_supporting_phase_init


#define trot_swing_phase_s_init_Ts = 1;
#define trot_swing_phase_s_init_fai = 0.5;
#define trot_swing_phase_s_init_xs = -60;
#define trot_swing_phase_s_init_xf = 0;
#define trot_swing_phase_s_init_zs = 50;
#define trot_swing_phase_s_init_h = 30;


#define trot_swing_phase_s_init_Ts = 1;
#define trot_swing_phase_s_init_fai = 0.5;
#define trot_swing_phase_s_init_xs = -60;
#define trot_swing_phase_s_init_xf = 0;
#define trot_swing_phase_s_init_h = 30;




#endif

