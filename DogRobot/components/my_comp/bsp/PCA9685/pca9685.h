#ifndef __PCA9685_H
#define __PCA9685_H
#include "iic.h"



/**************PCA9685驱动器地址*******************/

#define PCA9685_adrr 0x40	//PCA9685从机地址分配：模块右上方不做短接时的地址

#define PCA9685_MODE1 0x00	//设置寄存器1
#define PCA9685_PRESCALE 0xFE	//控制周期寄存器
#define PCA9685_RESET 0x00 //mode1 reset

#define LED0_ON_L 0x06	//LED_ON_L
#define LED0_ON_H 0x07	//LED0_ON_H
#define LED0_OFF_L 0x08//LED0_OFF_L
#define LED0_OFF_H 0x09//LED0_OFF_H

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#define SERVOMIN  115 // 4096个脉冲中最小脉冲长度计数
#define SERVOMAX  590 // 4096个脉冲中最大脉冲长度计数

//  例如要求舵机转到60度，这么算，
//  60度对应的脉宽=0.5ms+(60/180)*(2.5ms-0.5ms)=1.1666ms
//  利用占空比=1.1666ms/20ms=off/4096,off=239,50hz对应周期20ms
//  PCA9685_setpwm(num,0,239);;;;当然也可以利用SERVO000和SERVO180计算
//34
#define SERVO000  103 //0度对应4096的脉宽计数值
#define SERVO015  137
#define SERVO030  171
#define SERVO045  205
#define SERVO060  239
#define SERVO075  273
#define SERVO090  307
#define SERVO105  341
#define SERVO120  375
#define SERVO135  409
#define SERVO150  443
#define SERVO165  477
#define SERVO180  512 //180度对应4096的脉宽计算值，四个值可根据不同舵机修改



esp_err_t PCA9685_Write(uint8_t regAddr, uint8_t pData);
uint8_t PCA9685_Read(uint8_t regAddr);

void PCA9685_reset(void);
void PCA9685_freq(float freq);
void PCA9685_pwm(uint8_t num, uint32_t on, uint32_t off);

#endif

