#include "ntp.h"


static const char *TAG = "NTP";



rtc_time_t get_current_datetime()
{
  char str_time_buf[64];
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);

  setenv("TZ", "UTC+7:00", 1);
  tzset();
  localtime_r(&now, &timeinfo);
  strftime(str_time_buf, sizeof(str_time_buf), "%c", &timeinfo);
  ESP_LOGI(TAG, "The current date/time in Seattle is: %s", str_time_buf);

  rtc_time_t curr_time = {
    .hours = timeinfo.tm_hour,
    .minutes = timeinfo.tm_min,
    .seconds = timeinfo.tm_sec,
  };


  return curr_time;
}


void obtain_time()
{
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_set_time_sync_notification_cb(sntp_set_time_sync_notification_cb);
  sntp_init();

  time_t now = 0;
  struct tm timeinfo = {0};
  int retry = 0;

  while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < 5)
  {
    ESP_LOGI(TAG, "Waiting fo system %d", retry);
    vTaskDelay( 2000 / portTICK_PERIOD_MS);
  }
  time(&now);
  localtime_r(&now, &timeinfo);
}

void set_sys_sntp()
{
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  if (timeinfo.tm_year < (2016 - 1900))
  {
    obtain_time();
    time(&now);
  }

}


void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        ESP_LOGI(TAG,"WIFI_EVENT_STA_START\n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "WIFI_EVENT_STA_CONNECTED\n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGE(TAG, "WIFI_EVENT_STA_DISCONNECTED\n");
        for (int i  = 0; i < 5; i++)
        {
          esp_wifi_connect();
          ESP_LOGE(TAG, "RETRYING WIFI CONNECT %d", i);
        }
        break;
    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "IP_EVENT_STA_GOT_IP\n");
        break;
    default:
        break;
    }
}

void wifi_connection()
{
    esp_netif_init();                    
    esp_event_loop_create_default();     
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); 
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS
          }
        };
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_connect();
}



void ntp_init()
{
  nvs_flash_init();
  wifi_connection();
  set_sys_sntp();
}