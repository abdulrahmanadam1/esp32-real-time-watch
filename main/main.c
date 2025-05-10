#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "i2c.h"
#include "ssd1306.h"
#include "rtc.h"
#include "ntp.h"

static const char *TAG = "MAIN";

rtc_time_t curr_time;

void app_main(void)
{
    

    i2c_master_bus_handle_t i2c_handle = i2c_master_init();

    if (!i2c_precheck(i2c_handle))
    {
        ESP_LOGE(TAG, "FAILED TO PASS check_i2c");
        return;
    }
    ESP_LOGI(TAG, "I2C scan returns TRUE: all I2C devices are connected.");


        
    // -------- INITS -------- 
    lv_disp_t *disp = oled_init(i2c_handle);

    i2c_master_dev_handle_t rtc_handle =  i2c_device_init(i2c_handle, RTC_ADDR); //rtc_init(i2c_handle);
    ESP_LOGI(TAG, "Initialized RTC device");

    rtc_time_t set_time_value;


    // lvgl screen object
    lv_obj_t *scr = lv_disp_get_scr_act(disp);


    // SYNC time from NTP server to RTC module
    ntp_init();
    vTaskDelay(100 / portTICK_PERIOD_MS);

    ESP_LOGI(TAG, "SYNCING NTP AND RTC MODULE");
    set_time_value = get_current_datetime();
    set_time(rtc_handle, set_time_value);



    while(1)
    {
        // pull time from RTC
        curr_time = get_time(rtc_handle);
        int hours = curr_time.hours;
        int minutes = curr_time.minutes;
        int seconds = curr_time.seconds;

        
        ESP_LOGI(TAG, "%02d:%02d:%02d", hours, minutes, seconds);
        
        // display time to ssd1306
        display_time(scr, curr_time);
    }
}

