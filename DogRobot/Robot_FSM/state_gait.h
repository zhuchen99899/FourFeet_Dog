#ifndef __STATE_GAIT_H
#define __STATE_GAIT_H

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "Gait_protocol.h"


void state_gait_entry(void *parm);
void state_gait_do(void *parm);
void state_gait_exit(void *parm);







#endif // !__FSM_GAIT_H

