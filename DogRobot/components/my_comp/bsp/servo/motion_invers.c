#include "motion_invers.h"



static const char* TAG = "invers";
Dog_body_t My_dog_body; 

void Dog_struct_init(void){
My_dog_body.myDog_type=type_eight_freedom;
My_dog_body.shank_l2=LEG1_id4_L;
My_dog_body.harm_l1=LEG1_id0_L;
};



/**
 * @brief 单腿运动学逆解
 * @param Mydog_body :舵狗形体，机械结构数据
 * @param Leg_ID: 腿部编号
 * @param X  输入的坐标X
 * @param Y  输入的坐标Y
 * @param angle[2] :分解的舵机角度(八自由度)
 * @return void
 * @note
 * ***********/

int motion_invers(Dog_body_t Mydog_body,uint8_t Leg_ID, float X,float Y)
{
 float shank,harm,fail=0;

if(X>124||X<-136||Y>136||Y<0)
{
 ESP_LOGI(TAG, "X OR Y overflow");
}
else{
switch (Mydog_body.myDog_type)
{
case type_eight_freedom:
    shank=M_PI-acos((X*X+Y*Y-Mydog_body.harm_l1*Mydog_body.harm_l1
    -Mydog_body.shank_l2*Mydog_body.shank_l2)/(-2*Mydog_body.harm_l1*Mydog_body.shank_l2));
  
    fail=acos((X*X+Y*Y+Mydog_body.harm_l1*Mydog_body.harm_l1-Mydog_body.shank_l2*Mydog_body.shank_l2)/(
    2*Mydog_body.harm_l1*sqrt(X*X+Y*Y)));


    if(X>0)
    {
        harm=fabs(atan(Y/X))-fail; //必须使用fabs，abs输入为int型，会丢弃小数点后的位

    }
    else if(X<0)
    {
      
      harm=M_PI-fabs(atan(Y/X))-fail;
    }
    else
    {
    harm=M_PI_2-fail;
    }

    shank=(180/M_PI)*shank;//theta2
    harm=(180/M_PI)*harm;//theta1
    Leg_eightFreedom_input(Leg_ID,harm,shank);
    return 0;

    break;

default:
    break;
}//switch


}//endif

return -1;   
}