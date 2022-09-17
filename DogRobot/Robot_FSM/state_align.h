#ifndef __STATE_ALIGN_H
#define __STATE_ALIGN_H
#include "stdio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

void state_align_entry(void *parm);
void state_align_do(void *parm);
void state_align_exit(void *parm);



#endif
