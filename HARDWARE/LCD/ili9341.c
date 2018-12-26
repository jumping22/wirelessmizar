#include "ili9341.h"
#include "stdlib.h"
//#include "font.h" 
#include "usart.h"	 
#include "delay.h"	   
//#include "spi.h"	 
 				 
//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

//管理LCD重要参数
_lcd_dev lcddev;
	
uint8_t SPI_WriteByte(SPI_TypeDef* SPIx, uint8_t Byte);

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(u16 regval)
{ 
	SPILCD_CS_RESET;  //LCD_CS=0
	SPILCD_RS_RESET;
	SPI_WriteByte(SPI1,regval&0x00FF);
	SPILCD_CS_SET;  //LCD_CS=1	   		 
}

u16 LCD_RD_DATA(void)
{
	u8 a = 0;
	u8 b = 0;
	SPILCD_CS_RESET;  //LCD_CS=0
	SPILCD_RS_SET;				    	   
	a = SPI_WriteByte(SPI1,0x00);	
	b = SPI_WriteByte(SPI1,0x00);	
	SPILCD_CS_SET;  //LCD_CS=1   			 
	return (u16)a<<8 + b;
}	

u8 LCD_RD_DATA8(void)
{
	u8 a = 0;
	SPILCD_CS_RESET;  //LCD_CS=0
	SPILCD_RS_SET;				    	   
	a = SPI_WriteByte(SPI1,0x00);	
	SPILCD_CS_SET;  //LCD_CS=1   			 
	return a;
}	

//写LCD数据
//data:要写入的值
void LCD_WR_DATA(u16 data)
{
 	SPILCD_CS_RESET;  //LCD_CS=0
	SPILCD_RS_SET;	
	SPI_WriteByte(SPI1,data>>8);
	SPI_WriteByte(SPI1,data);
	SPILCD_CS_SET;  //LCD_CS=1		
}


void LCD_WR_DATA8(u8 da)   //写8位数据
{
	SPILCD_CS_RESET;  //LCD_CS=0
	SPILCD_RS_SET;				    	   
	SPI_WriteByte(SPI1,da);	
	SPILCD_CS_SET;  //LCD_CS=1   			 
}


//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WR_REG_DATA(u8 LCD_Reg, u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);  
}	 



void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{ 
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);     
}   

u16 LCD_ReadReg(u16 LCD_Reg)
{   
  LCD_WR_REG(LCD_Reg);
	return LCD_RD_DATA(); 
} 



//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}  		 
//LCD开启显示
void LCD_DisplayOn(void)
{					   

}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   

}   

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
  LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA8(Xpos>>8); 
	LCD_WR_DATA8(Xpos&0XFF);	 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA8(Ypos>>8); 
	LCD_WR_DATA8(Ypos&0XFF);
}

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(POINT_COLOR); 
} 



 void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTA时钟使能 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI1时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 ;	  //Pin_5 sck  pin7 MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA5/7/复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);//PA4	CS

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);//PB1	DC PA3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;//GPIO_Pin_0是背光现在用PWM来调 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);//PB0	LCD_RST PB1

	GPIO_SetBits(GPIOB, GPIO_Pin_0);		//bl pb0

 //	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工  SPI_Direction_1Line_Tx
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	//SPI2_ReadWriteByte(0xff);//启动传输	 
//	SPI1_SetSpeed(SPI_BaudRatePrescaler_256);//设置为18M时钟,高速模式

  //SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
} 

uint8_t SPI_WriteByte(SPI_TypeDef* SPIx, uint8_t Byte)
{
	while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPIx->DR=Byte;	 	//发送一个byte   
	while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);//等待接收完一个byte  
	return SPIx->DR;          	     //返回收到的数据			
}

//初始化lcd
void LCD_Init(void)
{
	SPI1_Init();
	
//	LCD_REST=0;		 
// 	delay_ms(50); // delay 20 ms 
//  LCD_REST=1;		 
// 	delay_ms(50); // delay 20 ms 

	SPILCD_RST_RESET ;	//LCD_RST=0	 //SPI接口复位
	delay_ms(20); // delay 20 ms 
  SPILCD_RST_SET ;	//LCD_RST=1		
	delay_ms(20);

	lcddev.width=240;
	lcddev.height=320;
	lcddev.wramcmd=0X2C;
	lcddev.setxcmd=0X2A;
	lcddev.setycmd=0X2B; 	

	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50);
	lcddev.id = LCD_ReadReg(0x0000);
	//delay_ms(500);
	LCD_WR_REG(0XD3);   
	LCD_RD_DATA8(); //dummy read 
  LCD_RD_DATA8();       //??0X00
  lcddev.id=LCD_RD_DATA8();   //??93   
  lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA8();   //??41  
	
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0xD9); 
	LCD_WR_DATA8(0X30); 
	 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA8(0x64); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0X12); 
	LCD_WR_DATA8(0X81); 
	 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA8(0x85); 
	LCD_WR_DATA8(0x10); 
	LCD_WR_DATA8(0x78); 
	 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA8(0x39); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x34); 
	LCD_WR_DATA8(0x02); 
	 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA8(0x20); 
	 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x00); 
	 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA8(0x21);   //VRH[5:0] 
	 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA8(0x12);   //SAP[2:0];BT[3:0] 
	 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA8(0x32); 
	LCD_WR_DATA8(0x3C); 
	 
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA8(0XC1); 
	 
//	LCD_WR_REG(0x36);    // Memory Access Control 
//	LCD_WR_DATA8(0x08); 
	
	LCD_WR_REG(0x36);     //set the model of scanning
	LCD_WR_DATA8((1<<5)|(0<<6)|(1<<7)|(1<<3)); //???
	//LCD_WR_DATA8((1<<5)|(1<<6));????
	//LCD_WR_DATA8(0x08); ???
	LCD_WR_REG(0x2B);     //set the page address ?????
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xEF);
	LCD_WR_REG(0x2A);    //set the column address
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x3F);
				
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA8(0x55); 
	
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA8(0x00);   
	LCD_WR_DATA8(0x18); 
	 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0xA2); 
	
	 
	 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA8(0x00); 
	 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA8(0x01); 
	 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA8(0x0F); 
	LCD_WR_DATA8(0x20); 
	LCD_WR_DATA8(0x1E); 
	LCD_WR_DATA8(0x09); 
	LCD_WR_DATA8(0x12); 
	LCD_WR_DATA8(0x0B); 
	LCD_WR_DATA8(0x50); 
	LCD_WR_DATA8(0XBA); 
	LCD_WR_DATA8(0x44); 
	LCD_WR_DATA8(0x09); 
	LCD_WR_DATA8(0x14); 
	LCD_WR_DATA8(0x05); 
	LCD_WR_DATA8(0x23); 
	LCD_WR_DATA8(0x21); 
	LCD_WR_DATA8(0x00); 
	 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x19); 
	LCD_WR_DATA8(0x19); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x12); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x28); 
	LCD_WR_DATA8(0x3F); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0x08); 
	LCD_WR_DATA8(0x25); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x0F); 
	 
	LCD_WR_REG(0x11);    //Exit Sleep 
	delay_ms(120); 
	LCD_WR_REG(0x29);    //Display on 

	LCD_Clear(WHITE); 
	
}  
//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 	//得到总点数
	//LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	LCD_SetCursor_FAST(0x00,0x0000);
	//LCD_WriteRAM_Prepare();     //开始写入GRAM
	LCD_WriteRAM_Prepare_FAST();
	
	for(index=0;index<totalpoint;index++)
	{
		//LCD_WR_DATA(color);
		LCD_WR_DATA_FAST(color);
	}
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{									   
	 	LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置 	    
	}
}  

//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 		//得到填充的宽度
	height=ey-sy+1;		//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++) LCD_WR_DATA(color[i*height+j]);//写入数据 
	}	  
}  
////画线
////x1,y1:起点坐标
////x2,y2:终点坐标  
//void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
//{
//	u16 t; 
//	int xerr=0,yerr=0,delta_x,delta_y,distance; 
//	int incx,incy,uRow,uCol; 
//	delta_x=x2-x1; //计算坐标增量 
//	delta_y=y2-y1; 
//	uRow=x1; 
//	uCol=y1; 
//	if(delta_x>0)incx=1; //设置单步方向 
//	else if(delta_x==0)incx=0;//垂直线 
//	else {incx=-1;delta_x=-delta_x;} 
//	if(delta_y>0)incy=1; 
//	else if(delta_y==0)incy=0;//水平线 
//	else{incy=-1;delta_y=-delta_y;} 
//	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
//	else distance=delta_y; 
//	for(t=0;t<=distance+1;t++ )//画线输出 
//	{  
//		LCD_DrawPoint(uRow,uCol);//画点 
//		xerr+=delta_x ; 
//		yerr+=delta_y ; 
//		if(xerr>distance) 
//		{ 
//			xerr-=distance; 
//			uRow+=incx; 
//		} 
//		if(yerr>distance) 
//		{ 
//			yerr-=distance; 
//			uCol+=incy; 
//		} 
//	}  
//}    
////画矩形	  
////(x1,y1),(x2,y2):矩形的对角坐标
//void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
//{
//	LCD_DrawLine(x1,y1,x2,y1);
//	LCD_DrawLine(x1,y1,x1,y2);
//	LCD_DrawLine(x1,y2,x2,y2);
//	LCD_DrawLine(x2,y1,x2,y2);
//}
////在指定位置画一个指定大小的圆
////(x,y):中心点
////r    :半径
//void Draw_Circle(u16 x0,u16 y0,u8 r)
//{
//	int a,b;
//	int di;
//	a=0;b=r;	  
//	di=3-(r<<1);             //判断下个点位置的标志
//	while(a<=b)
//	{
//		LCD_DrawPoint(x0+a,y0-b);             //5
// 		LCD_DrawPoint(x0+b,y0-a);             //0           
//		LCD_DrawPoint(x0+b,y0+a);             //4               
//		LCD_DrawPoint(x0+a,y0+b);             //6 
//		LCD_DrawPoint(x0-a,y0+b);             //1       
// 		LCD_DrawPoint(x0-b,y0+a);             
//		LCD_DrawPoint(x0-a,y0-b);             //2             
//  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
//		a++;
//		//使用Bresenham算法画圆     
//		if(di<0)di +=4*a+6;	  
//		else
//		{
//			di+=10+4*(a-b);   
//			b--;
//		} 						    
//	}
//} 	
////在指定位置显示一个汉字(16*16大小)
//void showhanzi16(unsigned int x,unsigned int y,unsigned char index)	
//{  
//	unsigned char i,j,k;
//	const unsigned char *temp=hanzi16;    
//	temp+=index*32;	
//	for(j=0;j<16;j++)
//	{
//		LCD_SetCursor(x,y+j);
//		LCD_WriteRAM_Prepare();	//开始写入GRAM
//		for(k=0;k<2;k++)
//		{
//			for(i=0;i<8;i++)
//			{ 		     
//			 	if((*temp&(1<<i))!=0)
//				{
//					LCD_WR_DATA(POINT_COLOR);
//				} 
//				else
//				{
//					LCD_WR_DATA(BACK_COLOR);
//				}   
//			}
//			temp++;
//		}
//	 }
//}	
////在指定位置显示一个汉字(32*32大小)
//void showhanzi32(unsigned int x,unsigned int y,unsigned char index)	
//{  
//	unsigned char i,j,k;
//	const unsigned char *temp=hanzi32;    
//	temp+=index*128;	
//	for(j=0;j<32;j++)
//	{
//		LCD_SetCursor(x,y+j);
//		LCD_WriteRAM_Prepare();	//开始写入GRAM
//		for(k=0;k<4;k++)
//		{
//			for(i=0;i<8;i++)
//			{ 		     
//			 	if((*temp&(1<<i))!=0)
//				{
//					LCD_WR_DATA(POINT_COLOR);
//				} 
//				else
//				{
//					LCD_WR_DATA(BACK_COLOR);
//				}   
//			}
//			temp++;
//		}
//	 }
//}													  
////在指定位置显示一个字符
////x,y:起始坐标
////num:要显示的字符:" "--->"~"
////size:字体大小 12/16
////mode:叠加方式(1)还是非叠加方式(0)
//void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
//{  							  
//    u8 temp,t1,t;
//	u16 y0=y;
//	u16 colortemp=POINT_COLOR;      			     
//	//设置窗口		   
//	num=num-' ';//得到偏移后的值
//	if(!mode) //非叠加方式
//	{
//	    for(t=0;t<size;t++)
//	    {   
//			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
//			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
//	        for(t1=0;t1<8;t1++)
//			{			    
//		        if(temp&0x80)POINT_COLOR=colortemp;
//				else POINT_COLOR=BACK_COLOR;
//				LCD_DrawPoint(x,y);	
//				temp<<=1;
//				y++;
//				if(y>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//					break;
//				}
//			}  	 
//	    }    
//	}else//叠加方式
//	{
//	    for(t=0;t<size;t++)
//	    {   
//			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
//			else temp=asc2_1608[num][t];		 //调用1608字体 	                          
//	        for(t1=0;t1<8;t1++)
//			{			    
//		        if(temp&0x80)LCD_DrawPoint(x,y); 
//				temp<<=1;
//				y++;
//				if(y>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
//				if((y-y0)==size)
//				{
//					y=y0;
//					x++;
//					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
//					break;
//				}
//			}  	 
//	    }     
//	}
//	POINT_COLOR=colortemp;	    	   	 	  
//}   
////m^n函数
////返回值:m^n次方.
//u32 LCD_Pow(u8 m,u8 n)
//{
//	u32 result=1;	 
//	while(n--)result*=m;    
//	return result;
//}			 
////显示数字,高位为0,则不显示
////x,y :起点坐标	 
////len :数字的位数
////size:字体大小
////color:颜色 
////num:数值(0~4294967295);	 
//void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
//{         	
//	u8 t,temp;
//	u8 enshow=0;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/LCD_Pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
//				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
//	}
//} 
////显示数字,高位为0,还是显示
////x,y:起点坐标
////num:数值(0~999999999);	 
////len:长度(即要显示的位数)
////size:字体大小
////mode:
////[7]:0,不填充;1,填充0.
////[6:1]:保留
////[0]:0,非叠加显示;1,叠加显示.
//void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
//{  
//	u8 t,temp;
//	u8 enshow=0;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/LCD_Pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
//				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
// 				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
//	}
//} 
////显示字符串
////x,y:起点坐标
////width,height:区域大小  
////size:字体大小
////*p:字符串起始地址		  
//void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
//{         
//	u8 x0=x;
//	width+=x;
//	height+=y;
//    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
//    {       
//        if(x>=width){x=x0;y+=size;}
//        if(y>=height)break;//退出
//        LCD_ShowChar(x,y,*p,size,1);
//        x+=size/2;
//        p++;
//    }  
//}

//void showimage(u16 x,u16 y) //显示40*40图片
//{  
//	u16 i,j,k;
//	u16 da;
//	k=0;
//	for(i=0;i<40;i++)
//	{	
//		LCD_SetCursor(x,y+i);
//		LCD_WriteRAM_Prepare();     			//开始写入GRAM	
//		for(j=0;j<40;j++)
//		{
//			da=qqimage[k*2+1];
//			da<<=8;
//			da|=qqimage[k*2]; 
//			LCD_WR_DATA(da);					
//			k++;  			
//		}
//	}
//}



