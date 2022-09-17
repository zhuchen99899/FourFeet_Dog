#include "wifi.h"



/*FreeRTOS事件组,当wifi连接时,发送信号*/
//定义事件标志组句柄
extern EventGroupHandle_t s_wifi_event_group;


//打印标签
static const char *TAG = "wifi station init";
static const char *TAG_socket = "socket assert";
//重连接记数
static int s_retry_num = 0;


/*事件句柄函数，用于注册wifi状态机事件*/
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{

    ip_event_got_ip_t* event;

        if(event_base==WIFI_EVENT){
        //WIFI事件组
            switch(event_id){
                case WIFI_EVENT_STA_START:
                 //wifi状态机状态为STA连接启动
                  esp_wifi_connect();
                  break;

                case WIFI_EVENT_STA_DISCONNECTED:
                //如果状态机状态为STA连接断开
                    if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
                        //小于最大重连接次数
                    esp_wifi_connect();
                    s_retry_num++;
                    ESP_LOGI(TAG, "retry to connect to the AP");
                    }
                    else{
                    //WIFI 连接失败
                     //设置freertos事件标志组WIFI_FAIL_BIT置1
                    xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
                    ESP_LOGI(TAG, "failed connect wifi");
                    }
                  break;
                case SYSTEM_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "STA connect success");
                break;
                default:
                break;
            }
        }

        else if(event_base==IP_EVENT)
        {

            switch(event_id){
                case IP_EVENT_STA_GOT_IP:
                //STA模式下获取IP成功
                event = (ip_event_got_ip_t*) event_data;
                ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
                xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
                break;
                default:
                break;
            }
           

        }
}

void wifi_connect_STA(void)
{

  
    ESP_ERROR_CHECK(esp_netif_init());//LWIP协议栈相关初始化

    ESP_ERROR_CHECK(esp_event_loop_create_default());//状态机事件循环创建
    esp_netif_create_default_wifi_sta(); //默认配置创建wifista

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); //默认配置创建wifi
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));//wifi初始化

    //esp事件句柄标志创建(状体机)
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    //esp事件句柄注册
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));


    //连接网路ssid与密码以及加密方式配置
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = ESP_WIFI_SSID,
            .password = ESP_WIFI_PASSWORD,
            /*设置密码意味着将连接所有安全模式，如果你的AP设备不支持WPA2加密，
            *开启下面行中添加注释来启用如WEP/WPA的AP */
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );//设置连接模式为station
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );//STA 连接设置
    ESP_ERROR_CHECK(esp_wifi_start() );//开启连接


    ESP_LOGI(TAG, "wifi_init_sta finished.");


    /*等待连接建立的freertos事件标志组*/
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,//返回时事件标志位不会被清
            pdFALSE, //其中一个标志被置位或超出等待时长时返回
            portMAX_DELAY);//阻塞等待


    /*判断相应事件是否发生*/
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 ESP_WIFI_SSID, ESP_WIFI_PASSWORD);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 ESP_WIFI_SSID, ESP_WIFI_PASSWORD);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    // /* esp事件注销 */
    // ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
    // ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    // /*删除事件标志组*/
    // vEventGroupDelete(s_wifi_event_group);
}


/*************TCP连接错误诊断***************/
//TCP错误码
int get_socket_error_code(int socket)
{
    int result;
    u32_t optlen = sizeof(int);
    int err = getsockopt(socket, SOL_SOCKET, SO_ERROR, &result, &optlen);
    if (err == -1)
    {
        ESP_LOGE(TAG_socket, "getsockopt failed:%s", strerror(err));
        return -1;
    }
    return result;
}

int show_socket_error_reason(const char *str, int socket)
{
    int err = get_socket_error_code(socket);

    if (err != 0)
    {
        ESP_LOGW(TAG_socket, "%s socket error %d %s", str, err, strerror(err));
    }

    return err;
}







