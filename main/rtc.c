#include "rtc.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

static const char *TAG = "RTC";

void set_time(i2c_master_dev_handle_t rtc, rtc_time_t time)
{
    // get time data from rtc_time_t obj
    int hour = time.hours;
    int minute = time.minutes;
    int seconds = time.seconds;
    bool am_pm = time.am_pm; 


    // convert second to datasheet formatting
    uint8_t sec_bit = (seconds % 10) | (seconds / 10) << 4; 
    uint8_t min_bit = (minute % 10) | (minute / 10) << 4; 
    uint8_t hrs_bit = (hour % 10) | (hour / 10) << 4;

    // set to 12 hr mode for AM/PM
    hrs_bit |= TWELVE_HOUR_MODE;

    // print out value of registers in RTC module
    ESP_LOGD(TAG, "hours 0x%02x, minutes 0x%02x, seconds 0x%02x", hrs_bit, min_bit, sec_bit);

    i2c_write_new(rtc, SC_REG, sec_bit);
    i2c_write_new(rtc, MN_REG, min_bit);
    i2c_write_new(rtc, HR_REG, hrs_bit);
    ESP_LOGI(TAG, "SUCCESSFULLY SET RTC TIME");
}

rtc_time_t get_time(i2c_master_dev_handle_t rtc)
{
    uint8_t sec_bits = i2c_read(rtc, 0x00);
    uint8_t min_bits = i2c_read(rtc, 0x01);
    uint8_t hr_bits = i2c_read(rtc, 0x02);
    int seconds = (sec_bits & 0x0F) + (sec_bits >> 4) * 10;
    int minutes = (min_bits & 0x0F) + (min_bits >> 4) * 10;
    int hours = (hr_bits & 0x0F) + ((hr_bits >> 4) & 1)* 10;


    bool am_pm = ((hr_bits >> 5) & 1) == 1 ? true : false;
    rtc_time_t time = {
        .hours = hours,
        .minutes = minutes,
        .seconds = seconds,
        .am_pm = am_pm
    };

    // uint8_t am_pm = (hr_bits >> 5) & 1; // bit 5 of addr2
    ESP_LOGD(TAG, "ACTUAL: hours 0x%02x, minutes 0x%02x, seconds 0x%02x", hr_bits, min_bits, sec_bits);
    return time;
}
