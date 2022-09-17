#include "pca9685.h"
#include "math.h"

static const char *TAG = "pca9685";
/**
 *@brief PCA9685写寄存器
 *@param regAddr PCA9685寄存器地址
 *@param pData 写入数据 
 *@param dataLen 数据长度
 */
esp_err_t PCA9685_Write(uint8_t regAddr, uint8_t pData)
{
esp_err_t ret;
ret=i2c_wr_reg(PCA9685_adrr,regAddr,pData);
return ret;
}

/**
 @brief PCA9685读寄存器数据
 @param regAddr PCA9685寄存器地址
 @param pData 读出数据
 @param dataLen 读出数据长度
 @return 错误码
*/
uint8_t PCA9685_Read(uint8_t regAddr)
{
uint8_t data;
data=i2c_rd_reg(PCA9685_adrr, regAddr);
return data;
}


/**
 *@brief PCA9685_MODE1 寄存器组复位 
 *@param data 写入寄存器值
 */
void PCA9685_reset(void)
{
int ret;
ret=PCA9685_Write(PCA9685_MODE1,PCA9685_RESET);
if (ret==ESP_OK)  ESP_LOGI(TAG, "PCA9685_reset_success");
else ESP_LOGI(TAG, "PcCA9685_reset_fail");
}


/**
 * @brief PCA9685_PRE_SCALE控制频率寄存器
 * @param freq 写入频率
 * @note  向PRE_SCALE写数据的时候，MODE1的SLEEP位需要置1，也就是在休眠状态下修改频率才有效。
 */

void PCA9685_freq(float freq)
{
esp_err_t write_ret;
uint8_t oldmode=0;
uint8_t prescale,newmode;
uint8_t Mode1_state=0;
uint8_t freq_state=1;

double prescaleval;
//设置角度
//60度=0.5ms+（60/180）*（2.5ms-0.5ms）=1.1666ms
//利用占空比=1.1666ms/20ms=off/4096，off=239，50hz对应周期20ms
freq *= 0.92;//频率校准，一般要乘以0.915,否则会出现误差 
prescaleval = 25000000;
prescaleval /= 4096;
prescaleval /= freq;
prescaleval -= 1;
prescale =floor(prescaleval + 0.5f);

/*读取mode1寄存器并存储*/
//读取Mode1寄存器时需要发送主机需要发送Nack读取


oldmode=PCA9685_Read(PCA9685_MODE1);

/*设置MODE1 RESTART位 0 :禁止重启, 设置SLEEP位 1 :休眠状态*/
newmode = (oldmode&0x7F) | 0x10; // 向PRE_SCALE写数据的时候，MODE1的SLEEP位需要置1，也就是在休眠状态下修改频率才有效。

write_ret=PCA9685_Write(PCA9685_MODE1,newmode);
if (write_ret==ESP_OK)  ESP_LOGI(TAG, "PCA9685_Write_MODE1_Sleep_success");
else ESP_LOGI(TAG, "PCA9685_Write_MODE1_Sleep_fail");


write_ret=PCA9685_Write(PCA9685_PRESCALE,prescale);// 设置预分频器
if (write_ret==ESP_OK)  ESP_LOGI(TAG, "PCA9685_Write_MODE1_set_freq_success");
else ESP_LOGI(TAG, "PCA9685_Write_MODE1_set_freq_fail");
write_ret=PCA9685_Write(PCA9685_MODE1,oldmode);//还原 PCA9685 寄存器原先模式
if (write_ret==ESP_OK)  ESP_LOGI(TAG, "PCA9685_Write_MODE1_set_mode1_success");
else ESP_LOGI(TAG, "PCA9685_Write_MODE1_set_mode1_fail");

vTaskDelay(2 / portTICK_RATE_MS); //等待设置频率完成

/*设置MODE1 清除重启标志位RESTART :RESTART位，默认为0，为1的时候是重启状态，但是清空该状态位需要向该位写“1”而不是写“0”。
            AI 值位1 :AI是寄存器寻址自增设置，批量读写寄存器时用，打开后，读写PWM寄存器时，一次读写两个字节，比较方便。
            ALLCALL 位值1: PCA9685启用所有传呼LEDPWM总线地址。
*/
write_ret=PCA9685_Write(PCA9685_MODE1,oldmode|0xa1);

if (write_ret==ESP_OK)  ESP_LOGI(TAG, "PCA9685_Write_MODE1_set_mode1_restart_success");
else ESP_LOGI(TAG, "PCA9685_Write_MODE1_set_mode1_restart_fail");

Mode1_state=PCA9685_Read(PCA9685_MODE1);
ESP_LOGI(TAG, "Mode now %x" ,Mode1_state);

freq_state=PCA9685_Read(PCA9685_PRESCALE);
ESP_LOGI(TAG, "frq now %x", freq_state);
}

/*LEDn_ON和LEDn_OFF寄存器控制PWM占空比。简单来说，在一个PWM周期内，
LEDn_ON控制的是什么时候开，LEDn_OFF控制的是什么时候关。
两者的取值范围都是从0到4095，这两个寄存器在工作时会和一个持续计数（0到4095）的计数器进行比较。
*/
/**
 * @brief PCA9685修改角度函数
 * @param num:舵机PWM输出引脚0~15
 * @param on  :PWM上升计数值0~4096
 * @param off :PWM下降计数值0~4096
 * @note
一个PWM周期分成4096份，由0开始+1计数，计到on时跳变为高电平，继续计数到off时
跳变为低电平，直到计满4096重新开始。
所以当on不等于0时可作延时,当on等于0时，off/4096的值就是PWM的占空比。

//其中开始一般设置为0，只需要取下降时间就行
//该函数为设置PWM波的占空比，一般都以4096为一个PWM波周期
	----------------------------------------------------------------*/
void PCA9685_pwm(uint8_t num, uint32_t on, uint32_t off) 
{
			
			PCA9685_Write(LED0_ON_L+4*num,on);
			PCA9685_Write(LED0_ON_H+4*num,on>>8);
			PCA9685_Write(LED0_OFF_L+4*num,off);
			PCA9685_Write(LED0_OFF_H+4*num,off>>8);
}

	