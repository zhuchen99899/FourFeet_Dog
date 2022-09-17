#include <OS_AppObjCreate.h>

static const char *TAG = "OS_APPCreate"; //ESP 打印标签
/********************消息队列创建********************/
//消息队列队列深度
#define Robot_fsm_state_Q_NUM 1  
#define Align_Servo_angle_Q_NUM 1
#define Align_Leg_XY_Q_NUM  1
#define Gait_trot_set_Q_NUM 1
#define Dog_gait_set_dir_Q_NUM 1 
#define Align_Mode_Q_NUM 1
#define Gait_Mode_Q_NUM 1
//消息队列结构体内存(消息队列中传递指针,为浅拷贝) 结构体必须在全局区，栈区(函数内)创建内存会导致浅拷贝问题
Align_Servor_ctrl_t Servor_ctrl_base;
Align_Leg_ctrl_t Leg_ctrl_base;

//状态机事件消息队列创建
static void Robot_FSM_STATE_QueueCreate(void)
{
extern QueueHandle_t Robot_FSM_STATE_Queue;
	
Robot_FSM_STATE_Queue =xQueueCreate(Robot_fsm_state_Q_NUM,sizeof(int));
	if (Robot_FSM_STATE_Queue==0)
	{
	/*消息创建失败处理机制*/
  ESP_LOGE(TAG, "状态机事件消息队列创建失败\r\n");
	}
	else 
	{

	/*消息创建成功处理机制*/
  ESP_LOGI(TAG, "状态机事件消息队列创建成功\r\n");
	}

}

static void Align_Servo_angle_ctrl_QueueCreate(void)
{
	extern QueueHandle_t Align_Servo_angle_Queue;
	Align_Servo_angle_Queue =xQueueCreate(Align_Servo_angle_Q_NUM,sizeof(struct Align_Servor_ctrl_s *));
	if (Align_Servo_angle_Queue==0)
	{
	/*消息创建失败处理机制*/
  	ESP_LOGE(TAG, "舵机转动角度控制消息队列创建失败\r\n");
	}
	else 
	{
	/*消息创建成功处理机制*/
  	ESP_LOGI(TAG, "舵机转动角度控制消息队列创建成功\r\n");
	}

}

static void Align_Leg_XY_ctrl_QueueCreate(void)
{
	extern QueueHandle_t Align_Leg_XY_Queue;
	Align_Leg_XY_Queue =xQueueCreate(Align_Leg_XY_Q_NUM,sizeof(struct Align_Leg_ctrl_s *));
	if (Align_Leg_XY_Queue==0)
	{
	/*消息创建失败处理机制*/
  	ESP_LOGE(TAG, "足端控制消息队列创建失败\r\n");
	}
	else 
	{
	/*消息创建成功处理机制*/
  	ESP_LOGI(TAG, "足端控制消息队列创建成功\r\n");
	}

}

static void Gait_trot_set_QueueCreate(void)
{
	extern QueueHandle_t Gait_trot_set_Queue;
	Gait_trot_set_Queue =xQueueCreate(Gait_trot_set_Q_NUM,sizeof(struct trot_gait_s *));
	if (Gait_trot_set_Queue==0)
	{
	/*消息创建失败处理机制*/
  	ESP_LOGE(TAG, "trot步态参数设置消息队列创建失败\r\n");
	}
	else 
	{
	/*消息创建成功处理机制*/
  	ESP_LOGI(TAG, "trot步态参数设置消息队列创建成功\r\n");
	}

}

static void Dog_gait_set_dir_QueueCreate(void)
{
	extern QueueHandle_t Dog_gait_set_dir_Queue;
	Dog_gait_set_dir_Queue =xQueueCreate(Dog_gait_set_dir_Q_NUM,sizeof(struct Dog_gait_set_s *));
	if (Dog_gait_set_dir_Queue==0)
	{
	/*消息创建失败处理机制*/
  	ESP_LOGE(TAG, "步态方向参数设置消息队列创建失败\r\n");
	}
	else 
	{
	/*消息创建成功处理机制*/
  	ESP_LOGI(TAG, "步态方向参数设置消息队列创建成功\r\n");
	}

}

static void Align_mode_QueueCreate(void)
{
extern QueueHandle_t Align_mode_Queue;
	Align_mode_Queue =xQueueCreate(Align_Mode_Q_NUM,sizeof(int));
	if (Align_mode_Queue==0)
	{
	/*消息创建失败处理机制*/
  	ESP_LOGE(TAG, "校准模式消息队列创建失败\r\n");
	}
	else 
	{
	/*消息创建成功处理机制*/
  	ESP_LOGI(TAG, "校准模式消息队列创建成功\r\n");
	}
	
}


static void Gait_mode_QueueCreate(void)
{
extern QueueHandle_t Gait_mode_Queue;
	Gait_mode_Queue =xQueueCreate(Gait_Mode_Q_NUM,sizeof(int));
	if (Gait_mode_Queue==0)
	{
	/*消息创建失败处理机制*/
  	ESP_LOGE(TAG, "步态模式消息队列创建失败\r\n");
	}
	else 
	{
	/*消息创建成功处理机制*/
  	ESP_LOGI(TAG, "步态模式消息队列创建成功\r\n");
	}
	
}



static void QueueCreate(void)
{
Robot_FSM_STATE_QueueCreate();
Align_Servo_angle_ctrl_QueueCreate();
Align_Leg_XY_ctrl_QueueCreate();
Align_mode_QueueCreate();
Gait_mode_QueueCreate();
Gait_trot_set_QueueCreate();
Dog_gait_set_dir_QueueCreate();

}





/********************信号量创建********************/


static void	BinarySemaphore_Align_ctrl_Create(void){
	extern SemaphoreHandle_t BinarySemaphore_Align_ctrl;
	BinarySemaphore_Align_ctrl=xSemaphoreCreateBinary();	
	if (BinarySemaphore_Align_ctrl==0)
	{
	/*Align_Servor_ctrl信号量创建失败*/
  	ESP_LOGE(TAG, "Align_Servor_ctrl信号量创建失败\r\n");
	}
	else 
	{
	/*Align_ctrl信号量创建成功*/
  	ESP_LOGI(TAG, "Align_ctrl信号量创建成功\r\n");
	}
}


//二值信号量创建
static void BinarySemaphoreCreate(void)
{
BinarySemaphore_Align_ctrl_Create();

}

//信号量创建
static void SemaphoreCreate(void)
 {
BinarySemaphoreCreate();

};

/********************事件标志组创建********************/
static void EventGroupCreate(void)
{
  extern EventGroupHandle_t s_wifi_event_group;

   s_wifi_event_group = xEventGroupCreate();//创建事件标志组

};


 

//Freertos 通讯机制创建
void OS_AppObjCreate(void)
{
//消息队列创建
QueueCreate();

//信号量创建
SemaphoreCreate();

//事件标志组创建	
EventGroupCreate();	

}




