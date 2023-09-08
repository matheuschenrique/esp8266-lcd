#ifndef LCD_I2C_HPP
#define LCD_I2C_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "driver/gpio.h"
#include "driver/i2c.h"

#define LCD_SLAVE_ADDRESS                   0x27
#define I2C_MASTER_SCL_IO                   GPIO_NUM_5                /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO                   GPIO_NUM_4                /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM                      I2C_NUM_0        /*!< I2C port number for master dev */
#define WRITE_BIT                           I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                            I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                        0x1              /*!< I2C master will check ack from slave*/

#define LCD_COLUMNS 16
#define LCD_ROWS 2

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

class LCD {
private:
    uint8_t _slave_address;
    uint8_t _lines;
    uint8_t _columns;
    uint8_t _backlight;
public:
    LCD(i2c_port_t i2c_port, 
        i2c_mode_t mode, 
        uint8_t slave_address, 
        gpio_num_t sda, 
        gpio_num_t scl, 
        uint8_t lines, 
        uint8_t columns);
    esp_err_t i2c_lcd_write(i2c_port_t i2c_num, uint8_t data);
    bool init(uint8_t backlight, uint8_t cursor_mode, uint8_t blink_mode);
    void write_nibble(uint8_t nibble, uint8_t rs);
    void write_command(uint8_t cmd);
    void write_data(uint8_t data);
    void write_string(const char *str);
    void write_string_at(const char *str, uint8_t row, uint8_t column);
    void set_cursor(uint8_t row, uint8_t column);
    void clear(void);
    void return_home(void);
};

#endif