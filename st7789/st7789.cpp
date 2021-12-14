#include "st7789.hpp"


//Private methods of main class
static void st7789::reset(uint8_t mode){
    if (mode == 1){
        HAL_GPIO_WritePin(ST7789_RST_PORT, RST_Pin, GPIO_PIN_SET);
    }else{
        HAL_GPIO_WritePin(ST7789_RST_PORT, RST_Pin, GPIO_PIN_RESET);
    }
}

static void st7789::dc(uint8_t mode){
    if(mode == 1){
        HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    }else{
        HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
    }
}

static void st7789::chip_select(uint8_t mode){
    if(mode == 1){
        HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    }else{
        HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
    }
}

static void st7789::write_command(uint8_t cmd){
    chip_select(1);
    dc(0);
    HAL_SPI_Transmit(&SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    chip_select(0);
}

static void st7789::write_data(uint8_t *buff, size_t size){
    chip_select(1);
    dc(1);
    while(size > 0){
        uint16_t chunk = size > 65535 ? 65535 : size;
        HAL_SPI_Transmit(&SPI_PORT, buff, chunk, HAL_MAX_DELAY);
        buff += chunk;
        size -= chunk;
    }
    chip_select(0);
}

static void st7789::write_small_data(uint8_t data){
    chip_select(1);
    dc(1);
    HAL_SPI_Transmit(&SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);
    chip_select(0);
}

static void st7789::set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
    chip_select(1);
    uint16_t x_start = x0 + this.x_shift, x_end = x1 + this.x_shift;
    uint16_t y_start = y0 + this.y_shift, y_end = y1 + this.y_shift;

    write_command(ST7789_CASET);
    {
        uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, y_end & 0xFF};
        write_data(data, sizeof(data));
    }

    write_command(ST7789_RASET);
    {
        uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
        write_data(data, sizeof(data));
    }
    write_command(ST7789_RAMWR);
    chip_select(0);

}

static void st7789::set_rotation(uint8_t rotation){
        write_command(ST7789_MADCTL);
    switch(rotation){
        case 0: 
            write_small_data(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
        break;

        case 1: 
            write_small_data(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
        break;

        case 2: 
            write_small_data(ST7789_MADCTL_RGB);
        break;

        case 3: 
            write_small_data(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
        break;
    }
}

//Public methods of main class
static void st7789::fill(uint8_t colour){
    set_window(0,0 WIDTH - 1, HIGH - 1);
    chip_select(1);
    for(uint16_t i = 0; i < WIDTH; i++){
        for(uint16_t k = 0; k < HIGH; k++){
            uint8_t data[] = {colour >> 8, colour & 0xFF};
            write_data(data, sizeof(data));
        }
    }
    chip_select(0);

}

static void st7789::fill_area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t colour){
    if ((x1 < 0) || (x1 > WIDTH) || (y1 < 0) || (y1 > HIGH)) return;

    chip_select(1);
    set_window(x0, y0, x1, y1);
    for(uint16_t i = x0; i < x1; i++){
        for(uint16_t k = y0; k < y1; k++){
            uint8_t data[] = {colour >> 8, colour & 0xFF};
            write_data(data, sizeof(data));
        }
    }
    chip_select(0);

}

st7789::st7789(uint8_t rotation){
    HAL_Delay(25);
    reset(0);
    HAL_Delay(25);
    reset(1);
    HAL_Delay(25);

    write_command(ST7789_COLMOD);
    write_small_data(ST7789_COLOR_MODE_16bit);
    write_command(0xB2);
    {
        uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
        write_data(data, sizeof(data));
    }
    set_rotaion(rotation);

    //Init LCD voltage generator
    write_command(0xB7);
    write_small_data(0x32);

    write_command(0xBB);
    write_small_data(0x19);

    write_command(0xC0);
    write_small_data(0x2C);

    write_command(0x2C);
    write_small_data(0x01);

    write_command(0xC3);
    write_small_data(0x12);

    write_command(0xC4);
    write_small_data(0x20);

    write_command(0x6C);
    write_small_data(0x0F);

    write_command(0x0D);
    write_small_data(0xA4);
    write_small_data(0xA1);

    write_command(0xE0);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
        write_data(data, sizeof(data));
    }

    write_command(0xE1);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
        write_data(data, sizeof(data));
    }
    write_command (ST7789_INVON);    //  Inversion ON
    write_command (ST7789_SLPOUT);  //  Out of sleep mode
    write_command (ST7789_NORON);    //  Normal Display on
    write_command (ST7789_DISPON);  //  Main screen turned on  

    HAL_Delay(50);
    fill(BLACK);

}

static void st7789::draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data){
    if ((x >= WIDTH) || (y >= HIGH)) return;
    if((x + w - 1) >= WIDTH) return;
    if((y + h - 1) >= HIGH) return;

    chip_select(1);
    set_window(x, y, x + w - 1, y + h - 1);
    write_data((uint8_t*) data, sizeof(uint16_t) * w * h);
    chip_select(0);
}

static void st7789::write_char(uint16_t x, uint16_t y, char symbol, FontDef font, uint16_t color, uint16_t bgcolor){
    chip_select(1);
    set_window(x, y, x + font.width - 1, y + font.height - 1);
    uint32_t buff;
    for(uint32_t i = 0; i < font.height; i++){
        buff = font.data[(symbol - 32) * font.height + i];
        for(uint32_t k = 0; k < font.width; k++){
            if((buff << k) & 0x8000){
                uint8_t data[] = {color >> 8, color & 0xFF};
                write_data(data, sizeof(data));
            }else{
                uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
                write_data(data, sizeof(data));
            }
        }
    }
    chip_select(0);
}

static void st7789::write_string(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor){
    chip_select(1);
    while(*str){
        if(x + font.width >= WIDTH){
            x = 0;
            y += font.height;

            if(y + font.height >= HIGH){
                break;
            }

            if(*str == ' '){
                str++;
                continue;
            }
        }
        write_char(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }
    chip_select(0);
}
//Private methods of children class


//Public methods of children class