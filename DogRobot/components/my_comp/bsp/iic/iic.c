#include "iic.h"

// static const char *TAG = "iic";

/***************I2C初始化******************/
void i2c_master_init(void)
{
int i2c_master_port = I2C_MASTER_NUM;
i2c_config_t conf;
conf.mode =I2C_MODE_MASTER;  //IIC主机模式
conf.sda_io_num = I2C_MASTER_SDA_IO;      //IO
conf.sda_pullup_en = GPIO_PULLUP_ENABLE; //SDA上拉使能
conf.scl_io_num = I2C_MASTER_SCL_IO;     //IO
conf.scl_pullup_en = GPIO_PULLUP_ENABLE; //SCL上拉使能
conf.master.clk_speed =I2C_MASTER_FREQ_HZ; //SCL线频率 100K
i2c_param_config(i2c_master_port,&conf); //设置IIC配置
//对接口配置完后，需要调用i2c_driver_install对I2C总线的驱动进行安装，主要是对数据发送缓存进行内存分配。
i2c_driver_install(i2c_master_port,conf.mode,I2C_MASTER_RX_BUF_DISABLE,I2C_MASTER_TX_BUF_DISABLE,0);
}



/**
 @brief I2C写数据函数
 @param slaveAddr -[in] 从设备地址
 @param addr -[in] 寄存器地址
 @param val -[in] 写入数据
 @return 错误码
*/
esp_err_t  i2c_wr_reg(uint8_t slaveAddr, uint8_t addr,uint8_t val)
{
    esp_err_t ret = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
	i2c_master_write_byte(cmd, slaveAddr << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, addr, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, val, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
	return ret;
}

/**
 @brief I2C读数据函数
 @param slaveAddr -[in] 从设备地址
 @param addr -[in] 寄存器地址
 @return data 返回单字节数据
 @note 不需要回应ACK信号
*/
uint8_t i2c_rd_reg(uint8_t slaveAddr, uint8_t addr)
{
    //esp_err_t ret = 0;
	uint8_t data = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
	i2c_master_write_byte(cmd, slaveAddr << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, addr, ACK_CHECK_EN);
    i2c_master_start(cmd);
	i2c_master_write_byte(cmd, slaveAddr << 1 | I2C_MASTER_READ, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &data, NACK_VAL);
    i2c_master_stop(cmd);
    //ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_RATE_MS);
    i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
	return data;
}




void iic_init(void)
{
i2c_master_init();
}