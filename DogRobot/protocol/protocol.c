#include "protocol.h"
#include <OS_AppObjCreate.h>
/*@note 在此项目中只有一个任务对环形缓冲区进行读写操作，所以不需要加上信号量，在多个任务访问时需要加上信号同步*/

static const char *TAG = "WIFI_protocol";

// 帧接收缓冲区结构体(环形缓冲区结构体)
typedef struct prot_frame_parser_s
{
uint8_t *recv_ptr;          //记录数据缓冲区的地址
uint16_t r_oft;             //记录当前缓冲区读指针所在位置
uint16_t w_oft;             //记录当前缓冲区写指针所在位置
uint16_t frame_len;         //记录接收到的帧长度，就是一个数据包的长度
uint16_t found_frame_head;  //标志位，可以判断是否找到帧头
}prot_frame_parser_t;

static  prot_frame_parser_t Wifi_Parser; //帧接收环形缓冲区信息

static uint8_t recv_buf[PROT_FRAME_LEN_RECV];  //接收环形缓冲区指针

 
/********************环形缓冲区初始化********************/

/**
* @brief 初始化接收协议 
* @param  
* @return 初始化状态 
* @note 使用环形缓冲区做数据存储,使用前需要对换冲区进行初始化,
        使用了prot_frame_paper_t 结构体来记录缓冲区当前的操作信息
*/
int32_t protocol_init(void)
{

//清空环形缓冲区
memset(&Wifi_Parser, 0,sizeof(prot_frame_parser_t));

/*初始化分配数据接收与解析帧缓冲*/
Wifi_Parser.recv_ptr= recv_buf;  //为环形缓冲区分配空间

return 0;
}

//清除标志
static void recover_Wifi_Parser_protocol_notes(void)
{
 Wifi_Parser.frame_len = 0;
 Wifi_Parser.found_frame_head = 0;
}
/********************环形缓冲区操作********************/

/**
* @brief   接收数据写入缓冲区
* @param   *buf:  数据缓冲区
* @param   ring_buf_len: 缓冲区大小
* @param   w_oft: 写偏移
* @param   *data: 需要写入的数据
* @param   *data_len: 需要写入数据的长度
* @return  void.
*/
static void recvbuf_put_data(uint8_t *buf, uint16_t ring_buf_len, uint16_t w_oft,
        uint8_t *data, uint16_t data_len)
{
    if ((w_oft + data_len) > ring_buf_len)               // 超过缓冲区尾
    {
        uint16_t data_len_part = ring_buf_len - w_oft;     // 缓冲区剩余长度      
        /* 数据分两段写入缓冲区*/
        memcpy(buf + w_oft, data, data_len_part);                         // 写入缓冲区尾
        memcpy(buf, data + data_len_part, data_len - data_len_part);      // 写入缓冲区头
        
    }
    else{
        memcpy(buf + w_oft, data, data_len);    // 数据直接写入缓冲区
        
    }
}


/**
 * @brief   计算环形缓冲区未解析的数据长度
 * @param   *buf:  数据缓冲区.
 * @param   ring_buf_len: 缓冲区大小
 * @param   start: 起始位置
 * @param   end: 结束位置
 * @return  为解析的数据长度
 * @note 该函数的处理流程，是判断读写指针的位置，即读写位置之差，返回串口接收到的数据的长度。
该函数调用了两次，第一次是查询串口有没有接收到数据，接收了多少。第二次是在已知帧头位
置的情况下，从帧头位置开始，重新检查一遍串口接收到的数据长度，判断帧长度是否足够长，
如果足够长就可以从串口接收到的数据中，解析到上位机传来的帧长度的信息。以放弃残缺的数据帧，减少不必要的性能消耗
 */
static int32_t recvbuf_get_len_to_parse(uint16_t frame_len, uint16_t ring_buf_len,uint16_t start, uint16_t end)
{
    uint16_t unparsed_data_len = 0;

    //读指针 小于 写指针，缓冲区有未取出数据，数据长度为写指针-读指针长度
    if (start <= end)
        unparsed_data_len = end - start;
    else
    //读指针 大于等于 写指针， 可能写指针重新回到环形缓冲头部，计算未解析的数据长度为总长+写指针-读指针
        unparsed_data_len = ring_buf_len - start + end;

     
    //帧的长度大于缓冲区未处理数据长度，缓冲区数据长度不正确，未能取出解析数据长度
    if (frame_len > unparsed_data_len)
        return 0;
    else
        return unparsed_data_len;
}


/**
 * @brief   查找环形缓冲区中待解析数据中的固定头字节位置
 * @param   *buf:  环形数据缓冲区.
 * @param   ring_buf_len: 缓冲区总大小
 * @param   start: 读指针
 * @param   len: 需要查找的长度
 * @return  -1：没有找到帧头，其他值：帧头的位置.
 * @note 此函数当存在几帧数据时，未最新的固定头位置
 */
static int32_t recvbuf_find_header(uint8_t *buf, uint16_t ring_buf_len, uint16_t start, uint16_t len)
{
    uint16_t i = 0;

    for (i = 0; i < (len - 3); i++)
    {
        
           //在环形缓冲区未解析数据中 组合四个字节所有可能是否匹配 固定头部，如果找到,返回固定头在环形缓冲区的位置
        if (((buf[(start + i + 0) % ring_buf_len] <<  0) |
             (buf[(start + i + 1) % ring_buf_len] <<  8) |
             (buf[(start + i + 2) % ring_buf_len] << 16) |
             (buf[(start + i + 3) % ring_buf_len] << 24)) == FRAME_HEADER)
        {
            //找到固定头,返回固定头在环形缓冲区的位置
            return ((start + i) % ring_buf_len);
        }

    }
    //未找到固定头 返回-1
    return -1;
}


/**
 * @brief   得到环形缓冲区中待解析数据中的帧长度字节
 * @param   *buf:  数据缓冲区.
 * @param   head_oft: 帧头的偏移位置
 * @return  帧长度. u16
 * @note 此函数必须在找到固定头后使用，此时读指针位置处于head_oft 固定头指针位置
 */
static uint16_t get_frame_len(uint8_t *frame, uint16_t head_oft)
{   
    //帧长度字节 4个字节
    //高字节移位24 ，次高字节移位16，其次字节移位8，最低位不移位 通过 |符号将其连接为 32位数 
    //返回位16位数说明高位被舍弃，实际能表示总长度为65536
    return ((frame[(head_oft + LEN_INDEX_VAL + 0) % PROT_FRAME_LEN_RECV] <<  0) |
            (frame[(head_oft + LEN_INDEX_VAL + 1) % PROT_FRAME_LEN_RECV] <<  8) |
            (frame[(head_oft + LEN_INDEX_VAL + 2) % PROT_FRAME_LEN_RECV] << 16) |
            (frame[(head_oft + LEN_INDEX_VAL + 3) % PROT_FRAME_LEN_RECV] << 24));    // 合成帧长度

}


/**
 * @brief   得到环形缓冲区中待解析数据中的帧类型字节（帧命令）
 * @param   *frame:  数据帧
 * @param   head_oft: 帧头的偏移位置
 * @return  帧类型 u8
 */
static uint8_t get_frame_type(uint8_t *frame, uint16_t head_oft)
{
    //&0xFF 使得返回的值为16进制数，方便宏查找
    return (frame[(head_oft + CMD_INDEX_VAL) % PROT_FRAME_LEN_RECV] & 0xFF);
}



/**
 * @brief   获取 crc16 校验值
 * @param   *frame:  数据缓冲区.
 * @param   head_oft: 帧头的偏移位置
 * @param   frame_len: 帧长
 * @return  校验值 uint16_t型.
 */
static uint16_t get_frame_checkcrc(uint8_t *frame, uint16_t head_oft, uint16_t frame_len)
{

    return ( (frame[(head_oft + frame_len - 1) % PROT_FRAME_LEN_RECV] << 8) | (frame[(head_oft + frame_len - 2) % PROT_FRAME_LEN_RECV]) );

}


/**
 * @brief   计算 CRC_16 校验值
 * @param   crc16_init: CRC校验初始值
 * @param   *_pBuf 数据指针
 * @param   _usLen 长度
 * @return  校验值 uint16_t型.
 */
static uint16_t check_CRC16(uint16_t crc16_init,uint8_t *_pBuf,int _usLen)
{
	uint16_t usIndex;  /* CRC循环中的索引 */
    uint8_t ucCRCHi=crc16_init>>8; /* 高CRC字节初始化 */
    uint8_t ucCRCLo=crc16_init ; /* 低CRC 字节初始化 */

    while (_usLen--)
    {

		usIndex = ucCRCHi ^ *_pBuf++; /* 计算CRC */
		ucCRCHi = ucCRCLo ^ s_CRCHi[usIndex];
		ucCRCLo = s_CRCLo[usIndex];
    }


    return (ucCRCLo <<8| ucCRCHi );

}


/**
 * @brief   分析待解析数据区数据是否符合整个通讯协议规定并查询帧类型（命令）
 * @param   *data:  帧数据
 * @param   data_len: 帧数据的大小
 * @return  帧类型（命令）.
 */
static uint8_t protocol_frame_parse(uint8_t *data, uint16_t *data_len)
{
    uint8_t frame_type = CMD_NONE; //初始化帧类型字节
    uint16_t need_to_parse_len = 0; //需要解析的长度
    int16_t header_oft = -1; //头部初始化
    uint16_t checkcrc16_num = 0xFFFF; //校验和
    

    // 得到未解析的数据长度
    need_to_parse_len = recvbuf_get_len_to_parse(Wifi_Parser.frame_len, PROT_FRAME_LEN_RECV, Wifi_Parser.r_oft, Wifi_Parser.w_oft);    
    
    if (need_to_parse_len < 4)     //(4byte头部) 此时肯定还不能同时找到帧头和帧长度
    { 
        ESP_LOGI(TAG, "need to parse len less than 4");
        return frame_type;
    }
    /* 还未找到帧头，需要进行查找*/
    if (0 == Wifi_Parser.found_frame_head)
    {

        header_oft = recvbuf_find_header(Wifi_Parser.recv_ptr, PROT_FRAME_LEN_RECV, Wifi_Parser.r_oft, need_to_parse_len);
        if (0 <= header_oft)
        {
            /* 已找到帧头*/
            Wifi_Parser.found_frame_head = 1; //找到固定头标记

            Wifi_Parser.r_oft = header_oft; //读指针移位到固定头位置
            
            /* 确认是否可以计算帧长*/
            //在确定固定头后，判断固定头后的数据(包含固定头)是否低于6个字节(4byte头部+2byte长度字节)，
            //如果低于9个字节，即数据帧不完整，放弃此帧 frame_type=CMD_NONE
                         need_to_parse_len = recvbuf_get_len_to_parse(Wifi_Parser.frame_len, PROT_FRAME_LEN_RECV,
                                                          Wifi_Parser.r_oft, Wifi_Parser.w_oft);
            if (need_to_parse_len< 6)
            {
                ESP_LOGI(TAG, "need to parse len less than 6 (in find head)");
                  if(need_to_parse_len>=4)Wifi_Parser.r_oft = (Wifi_Parser.r_oft + 1) % PROT_FRAME_LEN_RECV; //有可能出现完整头部，影响下一次解帧
                  recover_Wifi_Parser_protocol_notes();
                return frame_type;
            }
        }
        else 
        {
            /* 未解析的数据中依然未找到帧头，丢掉此次解析过的所有数据*/
            Wifi_Parser.r_oft = ((Wifi_Parser.r_oft + need_to_parse_len - 3) % PROT_FRAME_LEN_RECV);//移位读指针
            ESP_LOGI(TAG, "can not find head");
            return frame_type;
        }
    }
    
    /* 开始计算帧长，并确定是否可以进行数据解析*/
    if (0 == Wifi_Parser.frame_len) 
    {
        //取出帧长度字节
        Wifi_Parser.frame_len = get_frame_len(Wifi_Parser.recv_ptr, Wifi_Parser.r_oft);
        
        //未解析长度小于帧长度字节数据，说明数据丢包，舍弃未解析数据
        if(need_to_parse_len < Wifi_Parser.frame_len)
        {
            ESP_LOGI(TAG, "len err");
            if(need_to_parse_len>=4)Wifi_Parser.r_oft = (Wifi_Parser.r_oft + 1) % PROT_FRAME_LEN_RECV; //有可能出现完整头部，影响下一次解帧
            recover_Wifi_Parser_protocol_notes();
            return frame_type;
        }
    }

    /* 帧头位置确认，且未解析的数据超过帧长，可以计算校验和*/
    if ((Wifi_Parser.frame_len + Wifi_Parser.r_oft - PROT_FRAME_LEN_CHECKSUM) > PROT_FRAME_LEN_RECV)
    {
        //循环换成区剩余的长度不足存储当前帧长度，说明帧写指针回到缓冲区头部

        /* 数据帧被分为两部分，一部分在缓冲区尾，一部分在缓冲区头 */
        checkcrc16_num = check_CRC16(checkcrc16_num, Wifi_Parser.recv_ptr + Wifi_Parser.r_oft, 
                PROT_FRAME_LEN_RECV - Wifi_Parser.r_oft);
        checkcrc16_num = check_CRC16(checkcrc16_num, Wifi_Parser.recv_ptr, Wifi_Parser.frame_len -
                PROT_FRAME_LEN_CHECKSUM + Wifi_Parser.r_oft - PROT_FRAME_LEN_RECV);

    }
    else 
    {
        /* 数据帧可以一次性取完*/
        checkcrc16_num = check_CRC16(checkcrc16_num, Wifi_Parser.recv_ptr + Wifi_Parser.r_oft, Wifi_Parser.frame_len - PROT_FRAME_LEN_CHECKSUM);
    }
       
    if (checkcrc16_num == get_frame_checkcrc(Wifi_Parser.recv_ptr, Wifi_Parser.r_oft, Wifi_Parser.frame_len))
    {
        /* 校验成功，拷贝整帧数据 */
        if ((Wifi_Parser.r_oft + Wifi_Parser.frame_len) > PROT_FRAME_LEN_RECV) 
        {
           
            /* 数据帧被分为两部分，一部分在缓冲区尾，一部分在缓冲区头*/
            uint16_t data_len_part = PROT_FRAME_LEN_RECV - Wifi_Parser.r_oft;
            memcpy(data, Wifi_Parser.recv_ptr + Wifi_Parser.r_oft, data_len_part);
            memcpy(data + data_len_part, Wifi_Parser.recv_ptr, Wifi_Parser.frame_len - data_len_part);
        }
        else 
        {
            /*数据帧可以一次性取完*/
            
            memcpy(data, Wifi_Parser.recv_ptr + Wifi_Parser.r_oft, Wifi_Parser.frame_len);
        }
        *data_len = Wifi_Parser.frame_len;
        frame_type = get_frame_type(Wifi_Parser.recv_ptr, Wifi_Parser.r_oft);
        /*丢弃缓冲区中的命令帧*/
        Wifi_Parser.r_oft = (Wifi_Parser.r_oft + Wifi_Parser.frame_len) % PROT_FRAME_LEN_RECV;
    }
    else
    {
        /*校验错误，说明之前找到的帧头只是偶然出现的废数据*/
        Wifi_Parser.r_oft = (Wifi_Parser.r_oft + 1) % PROT_FRAME_LEN_RECV;
        ESP_LOGI(TAG, "crc  err");
    }

    //解数据帧完成，缓冲区结构体信息标志 除读写指针，归0
    Wifi_Parser.frame_len = 0;
    Wifi_Parser.found_frame_head = 0;
    ESP_LOGI(TAG, "sucess exit protocol");
    return frame_type;

}



/*********************通讯协议调用*************************/

/**
* @brief 接收数据并写入环形缓冲区,并记录环形缓冲区写指针
* @param  *data :要计算的数据的数组
* @param  data_len: 数据的大小
* @return  
* @note 
*/
void protocol_data_recv(uint8_t *data,uint16_t data_len)
{
    //接收数据并写入环形缓冲区
    recvbuf_put_data(Wifi_Parser.recv_ptr,PROT_FRAME_LEN_RECV,Wifi_Parser.w_oft,data,data_len);

    //记录写指针偏移
    Wifi_Parser.w_oft=(data_len+Wifi_Parser.w_oft) % PROT_FRAME_LEN_RECV;
    ESP_LOGI(TAG, "ring w_oft=%d",Wifi_Parser.w_oft);
}


/**
* @brief 接收数据并处理通讯协议
* @param  void
* @return  -1 无命令退出 , 0存在命令
* @note 
*/
int8_t receiving_process(void)
{
extern QueueHandle_t Robot_FSM_STATE_Queue;  //消息队列句柄
extern QueueHandle_t Gait_mode_Queue;
extern QueueHandle_t Gait_trot_set_Queue;

uint8_t frame_data[128]; //要能放下最长的帧
uint16_t frame_len = 0 ; //帧长度

uint8_t cmd_type=CMD_NONE; //命令类型
extern FSM_t stFsm;	//状态机结构体
static int FSM_event = STATE_Stop; //发送给状态机的事件

trot_set_t *trot_set_send_P;
extern trot_set_t trot_set_data;



int gait_mode;

trot_set_send_P=&trot_set_data;

while(1)
{
cmd_type = protocol_frame_parse(frame_data,&frame_len);
    switch (cmd_type)
    {

    case CMD_NONE: //空指令
        {
            ESP_LOGI(TAG, "protocol  none cmd");
            return -1;
        }
        break;

    case CMD_Servor_align: //校准模式
        {
            ESP_LOGI(TAG, "protocol   cmd   CMD_Servor_align");
            ESP_LOGI(TAG, "curState=%d",stFsm.stCurState);

            FSM_event=EVENT_Align;
            xQueueOverwrite(Robot_FSM_STATE_Queue,&FSM_event);
          
            return -1;
        }
        break;
    case CMD_Exit_Align://停止校准模式
        {
            ESP_LOGI(TAG, "protocol   cmd   CMD_Dog_stop");
            ESP_LOGI(TAG, "curState=%d",stFsm.stCurState);

            FSM_event=EVENT_Stop_from_Align;
            xQueueOverwrite(Robot_FSM_STATE_Queue,&FSM_event);
  
            return -1;
        }
        break;
    case CMD_Dog_Servor_ctrl://舵机校准
    {
    ESP_LOGI(TAG, "protocol   cmd   CMD_Dog_Servor_ctrl");

            tcp_rec_Dog_servor_ctrl(frame_data);

     return -1;
    }
    break;

    case CMD_Dog_XY_ctrl://足端校准
    {      
        ESP_LOGI(TAG, "protocol   cmd   CMD_Dog_XY_ctrl");

            tcp_rec_Dog_XY_ctrl(frame_data);

        
        return -1;
    }
    break;

    case CMD_Gait_ctrl://步态模式
    {
        ESP_LOGI(TAG, "Gati  cmd   CMD_Gait_ctrl");

        gait_mode=tcp_rec_gait_mode_ctrl(frame_data);

        // if(gait_mode<=leave_gait)
        // else
        FSM_event=EVENT_Gait;
        //先写入gait模式再触发状态切换
        xQueueOverwrite(Gait_mode_Queue,&gait_mode);
        xQueueOverwrite(Robot_FSM_STATE_Queue,&FSM_event);
        return -1;
    }
    break;

    case CMD_Gait_trot_set_ctrl://trot 步态参数设置
    {
        ESP_LOGI(TAG, "Gati  cmd   CMD_Gait_trot_set");
        trot_set_t trot_set_protocol=tcp_rec_trot_set_ctrl(frame_data);
        trot_set_send_P->h=trot_set_protocol.h;
        trot_set_send_P->speed=trot_set_protocol.speed;
        trot_set_send_P->Xf=trot_set_protocol.Xf;
        trot_set_send_P->Xs=trot_set_protocol.Xs;
        trot_set_send_P->Zs=trot_set_protocol.Zs;
        xQueueOverwrite(Gait_trot_set_Queue,(void *)&trot_set_send_P);
        return -1;
    }
    break;

    case CMD_Gait_dir_set:
    {
        ESP_LOGI(TAG, "Gati  cmd   CMD_Gait_dir_set");
        tcp_rec_gait_dir_set_ctrl(frame_data);

        return -1;
    }
    break;

    default:
        return -1;    
    break;
    }
}

}
