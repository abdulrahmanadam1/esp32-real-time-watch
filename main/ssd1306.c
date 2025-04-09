#include "ssd1306.h"

static const char *TAG = "SSD1306";

void print_char(lv_disp_t *disp, const char *c)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, c);
    // int32_t w = *(disp->driver->hor_res);
    lv_obj_set_width(label, 128);
    lv_obj_align(label, LV_ALIGN_OUT_TOP_MID, 0, 0);

}


void print_time(lv_disp_t *disp, int hour, int minute, int second)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);

    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text_fmt(scr, "%d:%d:%d",hour, minute, second);
    lv_obj_set_width(label, 128);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 0);

}


lv_disp_t *oled_init(i2c_master_bus_handle_t i2c_handle)
{
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = OLED_ADDR,
        .scl_speed_hz = OLED_I2C_CLK_SPEED,
        .control_phase_bytes = 1,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .dc_bit_offset = 6
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_handle, &io_config, &io_handle));
    ESP_LOGI(TAG, "SSD1306 panel IO installed");

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = -1,
    };
    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = 128
    };
    panel_config.vendor_config = &ssd1306_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    ESP_LOGI(TAG, "INIT LVGL");
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_port_init(&lvgl_cfg);

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = 128 * 64,
        .hres = 128,
        .vres = 64,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };
    lv_disp_t *disp = lvgl_port_add_disp(&disp_cfg);

    lv_disp_set_rotation(disp, 0);
    
    return disp;

}
