#ifndef __ST7789_H
#define __ST7789_H

#include "main.h"
#include "font.hpp"

#define SPI_PORT hspi1
extern SPI_HandleTypeDef SPI_PORT;

#ifdef hspi1
#define CS_GPIO_Port    GPIO_PIN_3
#define CS_Pin          GPIOA
#define RST_GPIO_Port   GPIO_PIN_4
#define RST_Pin         GPIOA
#define DC_GPIO_Port    GPIO_PIN_6
#define DC_Pin          GPIOA
#define BLK_GPIO_Port   GPIO_PIN_2
#define BLK_Pin         GPIOA
#endif

#ifndef CUSTOM_RESOLUTION 
#define HIGH 240
#define WIDTH 320
#endif

//Registers codes
#define ST7789_NOP 0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID 0X04
#define ST7789_RDDST 0X09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1    0xDA
#define ST7789_RDID2    0xDB
#define ST7789_RDID3    0xDC
#define ST7789_RDID4    0xDD

//Colour mode
#define ST7789_COLOR_MODE_16bit 0x55

//Colours codes
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define GRAY        0x8430
#define BRED        0xF81F
#define GRED        0xFFE0
#define GBLUE       0x07FF
#define BROWN       0xBC40
#define BRRED       0xFC07
#define DARKBLUE    0x01CF
#define LIGHTBLUE   0x7D7C
#define GRAYBLUE    0x5458

#define LIGHTGREEN  0x841F
#define LGRAY       0xC618    
#define LGRAYBLUE   0xA651
#define LBBLUE      0x2B12


//Macro
#define ABS(x) ((x) > 0 ? (x) : -(x))

class st7789{
    public:
        st7789(uint8_t rotation);
        static void fill(uint8_t colour);
        static void fill_area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour);
        static void draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
        static void write_char(uint16_t x, uint16_t y, char symbol, FontDef font, uint16_t color, uint16_t bgcolor);
        static void write_string(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor);
    private:
        static void reset(uint8_t mode);
        static void dc(uint8_t mode);
        static void chip_select(uint8_t mode);

        static void write_command(uint8_t cmd);
        static void write_data(uint8_t *buff, size_t size);
        static void write_small_data(uint8_t data);
        static void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
        static void set_rotaion(uint8_t rotation);
        uint8_t x_shift,
        y_shift;


};

class graphic: protected st7789{
    public:
        graphic();
    private:

};
#endif