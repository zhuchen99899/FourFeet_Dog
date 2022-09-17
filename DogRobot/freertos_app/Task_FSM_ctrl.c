#include "Task_FSM_ctrl.h"

FSM_t stFsm;	//定义状态机结构体

// static const char *TAG = "FSM_TASK"; //ESP 打印标签
/**************FSM控制任务***********************/
void Task_FSM_Ctrl(void * pvParameters)
{
extern QueueHandle_t Robot_FSM_STATE_Queue;
extern QueueHandle_t Gait_mode_Queue;
int fsm_val=-1;
int gait_mode=-1;
fsm_init(&stFsm,eventMap,actionMap);	//注册状态机


    while(1)
    {

    //PCA9685_pwm(1,0,SERVO090);
    xQueuePeek(Robot_FSM_STATE_Queue,&fsm_val,5 / portTICK_RATE_MS);

    //action_perfrom(&stFsmWeather,(void *)&a);
    switch (fsm_val)
    {
        
    case EVENT_Align:
        fsm_state_transfer(&stFsm,EVENT_Align);
        action_perfrom(&stFsm,NULL);  
        break;
    
    case EVENT_Stop_from_Align:
        fsm_state_transfer(&stFsm,EVENT_Stop_from_Align);
        action_perfrom(&stFsm,NULL);  
        break;

    case EVENT_Gait:
        fsm_state_transfer(&stFsm,EVENT_Gait);
        xQueuePeek(Gait_mode_Queue,&gait_mode,1 / portTICK_RATE_MS);
        action_perfrom(&stFsm,(void *)&gait_mode);

        break;
    default:
        break;
    }

          
        vTaskDelay(2 / portTICK_RATE_MS);
    
    }

}


