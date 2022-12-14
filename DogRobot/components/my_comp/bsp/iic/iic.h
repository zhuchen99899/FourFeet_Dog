#ifndef __IIC_H_
#define __IIC_H_

#include "driver/i2c.h"
#include "esp_log.h"
#include <stdio.h>



/**************I2C主机*******************/
#define I2C_MASTER_SCL_IO    19    /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO    18    /*!< gpio number for I2C master data  */

#define I2C_MASTER_NUM I2C_NUM_1   /*!< I2C port number for master dev */ 


/**************I2C硬件配设*******************/
#define I2C_MASTER_FREQ_HZ    100000     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0             /*!< I2C master doesn't need buffer */

#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */







void i2c_master_init(void);
esp_err_t  i2c_wr_reg(uint8_t slaveAddr, uint8_t addr,uint8_t val);
uint8_t i2c_rd_reg(uint8_t slaveAddr, uint8_t addr);

void iic_init(void);

#endif



