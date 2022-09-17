#include "Task_IDLE.h"

#include <OS_AppObjCreate.h>
// static const char *TAG = "IDLE_TASK"; //ESP 打印标签
/**************PWM控制任务***********************/
void Task_IDLE(void * pvParameters)
{
// extern  Dog_body_t My_dog_body; 
// float X=50,Y=80;
// float angles[1];
// int int_dataVal;


    while(1)
    {
    // xQueuePeek(Servo_angle_Queue,&int_dataVal,portMAX_DELAY);
	// ESP_LOGI(TAG,"PeekintData:%d",int_dataVal);
	
    //servo_input_angle(4,int_dataVal);
    //servo_align_input(0,int_dataVal);
    //servo_align_input(4,int_dataVal);




    // motion_invers(My_dog_body,X,Y,angles);
    // ESP_LOGI(TAG,"angles1=%f",angles[0]);
    // ESP_LOGI(TAG,"angles2=%f",angles[1]);
    vTaskDelay(1000 / portTICK_RATE_MS);
    
    
    }

}


