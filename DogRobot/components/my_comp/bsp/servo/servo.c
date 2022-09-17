#include "servo.h"

static const char *TAG = "servo";
/**
 * @brief PCA9685修改角度函数
 * @param num:舵机PWM输出引脚0~15
 * @param angle :角度
 * @return void
 * @note
 * ***********/

void servo_input_angle(uint8_t num,float angle)
{
uint32_t off_val; 

off_val= SERVO000+angle*34/15;
ESP_LOGI(TAG, "off_val=%d",off_val);    
PCA9685_pwm(num,0,off_val);

};


/**
 * @brief 舵机校准后角度
 * @param num:舵机PWM输出引脚0~15
 * @param angle :角度
 * @return void
 * @note
 * ***********/

void servo_align_input(uint8_t num,float angle)
{
switch(num)
    {
        case 0://运动范围 0~175
            if((0<=angle) && (angle<=175)){
            servo_input_angle(0,-angle+servo0_align);
            }
            else{
            ESP_LOGI(TAG, "ERRO:servo0 out of coverage");    
            }
        
        break;

        case 1:// 运动范围 0~175
            if((0<=angle) && (angle<=175)){
            servo_input_angle(1,-angle+servo1_align);
            }
            else{
            ESP_LOGI(TAG, "ERRO:servo1 out of coverage");    
            }
        break;
        
        case 2://运动范围 0~175
            if((0<=angle) && (angle<=175)){
            servo_input_angle(2,angle+servo2_align);
            }
            else{
            ESP_LOGI(TAG, "ERRO:servo2 out of coverage");    
            }
        break;

        case 3://运动范围 0~175
            if((0<=angle) && (angle<=175)){
            servo_input_angle(3,angle+servo3_align);
            }
            else{
            ESP_LOGI(TAG, "ERRO:servo3 out of coverage");    
            }
        break;

        case 4://运动范围 45~145
            if((45<=angle)&&(angle<=145)){
            servo_input_angle(4,angle+servo4_align);
            }
            else{
            ESP_LOGI(TAG, "ERRO:servo4 out of coverage");    
            }
        break;

        case 5://运动范围45~145
            if((45<=angle)&&(angle<=145)){
            servo_input_angle(5,angle+servo5_align);
            }
            else{
            ESP_LOGI(TAG, "ERRO:servo5 out of coverage");    
            }
        break;

        case 6://运动范围45~145
            if((45<=angle)&&(angle<=145)){
            servo_input_angle(6,-angle+servo6_align);
            }
            else{
            ESP_LOGI(TAG, "ERRO:servo6 out of coverage");    
            }
        break;

        case 7://运动范围45~145
            if((45<=angle)&&(angle<=145)){
            servo_input_angle(7,-angle+servo7_align);
            }
            else{
            ESP_LOGI(TAG, "ERRO:servo7 out of coverage");    
            }
        break;


        default:
        break;
    }

}

/**
 * @brief 舵机校准单腿运动
 * @param Leg_id: 腿部编号
 * @param theta1 :大腿角度
 * @param theta2 :小腿角度
 * @return void
 * @note
 * ***********/

void Leg_eightFreedom_input(uint8_t Leg_id,float theta1,float theta2)
{
    switch (Leg_id)
    {
    case 0:
        servo_align_input(0,theta1);
        servo_align_input(4,theta2);
        break;
    case 1:
        servo_align_input(1,theta1);
        servo_align_input(5,theta2);
        break;
    case 2:
        servo_align_input(2,theta1);
        servo_align_input(6,theta2);
        break;
    case 3:
        servo_align_input(3,theta1);
        servo_align_input(7,theta2);
        break;
    default:
        break;
    }

}