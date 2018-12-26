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

/****************************************Copyright (c)****************************************************
**--------------------------------------------------------------------------------------------------------
** Created by		WUDONG SHANG
** Modified by:    ENJIE DU	&	JUMPING WANG
** Modified date:        2018-12-25 CHRISTMAS DAY	- ������  
** Version:               V0.3
** Descriptions:          2D��ͼ���  
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
static uint32_t timer1 = 0;
GUI_RECT Rect = { 0, 0, 200, 200 };
unsigned char nextpag_flg = 0;
u16 bat_Voltage=0;
unsigned char tim_flg=0;
uint8_t user_gui_interface = 0;	//�û������̱�־

int main(void)
{
	delay_init(72);	     //��ʱ��ʼ��
	NVIC_Configuration();
	INTX_DISABLE();//�����ж�
	PWR_5182CTL_Init();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	Adc_Init();
	//TIM3_PWM_Init();//��Ļ����
	wireless = WirelessDevice_Init(uart_send);
	uart1_SetRecvCallback(wireless->analyze);
	I2C1_SetRecvCallback(microbit_recv_analyze);
	multi_button_init();
	uart_init(115200);
	I2C1_Slave_init();
	
#if defined(USING_LCD)
	TIM3_Int_Init(999,71);	//1KHZ ��ʱ��1ms
	#if defined(USING_ILI9341_8BIT)
		TFTLCD_Init();
	#elif defined(USING_ILI9341_SPI)
		LCD_Init();
	#endif
	GUI_Init();  
#endif
	wireless->Router->router_init();
	INTX_ENABLE();//�����ж�
	GPIO_SetBits(GPIOB,GPIO_Pin_10);//��5182��Դ		
	
	
	while(1) {
#if defined(USING_LCD)
		GUI_Delay(10);
#endif
		timer1++;
		multi_button_event();
		if (user_gui_interface == 0) {
			if(timer1 % 200 == 0)//2Sˢһ��
			{
				Voltage_Pros();
			}
			if(nextpag_flg ==1)
			{
					//nextpag_flg=2;
				if (timer1 % 100 == 0) { //1s
					//if(ShowSensorData_flg==1){
							Sensors_page();
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
			}
			else if(nextpag_flg==0)
			{
				nextpag_flg=2;
				//GUI_Clear();
				home_page();
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
	GUI_SetColor(GUI_BLUE);	 
	if(btn_event_val != get_button_event(&btn1)) {
		//btn_event_val = get_button_event(&btn1);
		btn1_event = get_button_event(&btn1);
		switch((uint8_t)btn1_event)
		{
			case NONE_PRESS:
				//GUI_DispStringAtCEOL("None press!",10,30);
				break;
			case SINGLE_CLICK:
				nextpag_flg = 1;
				tim_flg=1;
				GUI_Clear();
			break;//GUI_DispStringAt("key1_press!",15,120) 
			case PRESS_UP: break;
			default: break;			
//			case PRESS_REPEAT:break; 
//			case SINGLE_CLICK:
//				GUI_DispStringAtCEOL("Single click!",10,30);
//				break; 
//			case DOUBLE_CLICK:
//				GUI_DispStringAtCEOL("Doublee click!",10,30);
//				break; 
//			case LONG_RRESS_START:
//				GUI_DispStringAtCEOL("Long press start!",10,30);
//				break; 
//			case LONG_PRESS_HOLD:
//				GUI_DispStringAtCEOL("Long press hold!",10,30);
//				break; 
		}
	}
	
		if(btn_event_val != get_button_event(&btn2)) {
		btn2_event = get_button_event(&btn2);
		switch((uint8_t)btn2_event)
		{
			case NONE_PRESS:break;
			case SINGLE_CLICK:
				nextpag_flg = 0;
				GUI_Clear();
			break; //GUI_DispStringAt("key2_press!",15,120);
			case PRESS_UP: break;
			default: break;			
		}
	}
	  if(btn_event_val != get_button_event(&btn3)) {
		btn3_event = get_button_event(&btn3);
		switch((uint8_t)btn3_event)
		{
			case NONE_PRESS:break;
			case PRESS_DOWN:GUI_DispStringAt("key3_press!", 220, 120);break; 
			case PRESS_UP: break;
			default: break;			
		}
	}
		if(btn_event_val != get_button_event(&btn4)) {
		btn4_event = get_button_event(&btn4);
		switch((uint8_t)btn4_event)
		{
			case NONE_PRESS:break;
			case PRESS_DOWN:GUI_DispStringAt("key4_press!", 220, 120);break; 
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//����TIM3ʱ�� 

	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;   //��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	   //����ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=arr;		   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //����ʱ�ӷָ�
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��������ж�

	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3,ENABLE);		 //ʹ��TIM3
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

	GUI_SetFont(GUI_FONT_20_ASCII);
	GUI_DispStringAt("HomePage",3, 3);
	GUI_DispStringAt("NEXT",5,219);
	GUI_DispStringAt("BACK", 90, 219);
	GUI_DispStringAt("OK", 185, 219); 
	GUI_DispStringAt("CANCEL", 250, 219);
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
	GUI_DispStringAt("ID:1001                        ID:1004", 40, 80);//24���ո�
	GUI_DispStringAt("ID:1002                        ID:1005", 40, 120);//24���ո�
	GUI_DispStringAt("ID:1003                        ID:1006", 40, 160);//24���ո�
}

void Sensors_page(void)
{
	if(tim_flg==1)
	{
		GUI_SetColor(GUI_WHITE);
		GUI_SetPenSize(2);
		GUI_SetFont(GUI_FONT_20_ASCII);
		GUI_DispStringAt("Sensors",3, 3);
		//
		GUI_DispStringAt("NEXT",5,219);
		GUI_DispStringAt("BACK", 90, 219);
		GUI_DispStringAt("OK", 185, 219); 
		GUI_DispStringAt("CANCEL", 250, 219);
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
	GUI_DispStringAt("IR_Distance:", 25, 34);
	IRdistance_struct ird = wireless->irdistance->get();
	GUI_GotoXY(25 + 120, 34);
	GUI_DispDec(ird.distance, 4);

	GUI_DrawGradientRoundedH(20, 58, 300, 81, 5, 0x00FF7070, 0x00FFB0B0);
	GUI_DispStringAt("IR_Remote:", 25, 59);
	IRremote_struct irr = wireless->irremote->get();
	GUI_GotoXY(25 + 120, 59);
	GUI_DispHex(irr.key, 8);

	GUI_DrawGradientRoundedH(20, 82 + 1, 300, 105 + 1, 5, 0x00FF7070, 0x00FFB0B0);
	GUI_DispStringAt("PRS:", 25, 84);
	PRS_struct show_prs = wireless->prs->get();
	GUI_GotoXY(25 + 120, 84);
	GUI_DispDec(show_prs.analog, 4);

	GUI_DrawGradientRoundedH(20, 107 + 1, 300, 130 + 1, 5, 0x00FF7070, 0x00FFB0B0);
	GUI_DispStringAt("DHT11:", 25, 109);
	GUI_DispStringAt("Temp:", 100, 109);
	GUI_DispStringAt("Humi:", 200, 109);
	DHT11_struct show_dht11 = wireless->dht11->get();
	GUI_GotoXY(155, 109);
	GUI_DispDec(show_dht11.temperature, 2);
	GUI_GotoXY(250, 109);
	GUI_DispDec(show_dht11.humidity, 2);

	GUI_DrawGradientRoundedH(20, 132 + 1, 300, 155 + 1, 5, 0x00FF7070, 0x00FFB0B0);
	GUI_DispStringAt("PIR:", 25, 134);
	PIR_struct show_pir = wireless->pir->get();
	GUI_GotoXY(25 + 120, 134);
	GUI_DispDec(show_pir.digital, 1);

	GUI_DrawGradientRoundedH(20, 157 + 1, 300, 180 + 1, 5, 0x00FF7070, 0x00FFB0B0);
	GUI_DispStringAt("Potentiometer:", 25, 159);
	Potentiometer_struct show_poten = wireless->potentiometer->get();
	GUI_GotoXY(25 + 120, 159);
	GUI_DispDec(show_poten.analog, 4);

	GUI_DrawGradientRoundedH(20, 182 + 1, 300, 205 + 1, 5, 0x00FF7070, 0x00FFB0B0);
	GUI_DispStringAt("Large_Key:", 25, 184);
	BigKey_struct show_bigkey = wireless->bigkey->get();
	GUI_GotoXY(25 + 120, 184);
	GUI_DispDec(show_bigkey.digital, 1);
}

void Voltage_Pros(void)
{
	u16 Voltage_dat=100;
	GUI_SetTextMode(GUI_TM_NORMAL);
	GUI_SetColor(GUI_GREEN);
	bat_Voltage=(u16)((Get_Voltage()-340)*1.34);//��س���Ϊ4.15V LDO��С������3.4V
	if(bat_Voltage>0&&bat_Voltage<=100)
	{
		Voltage_dat=bat_Voltage;
	}
	if(bat_Voltage<=105){
		//if(bat_Voltage>)
		GUI_ClearRect(145,2,176,22);//������ͼ��������140��0
		if(Voltage_dat>0&&Voltage_dat<=25){//1���
			GUI_SetColor(GUI_RED);
			GUI_ClearRect(283,7,308,18);//������������
			GUI_FillPolygon(IN_Point1, 4, 270, 5);
			GUI_SetColor(GUI_GREEN);
			
		}
		if(Voltage_dat>25&&Voltage_dat<=50){//2���
			GUI_ClearRect(283,7,308,18);;//������������
			GUI_FillPolygon(IN_Point1, 4, 270, 5);
			GUI_FillPolygon(IN_Point2, 4, 270, 5);
		}
		if(Voltage_dat>50&&Voltage_dat<=75){//3���
			GUI_ClearRect(283,7,308,18);;//������������
			GUI_FillPolygon(IN_Point1, 4, 270, 5);
			GUI_FillPolygon(IN_Point2, 4, 270, 5);
			GUI_FillPolygon(IN_Point3, 4, 270, 5);

		}
		if(Voltage_dat>75&&Voltage_dat<=100){//4���
			GUI_ClearRect(283,7,308,18);//������������
			GUI_FillPolygon(IN_Point1, 4, 270, 5);
			GUI_FillPolygon(IN_Point2, 4, 270, 5);
			GUI_FillPolygon(IN_Point3, 4, 270, 5);
			GUI_FillPolygon(IN_Point4, 4, 270, 5);
		}
		
		GUI_ClearRect(230,5,275,23);//�����ѹ�ٷֱ�����
		GUI_SetColor(GUI_WHITE);
		GUI_DispStringAt("%", 260, 5);
		if(Voltage_dat==100){
			GUI_GotoXY(230, 5);
			GUI_DispDec(Voltage_dat, 3);//��ʾ3λ�ٷ���
		}
		else
		{
			GUI_GotoXY(240, 5);
			GUI_DispDec(Voltage_dat, 2);//��ʾ2λ�ٷ���
		}
	}
	else{//USB����
		
		//GUI_DispStringAt("101%", 240, 5);
		GUI_SetColor(GUI_LIGHTYELLOW);
		GUI_FillPolygon(Point_USB, 14, 140, 0);
	}
	GUI_SetTextMode(GUI_TM_TRANS);
	
	
}

