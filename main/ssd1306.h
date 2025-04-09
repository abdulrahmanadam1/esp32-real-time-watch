#ifndef SSD1306_H
#define SSD1306_H

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

#include "esp_lcd_io_i2c.h"
#include "esp_lcd_panel_vendor.h"

#include "esp_lvgl_port.h"
#include "lvgl.h"

#include "i2c.h"

#define OLED_ADDR 0x3C
lv_disp_t *oled_init(i2c_master_bus_handle_t i2c_handle);
void print_char(lv_disp_t *disp, const char *c);
void print_time(lv_disp_t *disp, int hour, int minute, int second);

#endif