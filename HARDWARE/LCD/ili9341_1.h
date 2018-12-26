#ifndef ILI9341_1_H_
#define ILI9341_1_H_
 
#include "sys.h"
#include "stdlib.h"
 
//====================================TFT ILI9341=====================================
#define ILI9341_SPI          SPI1
 
#define ILI9341_MOSI_GRP     GPIOA
#define ILI9341_MOSI_IDX     GPIO_Pin_7
#define ILI9341_MOSI_LOW()   GPIO_ResetBits(ILI9341_MOSI_GRP, ILI9341_MOSI_IDX)
#define ILI9341_MOSI_HIGH()  GPIO_SetBits(ILI9341_MOSI_GRP, ILI9341_MOSI_IDX)
 
#define ILI9341_RS_GRP       GPIOA
#define ILI9341_RS_IDX       GPIO_Pin_3
#define ILI9341_RS_LOW()     GPIO_ResetBits(ILI9341_RS_GRP, ILI9341_RS_IDX)
#define ILI9341_RS_HIGH()    GPIO_SetBits(ILI9341_RS_GRP, ILI9341_RS_IDX)
 
#define ILI9341_CS_GRP       GPIOA
#define ILI9341_CS_IDX       GPIO_Pin_4
#define ILI9341_CS_LOW()     GPIO_ResetBits(ILI9341_CS_GRP, ILI9341_CS_IDX)
#define ILI9341_CS_HIGH()    GPIO_SetBits(ILI9341_CS_GRP, ILI9341_CS_IDX)
 
#define ILI9341_CLK_GRP      GPIOA
#define ILI9341_CLK_IDX      GPIO_Pin_5
#define ILI9341_CLK_LOW()    GPIO_ResetBits(ILI9341_CLK_GRP, ILI9341_CS_IDX)
#define ILI9341_CLK_HIGH()   GPIO_SetBits(ILI9341_CLK_IDX, ILI9341_CS_IDX)
 
#define ILI9341_ENLED_GRP    GPIOB
#define ILI9341_ENLED_IDX    GPIO_Pin_0
#define ILI9341_ENLED_LOW()  GPIO_ResetBits(ILI9341_ENLED_GRP, ILI9341_ENLED_IDX)
#define ILI9341_ENLED_HIGH() GPIO_SetBits(ILI9341_ENLED_GRP, ILI9341_ENLED_IDX)
 
#define ILI9341_RESET_GRP    GPIOB
#define ILI9341_RESET_IDX    GPIO_Pin_1
#define ILI9341_RESET_LOW()  GPIO_ResetBits(ILI9341_RESET_GRP, ILI9341_RESET_IDX)
#define ILI9341_RESET_HIGH() GPIO_SetBits(ILI9341_RESET_GRP, ILI9341_RESET_IDX)
 
void ILI9341Config(void);

void ili9341_cmd_write(uint8_t cmd);
void ili9341_data_write(uint8_t data);
void ili9341_data_wtite_16(uint8_t high, uint8_t low);
//void ILI9341SetCursor(uint16_t x, uint16_t y);
//void ILI9341SetColor(uint16_t rgb565);
//void ILI9341WriteData(uint8_t high,uint8_t low);



#endif
