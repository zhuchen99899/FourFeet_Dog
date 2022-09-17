#include "state_gait.h"
#include "trot.h"
#include "motion_invers.h"
static const char *TAG = "state_gait"; //ESP 打印标签


extern gait_time_state_t gait_time_state;
extern Dog_gait_result_t Dog_gait_result_data;
extern Dog_body_t My_dog_body; 

//消息队列句柄
extern QueueHandle_t Gait_trot_set_Queue;
extern QueueHandle_t Dog_gait_set_dir_Queue;
void state_gait_entry(void *parm)
{
	ESP_LOGI(TAG, "state_gait_entry");
	motion_invers(My_dog_body,0, 0,130);
	motion_invers(My_dog_body,1, 0,130);
	motion_invers(My_dog_body,2, 0,130);
	motion_invers(My_dog_body,3, 0,130);
}
void state_gait_do(void *parm)
{


	trot_set_t *trot_state_data;
	Dog_gait_set_dir_t *Dog_gait_dir_data;

	int *gait_mode;
	gait_mode=(int*)parm;
	switch (*gait_mode)
	{
	case trot:


	//获取队列数据指针
	xQueuePeek(Gait_trot_set_Queue,(void *)&trot_state_data,5 / portTICK_RATE_MS);
	xQueuePeek(Dog_gait_set_dir_Queue,(void *)&Dog_gait_dir_data,5 / portTICK_RATE_MS);

	// ESP_LOGI(TAG, "t=%f",gait_time_state.t);
	// ESP_LOGI(TAG, "h=%f",trot_state_data->h);
	// ESP_LOGI(TAG, "Zs=%f",trot_state_data->Zs);
	// ESP_LOGI(TAG, "direction_0=%f",Dog_gait_dir_data->direction_0);
	// ESP_LOGI(TAG, "direction_3=%f",Dog_gait_dir_data->direction_3);
	
	//执行步态
	trot_gait(&gait_time_state,trot_state_data,&Dog_gait_result_data,Dog_gait_dir_data);

	//逆解运算并输出舵机
	motion_invers(My_dog_body,0, Dog_gait_result_data.X0,Dog_gait_result_data.Y0);
	motion_invers(My_dog_body,1, Dog_gait_result_data.X1,Dog_gait_result_data.Y1);
	motion_invers(My_dog_body,2, Dog_gait_result_data.X2,Dog_gait_result_data.Y2);
	motion_invers(My_dog_body,3, Dog_gait_result_data.X3,Dog_gait_result_data.Y3);
		break;
	
	default:
		break;
	}


}
void state_gait_exit(void *parm)
{
	ESP_LOGI(TAG, "state_gait_exit");
	motion_invers(My_dog_body,0, 0,130);
	motion_invers(My_dog_body,1, 0,130);
	motion_invers(My_dog_body,2, 0,130);
	motion_invers(My_dog_body,3, 0,130);
}