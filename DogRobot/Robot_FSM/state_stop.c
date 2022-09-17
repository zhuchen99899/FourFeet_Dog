#include "state_stop.h"

static const char *TAG = "state_align"; //ESP 打印标签
void state_stop_entry(void *parm)
{
	ESP_LOGI(TAG, "state_stop_entry");
}
void state_stop_do(void *parm)
{
	//ESP_LOGI(TAG, "state_stop_do");
}
void state_stop_exit(void *parm)
{
	ESP_LOGI(TAG, "state_stop_exit");
}
