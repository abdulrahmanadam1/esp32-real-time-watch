#include "i2c.h"
static const char *TAG = "I2C";
#define RTC_ADDR 0x68
#define EEPROM_ADDR 0x57
#define OLED_ADDR 0x3C
#define NUM_DEVICES 3
uint8_t i2c_addrs = {0x3C, 0x68, 0x3C};



i2c_master_bus_handle_t i2c_master_init() 
{
    i2c_master_bus_handle_t i2c_handle = NULL;
    i2c_master_bus_config_t i2c_bus = {
        .clk_source = I2C_CLK_SRC_APB,
        .scl_io_num = SCL_PORT,
        .sda_io_num = SDA_PORT,
        .i2c_port = I2C_NUM_0,
        .flags = {
            .enable_internal_pullup = true
        },
        .glitch_ignore_cnt = 7
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus, &i2c_handle));
    ESP_LOGI(TAG, "Successfully initialized i2c master bus config");
    return i2c_handle;
    
}

i2c_master_dev_handle_t i2c_device_init(i2c_master_bus_handle_t i2c_handle, uint8_t device_addr)
{
    i2c_master_dev_handle_t i2c_dev_handle = NULL;
    i2c_device_config_t i2c_dev = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = device_addr,
        .scl_speed_hz = I2C_FREQ,
        .scl_wait_us = 100
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_handle, &i2c_dev, &i2c_dev_handle));
    ESP_LOGI(TAG, "Successfully initialized i2c device config");

    return i2c_dev_handle;
}

void i2c_write_new(i2c_master_dev_handle_t i2c_dev_handle, uint8_t data_addr, uint8_t data)
{
    printf("\n transmitting 0x%02X\n", data);
    uint8_t write_buf[2] = {data_addr, data};
    ESP_ERROR_CHECK(i2c_master_transmit(i2c_dev_handle, write_buf, sizeof(write_buf), 100));
}


uint8_t i2c_read(i2c_master_dev_handle_t dev, uint8_t data_addr)
{
    size_t len = 1;
    uint8_t *data = malloc(len);

    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev, (uint8_t*)&data_addr, len, data, len, -1));
    // for(int i = 0; i < len; i++)
    //     ESP_LOGI(TAG, "0x%02x ", data[i]);
    uint8_t val = *data;
    free(data);
    // ESP_LOGI(TAG, "data: 0x%02x ", val);
    return val;
}

bool i2c_is_good(i2c_master_bus_handle_t i2c_handle)
{
    uint8_t i2c_addrs[NUM_DEVICES] = {0x3C, 0x68, 0x57};

    bool is_good = true;
    for (uint8_t i = 0; i < NUM_DEVICES; i++) {
        uint8_t addr = i2c_addrs[i];

        esp_err_t ret = i2c_master_probe(i2c_handle, addr, 50);
        if (ret == ESP_OK)
        {
            ESP_LOGI(TAG, "I2C ADDR 0x%02x is good! %s is up", addr,
            i == 0 ? "OLED":
            i == 1 ? "RTC" :
            i == 2 ? "EEPROM": "DEVICE: 0x%02x"
            );

        } else {
            ESP_LOGE(TAG, "I2C ADDR 0x%02x timed out! ensure that %s is conected!", addr,
            i == 0 ? "OLED":
            i == 1 ? "RTC" :
            i == 2 ? "EEPROM": "DEVICE: 0x%02x"
            );
            is_good = false;
        }
    }
    return is_good;
}




uint16_t scan_addrs(i2c_master_bus_handle_t i2c_handle)
{
    uint16_t first_addr; // first addr that return ACK
    for (uint16_t addr = 0; addr < 128; addr++) {
        switch (i2c_master_probe(i2c_handle, addr, -1)) {
            case ESP_OK:
                ESP_LOGI(TAG, "%02x: YESS!!!! IS THIS ONE!!", addr);
                first_addr = addr;
                break;
            case ESP_ERR_TIMEOUT:
                ESP_LOGI(TAG, "%02x: TIMEOUT", addr);
            default:
                ESP_LOGI(TAG, "%02x : ---", addr);
        }
        break;

    }
    return first_addr;
}



