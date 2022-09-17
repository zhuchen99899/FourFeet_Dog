#ifndef __TASK_IDLE_H
#define __TASK_IDLE_H


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "pca9685.h"

#include "servo.h"
#include "motion_invers.h"

void Task_IDLE(void * pvParameters);



#endif

