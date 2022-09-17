#include "Align_Dog_protocol.h"

static const char *TAG = "Dog_Align_protocol";
/**
  * @brief 小端模式的 float转 uint8_t
  * @param float
  * @return uch 转换后的单字节uch[3]数据
  * @note
  */
static uint8_t floatToUchar_L_Endian_change(float f)
{
FLOAT_CONV_t convFtoUch;
convFtoUch.f=f;
return convFtoUch.u_c[3];
}



/**
  * @brief 接收TCP舵机狗舵机控制报文
  * @param *frame_data
  * @return
  * @note 总共可以同时控制八组舵机(八自由度)
  */
void tcp_rec_Dog_servor_ctrl(uint8_t *frame_data)
{
extern QueueHandle_t Align_mode_Queue;
extern QueueHandle_t Align_Servo_angle_Queue;
extern SemaphoreHandle_t BinarySemaphore_Align_ctrl;	
Align_Servor_ctrl_t *Servor_ctrl;
extern Align_Servor_ctrl_t Servor_ctrl_base;
int Align_mode;
Servor_ctrl=&Servor_ctrl_base; //对消息队列传递指针初始化
uint32_t temp0 = COMPOUND_32BIT(&frame_data[12]);
float paraGroup_num_f=*(float *)&temp0;
uint8_t paraGroup_num= floatToUchar_L_Endian_change(paraGroup_num_f);
switch (paraGroup_num) {
case 1:
        {
            uint32_t temp1 = COMPOUND_32BIT(&frame_data[16]);
            uint32_t temp2 = COMPOUND_32BIT(&frame_data[20]);
            Align_mode=Align_mode_Servor;
            Servor_ctrl->paraGroup_num=paraGroup_num;
            Servor_ctrl->Servor_ID0=*(float *)&temp1;
            Servor_ctrl->Angle0=*(float *)&temp2;

            /*数据拷贝转移...*/
            
            ESP_LOGI(TAG, "paraGroup_num=%d",Servor_ctrl->paraGroup_num);
            ESP_LOGI(TAG, "Server_ID0=%f",Servor_ctrl->Servor_ID0);
            ESP_LOGI(TAG, "Angel0=%f",Servor_ctrl->Angle0);
            xQueueOverwrite(Align_mode_Queue,&Align_mode);
            
            xQueueOverwrite(Align_Servo_angle_Queue,(void *)&Servor_ctrl);
            xSemaphoreGive(BinarySemaphore_Align_ctrl);
            break;
        }
default:
    break;
}

}


/**
  * @brief 接收TCP舵机狗运动学逆解后腿部控制报文
  * @param *frame_data
  * @return
  * @note 总共可以同时控制四只腿部X,Y(八自由度)
  */
void tcp_rec_Dog_XY_ctrl(uint8_t *frame_data)
{
extern QueueHandle_t Align_mode_Queue;
extern QueueHandle_t Align_Leg_XY_Queue;
extern SemaphoreHandle_t BinarySemaphore_Align_ctrl;

Align_Leg_ctrl_t *Leg_ctrl;
extern Align_Leg_ctrl_t Leg_ctrl_base;
int Align_mode;
Leg_ctrl= &Leg_ctrl_base; //对消息队列传递指针初始化
uint32_t temp0 = COMPOUND_32BIT(&frame_data[12]);
float paraGroup_num_f=*(float *)&temp0;
uint8_t paraGroup_num= floatToUchar_L_Endian_change(paraGroup_num_f);
switch (paraGroup_num) {
case 1:
        {
            uint32_t temp1 = COMPOUND_32BIT(&frame_data[16]);
            uint32_t temp2 = COMPOUND_32BIT(&frame_data[20]);
            uint32_t temp3 = COMPOUND_32BIT(&frame_data[24]);
            Align_mode=Align_mode_Leg;
            Leg_ctrl->paraGroup_num=paraGroup_num;
            Leg_ctrl->Leg_ID1=*(float *)&temp1;
            Leg_ctrl->X1=*(float *)&temp2;
            Leg_ctrl->Y1=*(float *)&temp3;
            
            /*数据拷贝转移...*/
            
            ESP_LOGI(TAG, "paraGroup_num=%d",Leg_ctrl->paraGroup_num);
            ESP_LOGI(TAG, "Leg_ID1=%f",Leg_ctrl->Leg_ID1);
            ESP_LOGI(TAG, "X1=%f",Leg_ctrl->X1);
            ESP_LOGI(TAG, "Y1=%f",Leg_ctrl->Y1);

            xQueueOverwrite(Align_mode_Queue,&Align_mode);

            xQueueOverwrite(Align_Leg_XY_Queue,(void *)&Leg_ctrl);
            xSemaphoreGive(BinarySemaphore_Align_ctrl);
            break;
        }
default:
    break;
}

}



