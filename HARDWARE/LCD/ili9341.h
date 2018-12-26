#ifndef __ILI9341_H
#define __ILI9341_H		
#include "sys.h"	 
#include "stdlib.h"

//SPI��ʾ���ӿ�
//LCD_RST
#define SPILCD_RST_SET  GPIO_SetBits(GPIOB, GPIO_Pin_1)//PB0 PB1
#define SPILCD_RST_RESET GPIO_ResetBits(GPIOB, GPIO_Pin_1)//PB0  PB1
//LCD_RS//dc  
#define SPILCD_RS_SET  GPIO_SetBits(GPIOA, GPIO_Pin_3)//PB1  PA3
#define SPILCD_RS_RESET  GPIO_ResetBits(GPIOA, GPIO_Pin_3)//PB1   PA3
//LCD_CS  
#define SPILCD_CS_SET GPIO_SetBits(GPIOA, GPIO_Pin_4)//PA4 
#define SPILCD_CS_RESET  GPIO_ResetBits(GPIOA, GPIO_Pin_4)//PA4 

  
//LCD��Ҫ������
typedef struct  
{ 					    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8	wramcmd;		//��ʼдgramָ��
	u8  setxcmd;		//����x����ָ��
	u8  setycmd;		//����y����ָ��	 
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD�˿ڶ���---------------- 
#define	LCD_REST PBout(1) //LCD REST    		 PB1 	    
////LCD��ַ�ṹ��
//typedef struct
//{
//	u16 LCD_REG;
//	u16 LCD_RAM;
//} LCD_TypeDef;
////ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A10��Ϊ�������������� 
////ע������ʱSTM32�ڲ�������һλ����! 111110=0X3E			    
//#define LCD_BASE        ((u32)(0x60000000 | 0x0007FFFE))
//#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
void LCD_Init(void);													   	//��ʼ��
void LCD_DisplayOn(void);													//����ʾ
void LCD_DisplayOff(void);													//����ʾ
void LCD_Clear(u16 Color);	 												//����

void LCD_WR_REG(u16 regval);
#define LCD_WR_REG_FAST(regval) \
	SPILCD_CS_RESET; \
	SPILCD_RS_RESET; \
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET); \
	SPI1->DR = (uint8_t)(regval); \
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET); \
	(void)SPI1->DR; \
	SPILCD_CS_SET;

void LCD_WR_DATA(u16 data);
#define LCD_WR_DATA_FAST(data) \
{ \
 	SPILCD_CS_RESET; \
	SPILCD_RS_SET; \
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET); \
	SPI1->DR = (uint8_t)(data>>8); \
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET); \
	(void)SPI1->DR; \
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET); \
	SPI1->DR = (uint8_t)data; \
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET); \
	(void)SPI1->DR; \
	SPILCD_CS_SET; \
}

#define LCD_WR_DATA8_FAST(da) \
	SPILCD_CS_RESET; \
	SPILCD_RS_SET; \
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET); \
	SPI1->DR = (uint8_t)(da); \
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET); \
	(void)SPI1->DR; \
	SPILCD_CS_SET;
	
void LCD_WriteRAM_Prepare(void);
#define LCD_WriteRAM_Prepare_FAST() \
	LCD_WR_REG_FAST(lcddev.wramcmd); \

void LCD_SetCursor(u16 Xpos, u16 Ypos);										//���ù��
#define LCD_SetCursor_FAST(Xpos, Ypos) \
  LCD_WR_REG_FAST(lcddev.setxcmd); \
	LCD_WR_DATA8_FAST(Xpos>>8);  \
	LCD_WR_DATA8_FAST(Xpos&0XFF); \
	LCD_WR_REG_FAST(lcddev.setycmd); \
	LCD_WR_DATA8_FAST(Ypos>>8); \
	LCD_WR_DATA8_FAST(Ypos&0XFF);
	

void LCD_DrawPoint(u16 x,u16 y);											//����
#define LCD_DrawPoint_FAST(x, y, c) \
	LCD_SetCursor_FAST(x,y); \
	LCD_WriteRAM_Prepare_FAST(); \
	LCD_WR_DATA_FAST(c); 

//void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//���ٻ���
//void Draw_Circle(u16 x0,u16 y0,u8 r);										//��Բ
//void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//����
//void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//������
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//��䵥ɫ
#define LCD_Fill_FAST(sx, sy, ex, ey, color) \
	u16 i,j; \
	u16 xlen=0; \
	xlen=ex-sx+1; \
	for(i=sy;i<=ey;i++) { \
	 	LCD_SetCursor_FAST(sx,i); \
		LCD_WriteRAM_Prepare_FAST(); \
		for(j=0;j<xlen;j++)LCD_WR_DATA_FAST(color); \
	}
	
	
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//���ָ����ɫ
//void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//��ʾһ���ַ�
//void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//��ʾһ������
//void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//��ʾ ����
//void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//��ʾһ���ַ���,12/16����
//	  
//void showimage(u16 x,u16 y); //��ʾ40*40ͼƬ
//void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
//u16 LCD_ReadReg(u16 LCD_Reg);
//void LCD_WriteRAM_Prepare(void);
//void LCD_WR_DATA8(u8 da);   //д8λ����  
// 
//void showhanzi16(unsigned int x,unsigned int y,unsigned char index);//16*16����
//void showhanzi32(unsigned int x,unsigned int y,unsigned char index);//32*32����						  		 
#endif  
	 
	 



