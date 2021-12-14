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

//Public methods of main class
st7789::st7789(uint8_t rotation){




}

//Private methods of children class


//Public methods of children class