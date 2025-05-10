#ifndef NTP_H
#define NTP_H

#include <stdio.h>

#include "esp_sntp.h"
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "rtc.h"

// PUT WIFI SSID/PASSWORD HERE
#define WIFI_SSID ""
#define WIFI_PASS ""

void ntp_init();
void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void wifi_connection();
void obtain_time();
void set_sys_sntp();
void initialize_sntp();
rtc_time_t get_current_datetime();

#endif