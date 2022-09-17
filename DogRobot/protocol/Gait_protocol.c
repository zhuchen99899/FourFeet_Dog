#include "Gait_protocol.h"

static const char *TAG = "Dog_Gait_protocol";
/**
  * @brief 接收TCP gait控制报文
  * @param *frame_data
  * @return int
  * @note 
  */
 int tcp_rec_gait_mode_ctrl(uint8_t *frame_data)
 {

   int gait_mode;

   uint32_t temp1 = COMPOUND_32BIT(&frame_data[12]);

   gait_mode=(int) *(float *)&temp1;
  // ESP_LOGI(TAG, "framedata=%d",gait_mode);
   switch (gait_mode)
   {
   case leave_gait:
     /* code */
    break;
   case trot:
    ESP_LOGI(TAG, "trot giat mode");
   break;
   default:
     break;
   }

   return trot;
 }

 /**
  * @brief 接收TCP trot参数设置控制报文
  * @param *frame_data
  * @return trot_set_t
  * @note 
  */
  trot_set_t tcp_rec_trot_set_ctrl(uint8_t *frame_data)
  {
  trot_set_t trot_set_protocol;

  uint32_t temp_h = COMPOUND_32BIT(&frame_data[12]);
  uint32_t temp_speed = COMPOUND_32BIT(&frame_data[16]);
  uint32_t temp_Xf = COMPOUND_32BIT(&frame_data[20]);
  uint32_t temp_Xs = COMPOUND_32BIT(&frame_data[24]);
  uint32_t temp_Zs = COMPOUND_32BIT(&frame_data[28]);

  trot_set_protocol.h=*(float *)&temp_h;
  trot_set_protocol.speed=*(float *)&temp_speed;
  trot_set_protocol.Xf=*(float *)&temp_Xf;
  trot_set_protocol.Xs=*(float *)&temp_Xs;
  trot_set_protocol.Zs=*(float *)&temp_Zs;

  return trot_set_protocol;

  }



        



  /**
  * @brief 接收TCP gait 方向设置控制报文
  * @param *frame_data
  * @return void
  * @note 
  */
  void tcp_rec_gait_dir_set_ctrl(uint8_t *frame_data)
  {
  Dog_gait_set_dir_t *gait_dir_set_protocol;
  extern Dog_gait_set_dir_t Dog_gait_set_dir_data;
  extern QueueHandle_t Dog_gait_set_dir_Queue;

  gait_dir_set_protocol=&Dog_gait_set_dir_data;
  uint32_t temp_dir_num = COMPOUND_32BIT(&frame_data[12]);
  int dir=(int) *(float *)&temp_dir_num;


  switch (dir)
  {
  case stop:
    gait_dir_set_protocol->direction_0=0;
    gait_dir_set_protocol->direction_1=0;
    gait_dir_set_protocol->direction_2=0;
    gait_dir_set_protocol->direction_3=0;
    xQueueOverwrite(Dog_gait_set_dir_Queue,(void *)&gait_dir_set_protocol);

    break;
  case forward:
    gait_dir_set_protocol->direction_0=-1;
    gait_dir_set_protocol->direction_1=-1;
    gait_dir_set_protocol->direction_2=-1;
    gait_dir_set_protocol->direction_3=-1;
    xQueueOverwrite(Dog_gait_set_dir_Queue,(void *)&gait_dir_set_protocol);

    break;
  case backward:
    gait_dir_set_protocol->direction_0=1;
    gait_dir_set_protocol->direction_1=1;
    gait_dir_set_protocol->direction_2=1;
    gait_dir_set_protocol->direction_3=1;
    xQueueOverwrite(Dog_gait_set_dir_Queue,(void *)&gait_dir_set_protocol);

    break;
  case left:
    gait_dir_set_protocol->direction_0=-1;
    gait_dir_set_protocol->direction_1=-1;
    gait_dir_set_protocol->direction_2=1;
    gait_dir_set_protocol->direction_3=1;
    xQueueOverwrite(Dog_gait_set_dir_Queue,(void *)&gait_dir_set_protocol);

    break;
  case right:
    gait_dir_set_protocol->direction_0=1;
    gait_dir_set_protocol->direction_1=1;
    gait_dir_set_protocol->direction_2=-1;
    gait_dir_set_protocol->direction_3=-1;
    xQueueOverwrite(Dog_gait_set_dir_Queue,(void *)&gait_dir_set_protocol);

    break;

  default:
    
    break;
  }



  }