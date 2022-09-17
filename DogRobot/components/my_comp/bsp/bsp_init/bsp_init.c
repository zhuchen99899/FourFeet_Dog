#include "bsp_init.h"
#include "wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"


static const char* TAG = "bsp_init";


//板级驱动初始化
void bsp_init(void)
{
 //初始化NVS  NVS会记录wifi连接
esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");

//wifi_STA
wifi_connect_STA();

iic_init();
PCA9685_reset();
PCA9685_freq(50);

trot_init();

}



