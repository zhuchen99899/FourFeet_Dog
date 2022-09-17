#include "fsm.h"




//建立动作表
ACTION_MAP_t actionMap[] = 
{
	
	{STATE_Stop,	state_stop_entry,	state_stop_do,	state_stop_exit},
	{STATE_Align,	state_align_entry,	state_align_do,	state_align_exit},
	{STATE_Gait,    state_gait_entry,   state_gait_do, state_gait_exit},
};

/*定义状态机流程图*/
EVENT_MAP_t eventMap[] = 
{
	{EVENT_Stop_from_Align,	STATE_Align,	STATE_Stop },
	{EVENT_Align,	        STATE_Stop,	    STATE_Align},	
	{EVENT_Stop_from_Gait,	STATE_Gait,	    STATE_Stop},	
	{EVENT_Gait,	        STATE_Stop,	    STATE_Gait},
	{EVENT_MAP_END,	0,	0},		
};



/**
* @brief 状态机初始化函数(注册函数)
* @param  pFsm //状态机对象结构体
* @param  pEventMap //状态机事件流程图
* @param  *pActionMap //状态机动作流程图
* @return  void
* @note 
*/

void fsm_init(FSM_t* pFsm,EVENT_MAP_t* pEventMap,ACTION_MAP_t *pActionMap)
{
extern QueueHandle_t Robot_FSM_STATE_Queue;
	int fsm_event=-1;
    //状态机初始化 
	pFsm->stCurState = STATE_Stop;
	pFsm->stNextState = EVENT_MAP_END;
	pFsm->pEventMap = eventMap; 
	pFsm->pActionMap = actionMap;

	xQueueOverwrite(Robot_FSM_STATE_Queue,&fsm_event);
}




/***
 * @brief  状态机转换函数
 * @param  pFsm 状态机对象结构体
 * @param  stEventId  触发的事件ID
 * @return
***/
void fsm_state_transfer(FSM_t* pFsm, EVENT_t stEventID)
{
	uint8_t i = 0;
	//遍历状态表
	for(i=0; pFsm->pEventMap[i].stEventID<EVENT_MAP_END; i++)
	{



		if((stEventID == pFsm->pEventMap[i].stEventID)
		&&(pFsm->stCurState == pFsm->pEventMap[i].stCurState))
            //存在事件且，当前状态符合流程图
		{			
            //指向状态机下一个状态切换
			pFsm->stNextState = pFsm->pEventMap[i].stNextState;
			
			return;
		}
	}	  
}

/***
 * @brief  状态机动作执行函数
 * @param  pFsm 状态机对象指针
 * @return
***/
void action_perfrom(FSM_t* pFsm,void *parm)
{
	int eventmap_end=EVENT_MAP_END;
	
    //当下一状态存在状态表中时
	if(eventmap_end !=pFsm->stNextState )
	{
        //退出动作
		pFsm->pActionMap[pFsm->stCurState].ExitAct(parm);
        //下一状态进入动作
		pFsm->pActionMap[pFsm->stNextState].EnterAct(parm);
		//触发状态切换
		pFsm->stCurState = pFsm->stNextState;
        /*退出状态转换过程，此时除非有事件发生，
        fsm_state_transfer使得pFsm->stNextState 重新指向一个范围内的
        状态。否则将一直触发下面else，使得状态机一直保持当前状态运行动作
        */
		pFsm->stNextState = EVENT_MAP_END;
	}
	else
	{
		
		//当状态机对象下一状态指针指向EVENT_MAP_END时,表示状态机处于运行态，不改变状态
		//运行当前状态机运行动作函数
		pFsm->pActionMap[pFsm->stCurState].RunningAct(parm);
	}
}
