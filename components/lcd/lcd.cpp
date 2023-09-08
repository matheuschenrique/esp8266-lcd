#include "lcd.hpp"

static const char *TAG = "main";

LCD::LCD(i2c_port_t i2c_port, i2c_mode_t mode, uint8_t slave_address, gpio_num_t sda, gpio_num_t scl, uint8_t lines, uint8_t columns) : 
    _slave_address(slave_address), _lines(lines), _columns(columns) {
        i2c_config_t config;
        config.mode = mode;
        config.sda_io_num = sda;
        config.sda_pullup_en = (gpio_pullup_t)(1);
        config.scl_io_num = scl;
        config.scl_pullup_en = (gpio_pullup_t)(1);
        config.clk_stretch_tick = 300; // 300 ticks, Clock stretch is about 210us, you can make changes according to the actual situation.
        ESP_ERROR_CHECK(i2c_driver_install(i2c_port, config.mode));
        ESP_ERROR_CHECK(i2c_param_config(i2c_port, &config));
}

esp_err_t LCD::i2c_lcd_write(i2c_port_t i2c_num, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, _slave_address << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send data to I2C device. Error: %s", esp_err_to_name(ret));
    }
    return ret;
}

void LCD::write_nibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = nibble << 4;
    data |= (rs << 0);
    // RW pin low (write operation)
    data |= (1 << 2); // enable = 1
    data |= (_backlight << 3);
    ESP_ERROR_CHECK(i2c_lcd_write(I2C_MASTER_NUM, data));

    vTaskDelay(1 / portTICK_PERIOD_MS);
    data &= ~(1 << 2); // high to low pulse at enable
    ESP_ERROR_CHECK(i2c_lcd_write(I2C_MASTER_NUM, data));
    vTaskDelay(40 / portTICK_PERIOD_MS);
}

void LCD::write_command(uint8_t cmd) {
    uint8_t aux = (cmd & 0xF0) >> 4;     // send upper nibble
    write_nibble(aux, 0);

    aux = (cmd & 0x0F);
    write_nibble(aux, 0);

    vTaskDelay(3 / portTICK_PERIOD_MS);
}

void LCD::write_data(uint8_t data) {
    uint8_t aux = (data & 0xF0) >> 4;
    write_nibble(aux, 1);

    aux = (data & 0x0F);
    write_nibble(aux, 1);
}

void LCD::write_string(const char *str) {
    while (*str) {
        write_data(*str++);
    }
}

void LCD::write_string_at(const char *str, uint8_t row, uint8_t column) {
    set_cursor(row, column);
    write_string(str);
}

void LCD::set_cursor(uint8_t row, uint8_t column) {
    uint8_t offset[] = {0x00, 0x40, 0x14, 0x54}; // can be used to 4 rows display
    if (row >= _lines) {
        row = _lines - 1;
    }
    if (column >= _columns) {
        column = _columns - 1;
    }
    write_command(LCD_SETDDRAMADDR | (column + offset[row]));
}

void LCD::clear(void) {
    write_command(LCD_CLEARDISPLAY);
}

void LCD::return_home(void) {
    write_command(LCD_RETURNHOME);
}

bool LCD::init(uint8_t backlight, uint8_t cursor_mode, uint8_t blink_mode) {
    _backlight = backlight;
    write_command(0x00);
    write_nibble(0x03, 0);
    write_nibble(0x03, 0);
    write_nibble(0x03, 0);
    write_nibble(0x02, 0);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    write_command(LCD_FUNCTIONSET | LCD_2LINE | LCD_4BITMODE);
    write_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | cursor_mode | blink_mode);
    clear();
    write_command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    return_home();
    vTaskDelay(2 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "*******************\n");
    return ESP_FAIL;
}