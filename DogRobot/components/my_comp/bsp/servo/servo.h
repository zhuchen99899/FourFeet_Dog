#ifndef __SERVO_H
#define __SERVO_H
#include "esp_log.h"
#include "pca9685.h"


void servo_input_angle(uint8_t num,float angle);

void servo_align_input(uint8_t num,float angle);
void Leg_eightFreedom_input(uint8_t Leg_id,float theta1,float theta2);
#define servo0_align  175
#define servo1_align  175
#define servo2_align  0
#define servo3_align  0
#define servo4_align  -6
#define servo5_align  -6
#define servo6_align  180
#define servo7_align  185
#endif
