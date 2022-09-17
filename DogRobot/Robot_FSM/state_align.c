#include "state_align.h"
#include <OS_AppObjCreate.h>
#include "motion_invers.h"
#include "servo.h"
static const char *TAG = "state_align"; //ESP 打印标签

BaseType_t Semaphore_state=pdFALSE;

BaseType_t testqueue_state1=pdFALSE;
BaseType_t testqueue_state=pdFALSE;
extern QueueHandle_t Align_mode_Queue;
extern QueueHandle_t Align_Servo_angle_Queue;
extern QueueHandle_t Align_Leg_XY_Queue;

extern SemaphoreHandle_t BinarySemaphore_Align_ctrl;	

extern Dog_body_t My_dog_body; 
void state_align_entry(void *parm)
{
	ESP_LOGI(TAG, "state_align_entry");
}
void state_align_do(void *parm)
{	
	//控制参数从消息队列中取出
	Align_Servor_ctrl_t  *FSM_Servor_ctrl;
	Align_Leg_ctrl_t *FSM_Leg_ctrl;

	int Align_mode=-1;
	
	//获得舵机控制信号量，当协议收到对应报文时会发送

	Semaphore_state=xSemaphoreTake(BinarySemaphore_Align_ctrl,5 / portTICK_RATE_MS);
	
	 if(Semaphore_state==pdTRUE)
	 {	
	testqueue_state=xQueuePeek(Align_Leg_XY_Queue,(void *)&FSM_Leg_ctrl,5 / portTICK_RATE_MS);
	testqueue_state1=xQueuePeek(Align_Servo_angle_Queue,(void *)&FSM_Servor_ctrl,5 / portTICK_RATE_MS);	
	xQueuePeek(Align_mode_Queue,&Align_mode,5 / portTICK_RATE_MS);

		 ESP_LOGI(TAG, "获得控制信号量Align_mode=%d",Align_mode);

		 if(Align_mode==Align_mode_Servor){
			 //舵机控制
			ESP_LOGI(TAG, "舵机控制模式");
			switch (FSM_Servor_ctrl->paraGroup_num)
			{
			case 1:
			ESP_LOGI(TAG, "testqueue_state1=%d",testqueue_state1);
			ESP_LOGI(TAG, "FSM_Servor_ID0=%d",(uint8_t)FSM_Servor_ctrl->Servor_ID0);
			ESP_LOGI(TAG, "FSM_Angle0=%f",FSM_Servor_ctrl->Angle0);
			//servo_input_angle((uint8_t) FSM_Servor_ctrl->Servor_ID0,FSM_Servor_ctrl->Angle0);
			servo_align_input((uint8_t) FSM_Servor_ctrl->Servor_ID0,FSM_Servor_ctrl->Angle0);    
			break;
			
			default:
				break;
			}
		 }

			
		 if(Align_mode==Align_mode_Leg){
		//足端控制
	 	
			ESP_LOGI(TAG, "足端控制模式");
			switch (FSM_Leg_ctrl->paraGroup_num)
			{//单腿控制
			case 1:
			 ESP_LOGI(TAG, "testqueue_state=%d",testqueue_state);
			
			ESP_LOGI(TAG, "fsm_Leg_ID1=%f",FSM_Leg_ctrl->Leg_ID1);
			ESP_LOGI(TAG, "fsm_X1=%f",FSM_Leg_ctrl->X1);
			ESP_LOGI(TAG, "fsm_Y1=%f",FSM_Leg_ctrl->Y1);

			motion_invers(My_dog_body,(uint8_t)FSM_Leg_ctrl->Leg_ID1,FSM_Leg_ctrl->X1,FSM_Leg_ctrl->Y1);

			
				break;
			

			default:

				break;
			


			}//switch


		 }//if
	 }


		 


	

}

void state_align_exit(void *parm)
{
	ESP_LOGI(TAG, "state_align_exit");
}
