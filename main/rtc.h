#ifndef RTC_H
#define RTC_H

#include "i2c.h"

#define RTC_ADDR 0x68
#define EEPROM_ADDR 0x57

#define TWELVE_HOUR_MODE (1<<6)
#define HR_REG 0x02
#define MN_REG 0x01
#define SC_REG 0x00

typedef struct {
    int hours;
    int minutes;
    int seconds;
    bool am_pm;
} rtc_time_t;


// i2c_master_dev_handle_t rtc_init(i2c_master_bus_handle_t i2c_handle);

// void get_time(i2c_master_dev_handle_t rtc_handle);
void set_time(i2c_master_dev_handle_t rtc, rtc_time_t time);
rtc_time_t get_time(i2c_master_dev_handle_t rtc);




#endif