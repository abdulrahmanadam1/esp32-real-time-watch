#ifndef I2C_H
#define I2C_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

#define SDA_PORT 21
#define SCL_PORT 22

#define I2C_FREQ 100 * 1000
#define OLED_I2C_CLK_SPEED 400000
#define I2C_TIMEOUT 100

// init
i2c_master_bus_handle_t i2c_master_init();
i2c_master_dev_handle_t i2c_device_init(i2c_master_bus_handle_t i2c_handle, uint8_t device_addr);


// read/write
void i2c_write(i2c_master_dev_handle_t i2c_dev_handle, uint8_t data_addr, uint8_t data);
uint8_t i2c_read(i2c_master_dev_handle_t dev, uint8_t data_addr);


// i2c scan
bool i2c_precheck(i2c_master_bus_handle_t i2c_handle);


#endif