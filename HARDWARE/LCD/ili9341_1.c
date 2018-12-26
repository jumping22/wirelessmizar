#include "ili9341_1.h"
#include "delay.h"
 
/* ILI9341液晶的SPI GPIO初始化 */
static void ili9341_spi_init(SPI_TypeDef* SPIx, uint16_t spi_baud_div)
{
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
 
  /* clk和mosi是复用功能，其它引脚是通用推挽输出 */
  GPIO_InitStructure.GPIO_Pin  = ILI9341_MOSI_IDX | ILI9341_CLK_IDX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(ILI9341_MOSI_GRP, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin  = ILI9341_CS_IDX | ILI9341_RS_IDX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(ILI9341_CS_GRP, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin  = ILI9341_ENLED_IDX | ILI9341_RESET_IDX;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(ILI9341_RESET_GRP, &GPIO_InitStructure);
 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
 
  SPI_Init(SPIx, &SPI_InitStructure);
 
  SPI_Cmd(SPIx, ENABLE);
}
 
static void ili9341_spi_config(void)
{
  ili9341_spi_init(ILI9341_SPI, 0x02);
  ILI9341_ENLED_HIGH(); /* 点亮LCD */
  ILI9341_CS_HIGH();    /* 关闭失能cs */
  ILI9341_RESET_HIGH(); /* 复位引脚制高失能 */
  ILI9341_RS_HIGH();    /* RS制高 */
}
 
/* spi读写数据 */
static uint8_t ili9341_read_write_byte(uint8_t data)
{
  while (SPI_I2S_GetFlagStatus(ILI9341_SPI, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(ILI9341_SPI, data); 
  while (SPI_I2S_GetFlagStatus(ILI9341_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(ILI9341_SPI); //返回通过SPIx最近接收的数据
}
 
/* 写命令 */
void ili9341_cmd_write(uint8_t cmd)
{
  ILI9341_RS_LOW();
  ILI9341_CS_LOW();
 
  ili9341_read_write_byte(cmd);
 
  ILI9341_CS_HIGH();
}
 
/* 写数据 */
void ili9341_data_write(uint8_t data)
{
  ILI9341_RS_HIGH();
  ILI9341_CS_LOW();
 
  ili9341_read_write_byte(data);
 
  ILI9341_CS_HIGH();
}
 
/* 写16位数据 */
void ili9341_data_wtite_16(uint8_t high, uint8_t low)
{
  ILI9341_RS_HIGH();
  ILI9341_CS_LOW();
  ili9341_read_write_byte(high);
  ILI9341_CS_HIGH();
 
  ILI9341_RS_HIGH();
  ILI9341_CS_LOW();
  ili9341_read_write_byte(low);
  ILI9341_CS_HIGH();
}
 
/* 初始化9341液晶，根据手册时序 */
static void ili9341_init(void)
{
  ILI9341_RESET_LOW();
  delay_ms(10);
  ILI9341_RESET_HIGH();
  delay_ms(120);
  //************* Start Initial Sequence **********//
  ili9341_cmd_write(0xCB);
  ili9341_data_write (0x39);
  ili9341_data_write (0x2C);
  ili9341_data_write (0x00);
  ili9341_data_write (0x34);
  ili9341_data_write (0x02);
 
  ili9341_cmd_write(0xCF);
  ili9341_data_write (0x00);
  ili9341_data_write (0xC1);
  ili9341_data_write (0X30);
 
  ili9341_cmd_write(0xE8);
  ili9341_data_write (0x85);
  ili9341_data_write (0x00);
  ili9341_data_write (0x78);
 
  ili9341_cmd_write(0xEA);
  ili9341_data_write (0x00);
  ili9341_data_write (0x00);
 
  ili9341_cmd_write(0xED);
  ili9341_data_write (0x64);
  ili9341_data_write (0x03);
  ili9341_data_write (0X12);
  ili9341_data_write (0X81);
 
  ili9341_cmd_write(0xF7);
  ili9341_data_write(0x20);
 
  ili9341_cmd_write(0xC0);    // Power control
  ili9341_data_write (0x23);  // VRH[5:0]
 
  ili9341_cmd_write(0xC1);    // Power control
  ili9341_data_write (0x10);  // SAP[2:0];BT[3:0]
 
  ili9341_cmd_write(0xC5);    // VCM control
  ili9341_data_write (0x3e);
  ili9341_data_write (0x28);
 
  ili9341_cmd_write(0xC7);    // VCM control2
  ili9341_data_write (0X86);
 
  ili9341_cmd_write(0x36);    // Memory Access Control
  ili9341_data_write (0xe8);
 
  ili9341_cmd_write(0X2A);
  ili9341_data_write(0x00);   // start
  ili9341_data_write(0x00);
  ili9341_data_write(0x01);   // end
  ili9341_data_write(0x3F);
 
  ili9341_cmd_write(0X2B);
  ili9341_data_write(0x00);   // start
  ili9341_data_write(0x00);
  ili9341_data_write(0x00);   // end
  ili9341_data_write(0xEF);
 
  ili9341_cmd_write(0x3A);
  ili9341_data_write (0x55);
 
  ili9341_cmd_write(0xB1);
  ili9341_data_write (0x00);
  ili9341_data_write (0x18);
 
  ili9341_cmd_write(0xB6);    // Display Function Control
  ili9341_data_write (0x08);
  ili9341_data_write (0x82);
  ili9341_data_write (0x27);
 
  ili9341_cmd_write(0xF2);    // 3Gamma Function Disable
  ili9341_data_write (0x00);
 
  ili9341_cmd_write(0x26);    // Gamma curve selected
  ili9341_data_write (0x01);
 
  ili9341_cmd_write(0xE0);    // Set Gamma
  ili9341_data_write (0x0F);
  ili9341_data_write (0x31);
  ili9341_data_write (0x2b);
  ili9341_data_write (0x0c);
  ili9341_data_write (0x0e);
  ili9341_data_write (0x08);
  ili9341_data_write (0x4e);
  ili9341_data_write (0Xf1);
  ili9341_data_write (0x37);
  ili9341_data_write (0x07);
  ili9341_data_write (0x10);
  ili9341_data_write (0x03);
  ili9341_data_write (0x0e);
  ili9341_data_write (0x09);
  ili9341_data_write (0x00);
 
  ili9341_cmd_write(0XE1);    // Set Gamma
  ili9341_data_write (0x00);
  ili9341_data_write (0x0e);
  ili9341_data_write (0x14);
  ili9341_data_write (0x03);
  ili9341_data_write (0x11);
  ili9341_data_write (0x07);
  ili9341_data_write (0x31);
  ili9341_data_write (0xc1);
  ili9341_data_write (0x48);
  ili9341_data_write (0x08);
  ili9341_data_write (0x0f);
  ili9341_data_write (0x0C);
  ili9341_data_write (0x31);
  ili9341_data_write (0x36);
  ili9341_data_write (0x0f);
 
  ili9341_cmd_write(0x11);    // Exit Sleep
  delay_ms(120);
  ili9341_cmd_write(0x29);    // Display on
  ili9341_cmd_write(0x2c);
}
 
//=================================================================
/* ILI9341配置:GPIO配置，液晶初始化 */
void ILI9341Config(void)
{
  ili9341_spi_config(); /* 配置ILI9341GPIO和点亮屏 */
  ili9341_init();       /* ILI9341液晶初始化 */
}
 
//void ILI9341SetColor(uint16_t rgb565)
//{
//  uint8_t H;
//  uint8_t L;
//  H = (rgb565 & 0xFF00) >> 8;
//  L = (rgb565 & 0x00FF);
// 
//  WatchdogFeed();
// 
//  ili9341_data_wtite_16(H, L);
//}
// 
//void ILI9341SetCursor(uint16_t x, uint16_t y)
//{
//  ili9341_cmd_write(0x2a);
//  ili9341_data_wtite_16(x >> 8, x);
//  ili9341_cmd_write(0x2b);
//  ili9341_data_wtite_16(y >> 8, y);
//  ili9341_cmd_write(0x2c);
//}
// 
//void ILI9341WriteData(uint8_t high, uint8_t low)
//{
//  ili9341_data_wtite_16(high, low);
//}
