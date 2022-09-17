#include "trot.h"

gait_time_state_t gait_time_state;
Dog_gait_result_t Dog_gait_result_data;


trot_set_t trot_set_data;
Dog_gait_set_dir_t Dog_gait_set_dir_data;

extern QueueHandle_t Gait_trot_set_Queue;
extern QueueHandle_t Dog_gait_set_dir_Queue;
static const char *TAG = "trot";

static void gait_time_state_init(void)
{
gait_time_state.t=0.00;//步态时间(初值)   
gait_time_state.Ts=1;       //周期
gait_time_state.fai=0.5;    //支撑相占空比
}

static void Dog_gait_result_init(void)
{
Dog_gait_result_data.X0=0.0;
Dog_gait_result_data.Y0=0.0;
Dog_gait_result_data.X1=0.0;
Dog_gait_result_data.Y1=0.0;
Dog_gait_result_data.X2=0.0;
Dog_gait_result_data.Y2=0.0;
Dog_gait_result_data.X3=0.0;
Dog_gait_result_data.Y3=0.0;
}

static void trot_state_init(void)
{

  trot_set_t *trot_set_P;
  trot_set_P=&trot_set_data; //给指针指向全局的结构体
  trot_set_P->Xs=-30;       //初始位置
  trot_set_P->Xf=40.0;       //终点位置
  trot_set_P->Zs=85.0;       //Z轴起点位置
  trot_set_P->h=-35.0;        //抬腿高度 
  trot_set_P->speed=0.12;//步频调节
  //消息队列传递指针 会对指针进行拷贝
  xQueueOverwrite(Gait_trot_set_Queue,(void *)&trot_set_P);
}


static void Dog_gait_set_init(void){
Dog_gait_set_dir_t *Dog_gait_dir_P;
Dog_gait_dir_P=&Dog_gait_set_dir_data;//指针初始化
Dog_gait_dir_P->direction_0=-1.0;
Dog_gait_dir_P->direction_1=-1.0;
Dog_gait_dir_P->direction_2=-1.0;
Dog_gait_dir_P->direction_3=-1.0;
  //消息队列传递指针 会对指针进行拷贝
  xQueueOverwrite(Dog_gait_set_dir_Queue,(void *)&Dog_gait_dir_P);

}


void trot_init(void)
{
gait_time_state_init();
Dog_gait_result_init();

trot_state_init();
Dog_gait_set_init();
}



/**
 * @brief 步态函数
 * 
 * @param trot_state 
 * @param Dog_gait_result 
 * @param Dog_gait_set 
 */
void trot_gait(gait_time_state_t *time_state,trot_set_t *trot_state,Dog_gait_result_t *Dog_gait_result,Dog_gait_set_dir_t *Dog_gait_set)
{
    float sigma;
    float zep;
    float xep_Stand;
    float xep_Swing;

    //前半周期
    if(time_state->t<=time_state->Ts*time_state->fai)
    {
    ESP_LOGI(TAG, "前半周期");  
    sigma=2*M_PI*time_state->t/(time_state->fai*time_state->Ts);
    zep=trot_state->h*(1-cos(sigma))/2;
    xep_Swing=(trot_state->Xf-trot_state->Xs)*((sigma-sin(sigma)))/(2*M_PI)+trot_state->Xs;
    xep_Stand=(trot_state->Xs-trot_state->Xf)*((sigma-sin(sigma)))/(2*M_PI)+trot_state->Xf;
    
    //输出坐标
    
    Dog_gait_result->X0=Dog_gait_set->direction_0*xep_Stand;
    Dog_gait_result->X1=Dog_gait_set->direction_1*xep_Swing;
    Dog_gait_result->X2=Dog_gait_set->direction_2*xep_Stand;
    Dog_gait_result->X3=Dog_gait_set->direction_3*xep_Swing;


    Dog_gait_result->Y0=trot_state->Zs;
    Dog_gait_result->Y1=trot_state->Zs+zep;
    Dog_gait_result->Y2=trot_state->Zs;
    Dog_gait_result->Y3=trot_state->Zs+zep;
    //锁定姿态或限定转角
    // Dog_gait_result->Y0=ges_y+0;
    // Dog_gait_result->Y1=ges_y+zep;
    // Dog_gait_result->Y2=ges_y+0;
    // Dog_gait_result->Y3=ges_y+zep;
    }

    //后半周期
    else if((time_state->t>time_state->Ts*time_state->fai)&&(time_state->t<time_state->Ts))
    { 
     ESP_LOGI(TAG, "后半周期");  
    sigma=2*M_PI*(time_state->t-time_state->Ts*time_state->fai)/(time_state->Ts*time_state->fai);
    zep=trot_state->h*(1-cos(sigma))/2;
    xep_Swing=(trot_state->Xf-trot_state->Xs)*((sigma-sin(sigma)))/(2*M_PI)+trot_state->Xs;
    xep_Stand=(trot_state->Xs-trot_state->Xf)*((sigma-sin(sigma)))/(2*M_PI)+trot_state->Xf;
    
    
    //输出坐标
    
    Dog_gait_result->X0=Dog_gait_set->direction_0*xep_Swing;
    Dog_gait_result->X1=Dog_gait_set->direction_1*xep_Stand;
    Dog_gait_result->X2=Dog_gait_set->direction_2*xep_Swing;
    Dog_gait_result->X3=Dog_gait_set->direction_3*xep_Stand;



    Dog_gait_result->Y0=trot_state->Zs+zep;
    Dog_gait_result->Y1=trot_state->Zs;
    Dog_gait_result->Y2=trot_state->Zs+zep;
    Dog_gait_result->Y3=trot_state->Zs;
    //锁定姿态或限定转角
    // Dog_gait_result->Y0=ges_y+zep;
    // Dog_gait_result->Y1=ges_y;
    // Dog_gait_result->Y2=ges_y+zep;
    // Dog_gait_result->Y3=ges_y;
    }

    
    time_state->t=time_state->t+trot_state->speed;
    if (time_state->t>=1) time_state->t=0;

}