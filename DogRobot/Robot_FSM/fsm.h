#ifndef __FSM_H
#define __FSM_H

#include "esp_log.h"
#include <stdio.h>
#include "state_stop.h"
#include "state_align.h"
#include "state_gait.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
//状态机状态表
typedef enum{
	STATE_Stop=0 ,      //四足停止.所有舵机还原初始角度
	STATE_Align,      //四足进行校准,此状态需要接触机器人舵机与四足连接完成
	STATE_Gait        //四足步态

}STATE_t; 

//状态机触发事件
typedef enum
{
	EVENT_Stop_from_Align = 0,
	EVENT_Align,
	EVENT_Stop_from_Gait,
	EVENT_Gait,
	EVENT_MAP_END
}EVENT_t;


//状态机动作函数指针
typedef void (*STATE_ACTION)(void *);	

/*状态机动作结构体*/
typedef struct ACTION_MAP
{
	STATE_t 		stStateID;
	STATE_ACTION 	EnterAct;	
	STATE_ACTION 	RunningAct;	
	STATE_ACTION 	ExitAct;
}ACTION_MAP_t;

/*定义状态机事件结构体*/
typedef struct EVENT_MAP
{
	EVENT_t	stEventID;
	STATE_t stCurState;
	STATE_t stNextState;
}EVENT_MAP_t;

/*定义状态机结构体类型*/
typedef struct FSM
{
	STATE_t stCurState;
	STATE_t stNextState;
	ACTION_MAP_t *pActionMap;
	EVENT_MAP_t *pEventMap;
}FSM_t;

extern ACTION_MAP_t actionMap[];
extern EVENT_MAP_t eventMap[];

void fsm_init(FSM_t* pFsm,EVENT_MAP_t* pEventMap,ACTION_MAP_t *pActionMap);
void fsm_state_transfer(FSM_t* pFsm, EVENT_t stEventID);
void action_perfrom(FSM_t* pFsm,void *parm);
#endif  
