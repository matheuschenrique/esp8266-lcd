#include <stdio.h>
#include <string.h>

#include "lcd.hpp"

extern "C" {
  void app_main(void);
}

static void i2c_task(void *arg) {
    LCD lcd(I2C_MASTER_NUM, I2C_MODE_MASTER, LCD_SLAVE_ADDRESS, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO, LCD_ROWS, LCD_COLUMNS);
    lcd.init(1, LCD_CURSOROFF, LCD_BLINKOFF);
    lcd.set_cursor(0, 0);
    lcd.write_string_at("LCD is Working", 0, 0);
    while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    i2c_driver_delete(I2C_MASTER_NUM);
}

// app_main() is the entry point for our programs and it must be a C function:

void app_main() {
    xTaskCreate(i2c_task, "i2c_task", 2048, NULL, 10, NULL);
}