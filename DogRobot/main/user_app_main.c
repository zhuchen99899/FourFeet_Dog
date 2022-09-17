#include "includes.h"



//创建消息队列句柄

QueueHandle_t Align_Leg_XY_Queue;
QueueHandle_t Robot_FSM_STATE_Queue;
QueueHandle_t Align_Servo_angle_Queue;
QueueHandle_t Align_Leg_XY_Queue;
QueueHandle_t Align_mode_Queue;
QueueHandle_t Gait_mode_Queue;
QueueHandle_t Gait_trot_set_Queue;
QueueHandle_t Dog_gait_set_dir_Queue;
//创建信号量句柄
SemaphoreHandle_t BinarySemaphore_Align_ctrl;
//创建事件标志组句柄
EventGroupHandle_t s_wifi_event_group;//事件标志组句柄

/*创建任务句柄*/
TaskHandle_t xHandle_start_task = NULL;



//TCP连接任务
#define TCP_conncet_STK_NAME "TCP_conncet"
#define TCP_connect_STK_SIZE  2048
#define TCP_connect_STK_PRIO 2
TaskHandle_t xHandle__tcp_connect_task=NULL;
extern void Task_TCP_connect(void * pvParameters);




//FSM任务
#define FSM_STK_NAME "FSM_Control"
#define FSM_STK_SIZE  4096
#define FSM_STK_PRIO   3
TaskHandle_t xHandle__FSM_task=NULL;
extern void Task_FSM_Ctrl(void * pvParameters);

//空闲任务
#define IDLE_STK_NAME   "IDLE_TASK"
#define IDLE_STK_SIZE   2048
#define IDLE_STK_PRIO   0
TaskHandle_t xHandle__IDLE_task=NULL;
extern void Task_IDLE(void * pvParameters);

//启动任务
void vStartTask1(void * pvParameters)
{

    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

//vPortCPUInitializeMutex(&mux);

 taskENTER_CRITICAL(&mux);    //进入临界区
    //创建TCP任务
    xTaskCreatePinnedToCore(Task_TCP_connect,             //任务函数
                           TCP_conncet_STK_NAME,            //任务名称
                            TCP_connect_STK_SIZE,               //堆栈大小
                            NULL,               //传递参数
                            TCP_connect_STK_PRIO,                  //任务优先级
                            &xHandle__tcp_connect_task,      //任务句柄
                            tskNO_AFFINITY);    //无关联，不绑定在任何一个核上

    //创建FSM任务
    xTaskCreatePinnedToCore(Task_FSM_Ctrl,             //任务函数
                            FSM_STK_NAME,          //任务名称
                            FSM_STK_SIZE,         //堆栈大小
                            NULL,                         //传递参数
                            FSM_STK_PRIO,         //任务优先级
                            &xHandle__FSM_task,   //任务句柄
                            tskNO_AFFINITY);   //无关联，不绑定在任何一个核上
    //创建空闲任务
    xTaskCreatePinnedToCore(Task_IDLE,        //任务函数
                            IDLE_STK_NAME,          //任务名称
                            IDLE_STK_SIZE,         //堆栈大小
                            NULL,                         //传递参数
                            IDLE_STK_PRIO,         //任务优先级
                            &xHandle__IDLE_task,   //任务句柄
                            tskNO_AFFINITY);   //无关联，不绑定在任何一个核上
    //删除自身任务
    vTaskDelete(xHandle_start_task);      




  taskEXIT_CRITICAL(&mux); //退出临界区

}


//   vPortExitCritical(&mux);        //退出临界区  
// vPortEnterCritical(&mux);

/*创建Start任务函数*/
void vCreate_Start_task(void)
{

    //创建Start任务
    xTaskCreatePinnedToCore(vStartTask1,             //任务函数
                            "start_task",            //任务名称
                            2048,               //堆栈大小
                            NULL,               //传递参数
                            1,                  //任务优先级
                            &xHandle_start_task,     //任务句柄
                            tskNO_AFFINITY);    //无关联，不绑定在任何一个核上
}

/*主函数*/
void app_main(void)
{
    
    OS_AppObjCreate();//freertos通讯机制创建
    bsp_init();
    protocol_init(); //通讯协议环形缓冲区初始化
    Dog_struct_init(); //四足结构相关系初始化

	//调用函数创建启动任务
    vCreate_Start_task();
}
