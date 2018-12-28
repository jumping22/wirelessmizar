#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"

#if defined(USING_ILI9341_8BIT)
 #include "ILI93xx.h"
#elif defined(USING_ILI9341_SPI)
 #include "ili9341.h"
#endif

#include "spi.h"
#include "flash.h"
#include "GUI.h"
#include "i2c_STX.h"

#include "microbit_iic.h"
#include "EndDevice.h"
#include "multi_button.h"
#include "pwm.h"
#include "adc.h"
#include "Font.h"
/****************************************Copyright (c)****************************************************
**--------------------------------------------------------------------------------------------------------
** Created by		WUDONG SHANG
** Modified by:    ENJIE DU	&	JUMPING WANG
** Modified date:        2018-12-25 CHRISTMAS DAY	- ···  
** Version:               V0.3
** Descriptions:          2D绘图添加  
** All copyright by LANDZO Electronic 
*********************************************************************************************************/
void TIM3_Int_Init(u16 arr,u16 psc);
void multi_button_init(void);
void multi_button_event(void);
void PWR_5182CTL_Init(void);
void home_page(void);
void Sensors_page(void);
void Voltage_Pros(void);

const GUI_POINT OutPoints[] = {
	{ 8, 5 },
	{ 11, 5 },
	{ 11, 0 },
	{ 40, 0 },
	{ 40, 15 },
	{ 11, 15 },
	{ 11, 10 },
	{ 8, 10}
};
const GUI_POINT IN_Point1[] = {
	{33,3},
	{37,3},
	{37,13},
	{33,13}
};
const GUI_POINT IN_Point2[] = {
	{ 27, 3 },
	{ 31, 3 },
	{ 31, 13 },
	{ 27, 13 }
};
const GUI_POINT IN_Point3[] = {
	{ 21, 3 },
	{ 25, 3 },
	{ 25, 13 },
	{ 21, 13 }
};
const GUI_POINT IN_Point4[] = {
	{ 15, 3 },
	{ 19, 3 },
	{ 19, 13 },
	{ 15, 13 }
};

const GUI_POINT Point_USB[] = {
	{ 5, 10 },
	{ 20, 10 },
	{ 30, 2 },
	{ 30, 6 },
	{ 36, 6 },
	{ 36, 8 },
	{ 30, 8 },
	{ 30, 16 },
	{ 36, 16 },
	{ 36, 18 },
	{ 30, 18 },//
	{ 30, 22 },
	{ 20, 14 },
	{ 5, 14 }
};

void Show_IR_Distance(void);
void Show_IR_Remote(void);
void Show_PRS(void);
void Show_DHT11(void);
void Show_PIR(void);
void Show_Potentiometer(void);
void Show_Large_Key(void);



WirelessDevice_class *wireless;
uint32_t timer1 = 0;
GUI_RECT Rect = { 0, 0, 200, 200 };
uint8_t nextpag_flg = 0;
u16 bat_Voltage=0;
unsigned char tim_flg = 1;
uint8_t user_gui_interface = 0;	//用户界面编程标志
uint8_t user_gui_stop = 0;//非用户界面暂停恢复标志
uint8_t page_number = 0;
extern void MainTask(void);
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong;
extern const char str_Landzo[] = {"\xe8\x93\x9d\xe5\xae\x99\xe7\x94\xb5\xe5\xad\x90"};
extern const char str_IR_Distance[] = {"\xe7\xba\xa2\xe5\xa4\x96\xe6\xb5\x8b\xe8\xb7\x9d"};
extern const char str_PRS[] = {"\xe5\x85\x89\xe6\x95\x8f\xe7\x94\xb5\xe9\x98\xbb"};
extern const char str_Dht11[] = {"\xe6\xb8\xa9\xe6\xb9\xbf\xe5\xba\xa6"};
extern const char str_Dht11_temp[] = {"\xe6\xb8\xa9\xe5\xba\xa6"};
extern const char str_Dht11_humi[] = {"\xe6\xb9\xbf\xe5\xba\xa6"};
extern const char str_PIR[] = {"\xe4\xba\xba\xe4\xbd\x93\xe7\xba\xa2\xe5\xa4\x96"};
extern const char str_bigkey[] = {"\xe5\xa4\xa7\xe6\x8c\x89\xe9\x94\xae"};
extern const char str_Potentiometer[] = {"\xe6\x97\x8b\xe8\xbd\xac\xe7\x94\xb5\xe4\xbd\x8d\xe5\x99\xa8"};
extern const char str_IR_Remote[] = {"\xe7\xba\xa2\xe5\xa4\x96\xe6\x8e\xa5\xe5\x8f\x97"};
extern const char str_stop[] = {"\xe6\x9a\x82\xe5\x81\x9c"};
extern const char str_back[] = {"\xe4\xb8\x8a\xe4\xb8\x80\xe9\xa1\xb5"};
extern const char str_next[] = {"\xe4\xb8\x8b\xe4\xb8\x80\xe9\xa1\xb5"};
extern const char str_begin[] = {"\xe6\x81\xa2\xe5\xa4\x8d"};

int main(void)
{
	delay_init(72);	     //延时初始化
	NVIC_Configuration();
	INTX_DISABLE();//关总中断
	PWR_5182CTL_Init();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	Adc_Init();
	//TIM3_PWM_Init();//屏幕背光
	wireless = WirelessDevice_Init(uart_send);
	uart1_SetRecvCallback(wireless->analyze);
	I2C1_SetRecvCallback(microbit_recv_analyze);
	multi_button_init();
	uart_init(115200);
	I2C1_Slave_init();
	
#if defined(USING_LCD)
	TIM3_Int_Init(999,71);	//1KHZ 定时器1ms
	#if defined(USING_ILI9341_8BIT)
		TFTLCD_Init();
	#elif defined(USING_ILI9341_SPI)
		LCD_Init();
	#endif
	GUI_Init();  
#endif
	wireless->Router->router_init();
	INTX_ENABLE();//开总中断
	GPIO_SetBits(GPIOB,GPIO_Pin_10);//打开5182电源		
	
	//WM_MULTIBUF_Enable(1);
	
	GUI_UC_SetEncodeUTF8();
//	GUI_SetFont(GUI_FONT_20_ASCII);
//	GUI_SetFont(&GUI_FontFontSong);
	//MainTask();
	
	//GUI_DispStringAt(&lanzhoudianzi1[0], 0, 0);
	//tim_flg=1;
	//Sensors_page();
//	GUI_SetColor(GUI_WHITE);
//	GUI_SetPenSize(2);

//	GUI_SetFont(&GUI_FontFontSong);
//	GUI_DispStringAt(str_Landzo,3, 3);
//	//GUI_SetFont(&GUI_FontFontSong);
//	GUI_DispStringAt(str_next,5,219);
//	GUI_DispStringAt(str_back, 90, 219);
//	GUI_DispStringAt(str_stop, 185, 219); 
//	GUI_DispStringAt(str_begin, 250, 219);
//	GUI_SetFont(GUI_FONT_20_ASCII);
//	GUI_DrawPolygon(OutPoints, 8, 270, 5);
//	GUI_SetPenSize(1);
//	GUI_SetLineStyle(GUI_LS_DASH);
//	GUI_DrawLine(0, 25, 320, 25);
//	GUI_DrawLine(0, 214, 320, 214);
//	GUI_SetColor(GUI_LIGHTGRAY);
//	//GUI_DrawGradientRoundedH(10, 30, 310, 210, 15, 0x00838383,0x00D3D3D3);
//	GUI_AA_SetFactor(15);
//	GUI_AA_FillRoundedRect(10, 30, 310, 210, 15);
//	GUI_SetColor(GUI_BLACK);
//	GUI_SetTextMode(GUI_TM_TRANS);
	
	while(1) {
#if defined(USING_LCD)
		GUI_Delay(10);
#endif
		timer1++;
		multi_button_event();
		if (user_gui_interface == 0) {
			if(user_gui_stop == 0)
			{
						if(timer1 % 200 == 0)//2S刷一次
						{
							if(page_number == 0 ){
								Voltage_Pros();
							}
						}
//						if(nextpag_flg ==1)
//						{
								//nextpag_flg=2;
							if (timer1 % 100 == 0) { //1s
								//if(ShowSensorData_flg==1){
											

										switch (page_number){
											
											case 0:{
												Sensors_page();
											}break;
											
											case 1:{
//												GUI_SetTextMode(GUI_TM_TRANS);
//												GUI_SetFont(GUI_FONT_8X18);
												Show_IR_Distance();
											}break;
											
											case 2:{
//												GUI_SetTextMode(GUI_TM_TRANS);
//												GUI_SetFont(GUI_FONT_8X18);
												Show_IR_Remote();
											}break;
											
											case 3:{
//												GUI_SetTextMode(GUI_TM_TRANS);
//												GUI_SetFont(GUI_FONT_8X18);
												Show_PRS();
											}break;
											
											case 4:{
//												GUI_SetTextMode(GUI_TM_TRANS);
//												GUI_SetFont(GUI_FONT_8X18);
												Show_DHT11();
											}break;
											
											case 5:{
//												GUI_SetTextMode(GUI_TM_TRANS);
//												GUI_SetFont(GUI_FONT_8X18);
												Show_PIR();
											}break;
											
											case 6:{
//												GUI_SetTextMode(GUI_TM_TRANS);
//												GUI_SetFont(GUI_FONT_8X18);
												Show_Potentiometer();
											}break;
											
											case 7:{
//												GUI_SetTextMode(GUI_TM_TRANS);
//												GUI_SetFont(GUI_FONT_8X18);
												Show_Large_Key();
											}break;
											default :break;
											
					
					
				}
				//						Sensors_page();
				//					GUI_SetTextMode(GUI_TM_TRANS);
				//					GUI_SetFont(GUI_FONT_8X18);
				//					Show_IR_Distance();
				//					Show_IR_Remote();
				//					Show_PRS();
				//					Show_DHT11();
				//					Show_PIR();
				//					Show_Potentiometer();
				//					Show_Large_Key();
										
				}
//						}
//						else if(nextpag_flg==0)
//						{
//							nextpag_flg=2;
//							//GUI_Clear();
//							home_page();
//						}
			}
		}
	}
}

static struct button btn1;
static struct button btn2;
static struct button btn3;
static struct button btn4;

PressEvent btn1_event;
PressEvent btn2_event;
PressEvent btn3_event;
PressEvent btn4_event;

static uint8_t btn1_read(void);
static uint8_t btn2_read(void);
static uint8_t btn3_read(void);
static uint8_t btn4_read(void);

void multi_button_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	button_init(&btn1, btn1_read, 0);
	button_init(&btn2, btn2_read, 0);
	button_init(&btn3, btn3_read, 0);
	button_init(&btn4, btn4_read, 0);
	
	button_start(&btn1);
	button_start(&btn2);
	button_start(&btn3);
	button_start(&btn4);
}

void multi_button_event(void)
{
	uint32_t btn_event_val;
	//GUI_SetColor(GUI_BLUE);	 
	if(btn_event_val != get_button_event(&btn1)) {
		//btn_event_val = get_button_event(&btn1);
		btn1_event = get_button_event(&btn1);
		switch((uint8_t)btn1_event)
		{
			case NONE_PRESS:
				//GUI_DispStringAtCEOL("None press!",10,30);
				break;
			case SINGLE_CLICK:
				if (user_gui_interface == 0) {
					page_number ++;
					if(page_number > 7)
					{
						page_number = 1;
					}
					tim_flg = 1;
					GUI_Clear();
				}
			break;//GUI_DispStringAt("key1_press!",15,120) 
			case PRESS_UP: break;
			default: break;			

		}
	}
	
		if(btn_event_val != get_button_event(&btn2)) {
		btn2_event = get_button_event(&btn2);
		switch((uint8_t)btn2_event)
		{
			case NONE_PRESS:break;
			case SINGLE_CLICK:{
					if (user_gui_interface == 0) {
						page_number --;
						if(page_number < 1)
						{
							page_number = 7;
						}
						tim_flg = 1;
						GUI_Clear();
					}
			}break; //GUI_DispStringAt("key2_press!",15,120);
			
			case PRESS_UP: break;
			default: break;			
		}
	}
	  if(btn_event_val != get_button_event(&btn3)) {
		btn3_event = get_button_event(&btn3);
		switch((uint8_t)btn3_event)
		{
			case NONE_PRESS:break;
			case SINGLE_CLICK:{
				user_gui_stop = !user_gui_stop;	
			}break; 
			
			case PRESS_UP: break;
			default: break;			
		}
	}
		if(btn_event_val != get_button_event(&btn4)) {
		btn4_event = get_button_event(&btn4);
		switch((uint8_t)btn4_event)
		{
			case NONE_PRESS:break;
			case SINGLE_CLICK:
			{
				page_number = 0;
				tim_flg=1;
				GUI_Clear();
			}break; 
			
			case PRESS_UP: break;
			default: break;			
		}
	}
	
	if(btn_event_val != get_button_event(&btn2)) {
		btn2_event = get_button_event(&btn2);
	}
	if(btn_event_val != get_button_event(&btn3)) {
		btn3_event = get_button_event(&btn3);
	}
	if(btn_event_val != get_button_event(&btn4)) {
		btn4_event = get_button_event(&btn4);
	}
	button_ticks(); 
}

static uint8_t btn1_read(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
}
static uint8_t btn2_read(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
}
static uint8_t btn3_read(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}
static uint8_t btn4_read(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
}




extern __IO int32_t OS_TimeMS;
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启TIM3时钟 

	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;   //分频值
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	   //计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;		   //自动重装数值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //设置时钟分割
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3,ENABLE);		 //使能TIM3
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET) {	
		OS_TimeMS++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

void PWR_5182CTL_Init()
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}

void home_page(void)
{
	GUI_SetColor(GUI_WHITE);
	GUI_SetPenSize(2);

	GUI_SetFont(&GUI_FontFontSong);
	GUI_DispStringAt(str_Landzo,3, 3);
	//GUI_SetFont(&GUI_FontFontSong);
	GUI_DispStringAt(str_next,5,219);
	GUI_DispStringAt(str_back, 90, 219);
	GUI_DispStringAt(str_stop, 185, 219); 
	GUI_DispStringAt(str_begin, 250, 219);
	GUI_SetFont(GUI_FONT_20_ASCII);
	GUI_DrawPolygon(OutPoints, 8, 270, 5);
	GUI_SetPenSize(1);
	GUI_SetLineStyle(GUI_LS_DASH);
	GUI_DrawLine(0, 25, 320, 25);
	GUI_DrawLine(0, 214, 320, 214);
	GUI_SetColor(GUI_LIGHTGRAY);
	//GUI_DrawGradientRoundedH(10, 30, 310, 210, 15, 0x00838383,0x00D3D3D3);
	GUI_AA_SetFactor(15);
	GUI_AA_FillRoundedRect(10, 30, 310, 210, 15);
	GUI_SetColor(GUI_BLACK);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_DispStringAt("Sensors is connected:", 40, 40);
	GUI_DispStringAt("ID:1001                        ID:1004", 40, 80);//24个空格
	GUI_DispStringAt("ID:1002                        ID:1005", 40, 120);//24个空格
	GUI_DispStringAt("ID:1003                        ID:1006", 40, 160);//24个空格
}


void Sensors_page(void)
{
	if(tim_flg==1)
	{
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		
		GUI_SetColor(GUI_WHITE);
		GUI_SetPenSize(2);
		//GUI_SetFont(GUI_FONT_20_ASCII);
		GUI_SetFont(&GUI_FontFontSong);
		GUI_DispStringAt(str_Landzo,3, 3);
		//
		GUI_DispStringAt(str_next,5,219);
		GUI_DispStringAt(str_back, 90, 219);
		GUI_DispStringAt(str_stop, 185, 219); 
		GUI_DispStringAt(str_begin, 250, 219);
		GUI_DrawPolygon(OutPoints, 8, 270, 5);
		GUI_SetPenSize(1);
		GUI_SetLineStyle(GUI_LS_DASH);
		GUI_DrawLine(0, 25, 320, 25);
		GUI_DrawLine(0, 214, 320, 214);
		GUI_SetColor(GUI_LIGHTGRAY);

		//GUI_DrawGradientRoundedH(10, 30, 310, 210, 15, 0x00838383, 0x00D3D3D3);
		GUI_AA_SetFactor(10);
		GUI_AA_FillRoundedRect(10, 30, 310, 210, 15);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetColor(GUI_BLACK);
		tim_flg=0;
	}
	
	GUI_SetColor(GUI_WHITE);
	
	
	GUI_DrawGradientRoundedH(20, 33, 300, 56, 5, 0x00FF7070, 0x00FFB0B0);
	//GUI_DispStringAt("IR_Distance:", 25, 34);
	GUI_SetFont(&GUI_FontFontSong);
	GUI_DispStringAt(str_IR_Distance, 25, 34);
	GUI_SetFont(GUI_FONT_20_ASCII);
	IRdistance_struct ird = wireless->irdistance->get();
	GUI_GotoXY(25 + 120, 34);
	GUI_DispDec(ird.distance, 4);

	GUI_DrawGradientRoundedH(20, 58, 300, 81, 5, 0x00FF7070, 0x00FFB0B0);
	//GUI_DispStringAt("IR_Remote:", 25, 59);
	GUI_SetFont(&GUI_FontFontSong);
	GUI_DispStringAt(str_IR_Remote, 25, 59);
	GUI_SetFont(GUI_FONT_20_ASCII);
	IRremote_struct irr = wireless->irremote->get();
	GUI_GotoXY(25 + 120, 59);
	GUI_DispHex(irr.key, 8);

	GUI_DrawGradientRoundedH(20, 82 + 1, 300, 105 + 1, 5, 0x00FF7070, 0x00FFB0B0);
	//GUI_DispStringAt("PRS:", 25, 84);
	GUI_SetFont(&GUI_FontFontSong);
	GUI_DispStringAt(str_PRS, 25, 84);
	GUI_SetFont(GUI_FONT_20_ASCII);
	PRS_struct show_prs = wireless->prs->get();
	GUI_GotoXY(25 + 120, 84);
	GUI_DispDec(show_prs.analog, 4);

	GUI_DrawGradientRoundedH(20, 107 + 1, 300, 130 + 1, 5, 0x00FF7070, 0x00FFB0B0);
//	GUI_DispStringAt("DHT11:", 25, 109);
//	GUI_DispStringAt("Temp:", 100, 109);
//	GUI_DispStringAt("Humi:", 200, 109);
	GUI_SetFont(&GUI_FontFontSong);
	GUI_DispStringAt(str_Dht11, 25, 109);
	GUI_DispStringAt(str_Dht11_temp, 100, 109);
	GUI_DispStringAt(str_Dht11_humi, 200, 109);
	GUI_SetFont(GUI_FONT_20_ASCII);
	DHT11_struct show_dht11 = wireless->dht11->get();
	GUI_GotoXY(145, 109);
	GUI_DispDec(show_dht11.temperature, 2);
	GUI_GotoXY(242, 109);
	GUI_DispDec(show_dht11.humidity, 2);

	GUI_DrawGradientRoundedH(20, 132 + 1, 300, 155 + 1, 5, 0x00FF7070, 0x00FFB0B0);
	//GUI_DispStringAt("PIR:", 25, 134);
	GUI_SetFont(&GUI_FontFontSong);
	GUI_DispStringAt(str_PIR, 25, 134);
	GUI_SetFont(GUI_FONT_20_ASCII);
	PIR_struct show_pir = wireless->pir->get();
	GUI_GotoXY(25 + 120, 134);
	GUI_DispDec(show_pir.digital, 1);

	GUI_DrawGradientRoundedH(20, 157 + 1, 300, 180 + 1, 5, 0x00FF7070, 0x00FFB0B0);
	//GUI_DispStringAt("Potentiometer:", 25, 159);
	GUI_SetFont(&GUI_FontFontSong);
	GUI_DispStringAt(str_Potentiometer, 25, 159);
	GUI_SetFont(GUI_FONT_20_ASCII);
	Potentiometer_struct show_poten = wireless->potentiometer->get();
	GUI_GotoXY(25 + 120, 159);
	GUI_DispDec(show_poten.analog, 4);

	GUI_DrawGradientRoundedH(20, 182 + 1, 300, 205 + 1, 5, 0x00FF7070, 0x00FFB0B0);
	//GUI_DispStringAt("Large_Key:", 25, 184);
	GUI_SetFont(&GUI_FontFontSong);
	GUI_DispStringAt(str_bigkey, 25, 184);
	GUI_SetFont(GUI_FONT_20_ASCII);
	BigKey_struct show_bigkey = wireless->bigkey->get();
	GUI_GotoXY(25 + 120, 184);
	GUI_DispDec(show_bigkey.digital, 1);
	
}

void Voltage_Pros(void)
{
	u16 Voltage_dat=100;
	GUI_SetTextMode(GUI_TM_NORMAL);
	GUI_SetColor(GUI_GREEN);
	bat_Voltage=(u16)((Get_Voltage()-340)*1.34);//电池充满为4.15V LDO最小输入在3.4V
	if(bat_Voltage>0&&bat_Voltage<=100)
	{
		Voltage_dat=bat_Voltage;
	}
	if(bat_Voltage<=105){
		//if(bat_Voltage>)
		GUI_ClearRect(145,2,176,22);//清除充电图标内区域140，0
		if(Voltage_dat>0&&Voltage_dat<=25){//1格电
			GUI_SetColor(GUI_RED);
			GUI_ClearRect(283,7,308,18);//清除电池内区域
			GUI_FillPolygon(IN_Point1, 4, 270, 5);
			GUI_SetColor(GUI_GREEN);
			
		}
		if(Voltage_dat>25&&Voltage_dat<=50){//2格电
			GUI_ClearRect(283,7,308,18);;//清除电池内区域
			GUI_FillPolygon(IN_Point1, 4, 270, 5);
			GUI_FillPolygon(IN_Point2, 4, 270, 5);
		}
		if(Voltage_dat>50&&Voltage_dat<=75){//3格电
			GUI_ClearRect(283,7,308,18);;//清除电池内区域
			GUI_FillPolygon(IN_Point1, 4, 270, 5);
			GUI_FillPolygon(IN_Point2, 4, 270, 5);
			GUI_FillPolygon(IN_Point3, 4, 270, 5);

		}
		if(Voltage_dat>75&&Voltage_dat<=100){//4格电
			GUI_ClearRect(283,7,308,18);//清除电池内区域
			GUI_FillPolygon(IN_Point1, 4, 270, 5);
			GUI_FillPolygon(IN_Point2, 4, 270, 5);
			GUI_FillPolygon(IN_Point3, 4, 270, 5);
			GUI_FillPolygon(IN_Point4, 4, 270, 5);
		}
		
		GUI_ClearRect(230,5,275,23);//清除电压百分比区域
		GUI_SetColor(GUI_WHITE);
		GUI_DispStringAt("%", 260, 5);
		if(Voltage_dat==100){
			GUI_GotoXY(230, 5);
			GUI_DispDec(Voltage_dat, 3);//显示3位百分数
		}
		else
		{
			GUI_GotoXY(240, 5);
			GUI_DispDec(Voltage_dat, 2);//显示2位百分数
		}
	}
	else{//USB接入
		
		//GUI_DispStringAt("101%", 240, 5);
		GUI_SetColor(GUI_LIGHTYELLOW);
		GUI_FillPolygon(Point_USB, 14, 140, 0);
	}
	GUI_SetTextMode(GUI_TM_TRANS);
	
	
}

