#include "TCP_connect.h"

static const char *TAG = "wifi server connect"; //ESP 打印标签
/*socket*/
int server_socket = 0;
int connect_socket = 0;
//socket 配设结构体
static struct sockaddr_in server_addr;
static struct sockaddr_in client_addr;
//socket 长度
static unsigned int socklen = sizeof(client_addr);


//TCP重连接标志
bool g_rxtx_need_restart = false;

/*TCP接收任务*/
#define TCP_rec_STK_NAME "TCP_rec"
#define TCP_rec_STK_SIZE      4096
#define TCP_rec_STK_PRIO      4
TaskHandle_t xHandle__tcp_rec_task= NULL;
extern void Task_TCP_rec(void * pvParameters);

//定义事件标志组句柄
extern EventGroupHandle_t s_wifi_event_group;

/***********建立TCP服务器**************/
//形参的含义:true 为创建服务器并且等待连接,false 仅仅是等待连接;返回值是是否成功接收到
esp_err_t create_tcp_server(bool isCreatServer)
{
	//首次建立server  创建服务器并且等待连接
	if (isCreatServer){
		ESP_LOGI(TAG, "server socket....,port=%d", TCP_PORT);
		server_socket = socket(AF_INET, SOCK_STREAM, 0);//新建socket
		if (server_socket < 0){
			show_socket_error_reason("create_server", server_socket);
			close(server_socket);//新建失败后，关闭新建的socket，等待下次新建
			return ESP_FAIL;
		}
		//配置新建server socket参数
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(TCP_PORT);
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		//bind:地址的绑定
		if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
			show_socket_error_reason("bind_server", server_socket);
			close(server_socket);//bind失败后，关闭新建的socket，等待下次新建
			return ESP_FAIL;
		}
	}
	//listen，下次时，直接监听 监听指定的端口
	if (listen(server_socket, 5) < 0){
		show_socket_error_reason("listen_server", server_socket);
		close(server_socket);//listen失败后，关闭新建的socket，等待下次新建
		return ESP_FAIL;
	}
	//accept，搜寻全连接队列
	connect_socket = accept(server_socket, (struct sockaddr *)&client_addr, &socklen);
    //判断是否连接成功
	if (connect_socket < 0){
		show_socket_error_reason("accept_server", connect_socket);
		close(server_socket);//accept失败后，关闭新建的socket，等待下次新建
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "tcp connection established!");
	return ESP_OK;

}








/**************TCP 连接任务***********************/
void Task_TCP_connect(void * pvParameters)
{



	//死循环
    while(1)
    {
    ESP_LOGI(TAG, "start create tcp server");
    g_rxtx_need_restart=false;//第一次连接时设定重连信号为禁止重连
    ESP_LOGI(TAG, "Eventgroupwait");
    /*等待连接建立的freertos事件标志组*/
    xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT ,
            pdFALSE,//返回时事件标志位不会被清
            pdTRUE, //其中一个标志被置位或超出等待时长时不反回
            portMAX_DELAY);//阻塞等待
    
        //xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);//等待WIFI连接信号标志组,当wifi连接成功后进入
	
    ESP_LOGI(TAG, "Eventgroupwait success");
    vTaskDelay(3000 / portTICK_RATE_MS);// 延时3S准备建立server
    ESP_LOGI(TAG, "create tcp server");
    int socket_ret = create_tcp_server(true);// 建立server

        if (socket_ret == ESP_FAIL){// 建立失败
			ESP_LOGI(TAG, "create tcp socket error,stop and restart connect...");
			continue;//余下循环体不执行，重新建立第一次连接
		}else{// 建立成功
			ESP_LOGI(TAG, "create tcp socket succeed...");            
			// 建立tcp接收数据任务
			if (pdPASS != xTaskCreate(&Task_TCP_rec, TCP_rec_STK_NAME, TCP_rec_STK_SIZE, NULL, TCP_rec_STK_PRIO, &xHandle__tcp_rec_task)){
				ESP_LOGI(TAG, "Recv task create fail!");
			}else{
				ESP_LOGI(TAG, "Recv task create succeed!");
			}
		}

        //server重连(在接收任务中服务器故障重连)
        while (1){

                vTaskDelay(3000 / portTICK_RATE_MS); 
                if (g_rxtx_need_restart){// 重新建立server，流程和上面一样
                    ESP_LOGI(TAG, "tcp server error,some client leave,restart...");
                    // 建立server
                    if (ESP_FAIL != create_tcp_server(false)){
                        if (pdPASS != xTaskCreate(&Task_TCP_rec, TCP_rec_STK_NAME, TCP_rec_STK_SIZE, NULL, TCP_rec_STK_PRIO, &xHandle__tcp_rec_task)){
                            ESP_LOGE(TAG, "tcp server Recv task create fail!");
                        }else{
                            ESP_LOGI(TAG, "tcp server Recv task create succeed!");
                            g_rxtx_need_restart = false;//重新建立完成，清除标记
                        }
                    }
                }
            }
        



        
    }
    vTaskDelete(NULL);
    
}

