#include "TCP_rec.h"

static const char *TAG = "TCP_rec_task"; //ESP 打印标签

//TCP重连接标志
extern bool g_rxtx_need_restart;
extern int server_socket;
extern int connect_socket;


//该函数是以十六进制打印buf数组中的值
void dump_buf(uint8_t *buf, int bytes)
{
    int    i;  
    for(i=0; i<bytes; i++)
    {   
		ESP_LOGI(TAG, "recvData: 0x%02x",  (uint8_t)buf[i]);//打印接收到的数组
        if( 0 == (i+1)%16 )
            printf("\n");
    }
    printf("\n");
}



void close_socket()
{
    close(connect_socket);
    close(server_socket);
}


//任务2
void Task_TCP_rec(void * pvParameters)
{

extern int connect_socket; //TCP 连接


int len = 0;
uint8_t databuff[128];

	while (1){
		memset(databuff, 0x00, sizeof(databuff));//清空缓存
		len = recv(connect_socket, databuff, sizeof(databuff), 0);//读取接收数据
		g_rxtx_need_restart = false;
		if (len > 0){
			//ESP_LOGI(TAG, "recvData: %s", databuff);//打印接收到的数组
			// dump_buf(databuff,sizeof(databuff));
			//send(connect_socket, databuff, strlen(databuff), 0);//接收数据回发

			// intdataVal=atoi((char *)databuff);
			// ESP_LOGI(TAG,"intData:%d",intdataVal);
			// xQueueOverwrite(Servo_angle_Queue,&intdataVal);


		   protocol_data_recv(databuff,sizeof(databuff));
		   receiving_process();
			//sendto(connect_socket, databuff , sizeof(databuff), 0, (struct sockaddr *) &remote_addr,sizeof(remote_addr));
		}else{
			show_socket_error_reason("recv_data", connect_socket);//打印错误信息
			g_rxtx_need_restart = true;//服务器故障，标记重连
           
			vTaskDelete(NULL);
		}
	}
	close_socket();
	g_rxtx_need_restart = true;//标记重连


	vTaskDelete(NULL);

    
}

