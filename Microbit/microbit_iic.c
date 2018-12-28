#include "microbit_iic.h" 
#include "delay.h"
#include "i2c_STX.h"
#include "EndDevice.h"
#include "multi_button.h"
#include "GUI.h"


/**************************    GUI define   *********************************/
static void Router_Control(uint8_t ocmd, uint8_t *data, uint8_t length);
//void myGUI_SetOptions(uint8_t cmd, uint8_t *data, uint8_t length);		//GUI 设置要显示的功能项
void GUI_Control(uint8_t ocmd, uint8_t *data, uint8_t length);
void GUI_ShowSensorData(unsigned char num_Sensor);	//GUI中显示所有传感器数据
void GUI_TextOptions(uint8_t ocmd, uint8_t *data, uint8_t length);	//GUI 文字功能
void GUI_NumberOptions(uint8_t ocmd, uint8_t *data, uint8_t length);	//GUI 数字功能
void GUI_2DGraphicsOptions(uint8_t ocmd, uint8_t *data, uint8_t length);	//GUI 2D图形功能

void Show_IR_Distance(void);
void Show_IR_Remote(void);
void Show_PRS(void);
void Show_DHT11(void);
void Show_PIR(void);
void Show_Potentiometer(void);
void Show_Large_Key(void);

unsigned char ShowSensorData_flg=0;
extern uint8_t user_gui_interface;



static void microbit_sensor_deal(void);


extern WirelessDevice_class *wireless;

extern PressEvent btn1_event;
extern PressEvent btn2_event;
extern PressEvent btn3_event;
extern PressEvent btn4_event;

extern uint32_t timer1 ;
extern uint8_t nextpag_flg ;

static uint8_t mb_recv_state = 0;
static uint8_t mb_recv_buf[MB_RECV_BUF_SIZE];
static uint8_t mb_recv_buf_length = 0;
static uint8_t mb_recv_buf_index = 0;

void microbit_recv_analyze(uint8_t recvdata)
{
	switch(mb_recv_state) {
		case 0: {
			if (recvdata == MB_HEAD) {
				mb_recv_state = 1;
				mb_recv_buf_index = 0;
				mb_recv_buf_length = 0;
			} else {
				mb_recv_state = 0;
			}
		}break;
		
		case 1: {
			mb_recv_buf_length = recvdata;
			mb_recv_state = 2;
		}break;
		
		case 2: {
			mb_recv_buf[mb_recv_buf_index++] = recvdata;
			if (mb_recv_buf_index >= mb_recv_buf_length) {
				mb_recv_state = 3;
			}
		}break;
		
		case 3: {
			if (recvdata == 0xff) {
				microbit_sensor_deal();
				mb_recv_state = 0;
			}
		}break;
	}
}
 

static void microbit_sensor_deal(void)
{
	uint8_t *buf = mb_recv_buf;
	uint8_t length = mb_recv_buf_index;
	
//	if (buf[0] != 0xa4) {
//		return;
//	}
	switch(buf[0]) {	//cmd
		case 0xff : { //路由相关
			uint8_t ocmd = buf[1];
			uint8_t *data = &buf[2];
			uint8_t dlength = length - 1;
			Router_Control(ocmd, data, dlength);
		}break;
		
		case 0: {	//4个按键值
			uint8_t sendbuf[8] = {0xa5, 0x04, btn1_event, btn2_event, btn3_event, btn4_event, 0xff, 0};
			I2C1_SetSendBuf(sendbuf, 7);
		}break;
		
		case 1: {	//红外测距
			uint8_t sendbuf[6] = {0xa5, 0x02, 0, 0, 0xff, 0};
			IRdistance_struct ird = wireless->irdistance->get();
			sendbuf[2] = ird.buf[0];
			sendbuf[3] = ird.buf[1];
			I2C1_SetSendBuf(sendbuf, 5);
		}break;
		
		case 2: {	//红外遥控器
			uint8_t sendbuf[8] = {0xa5, 0x04, 0, 0, 0, 0, 0xff, 0};
			IRremote_struct irr = wireless->irremote->get();
			sendbuf[2] = irr.buf[0];
			sendbuf[3] = irr.buf[1];
			sendbuf[4] = irr.buf[2];
			sendbuf[5] = irr.buf[3];
			I2C1_SetSendBuf(sendbuf, 7);
		}break;
		
		case 3: {	//光敏电阻
			uint8_t sendbuf[6] = {0xa5, 0x02, 0, 0, 0xff, 0};
			PRS_struct prs = wireless->prs->get();
			sendbuf[2] = prs.buf[0];
			sendbuf[3] = prs.buf[1];
			I2C1_SetSendBuf(sendbuf, 5);
		}break;
		
		case 4: {	//温湿度
			uint8_t sendbuf[8] = {0xa5, 0x04, 0, 0, 0, 0, 0xff, 0};
			DHT11_struct dht = wireless->dht11->get();
			sendbuf[2] = dht.buf[0];
			sendbuf[3] = dht.buf[1];
			sendbuf[4] = dht.buf[2];
			sendbuf[5] = dht.buf[3];
			I2C1_SetSendBuf(sendbuf, 7);
		}break;
		
		case 5: {	//人体红外
			uint8_t sendbuf[5] = {0xa5, 0x02, 0, 0xff, 0};
			PIR_struct pir = wireless->pir->get();
			//sendbuf[2] = pir.buf[1];				//两个字节的数字量 取第二个有效字节
			sendbuf[2] = pir.digital;
			I2C1_SetSendBuf(sendbuf, 4);
		}break;
		
		case 6: {	//电位器
			uint8_t sendbuf[6] = {0xa5, 0x02, 0, 0, 0xff, 0};
			Potentiometer_struct rtt = wireless->potentiometer->get();
			sendbuf[2] = rtt.buf[0];
			sendbuf[3] = rtt.buf[1];
			I2C1_SetSendBuf(sendbuf, 5);
		}break;
		
		case 7: {	//大按键
			uint8_t sendbuf[5] = {0xa5, 0x02, 0, 0xff, 0};
			BigKey_struct key = wireless->bigkey->get();
			sendbuf[2] = key.digital;				//两个字节的数字量 取第二个有效字节
			I2C1_SetSendBuf(sendbuf, 4);
		}break;
		
		case 8: {	//GUI 设置
			uint8_t ocmd = buf[1];
			uint8_t *data = &buf[2];
			uint8_t dlength = length - 1;
			GUI_Control(ocmd, data, dlength);
		}break;
		
		case 9: {	//GUI 文字功能
			uint8_t ocmd = buf[1];
			uint8_t *data = &buf[2];
			uint8_t dlength = length - 1;
			GUI_TextOptions(ocmd, data, dlength);
		}break;
		
		case 10: {	//GUI 显示数字
			uint8_t ocmd = buf[1];
			uint8_t *data = &buf[2];
			uint8_t dlength = length - 1;
			GUI_NumberOptions(ocmd, data, dlength);
			GUI_DispStringAt("wqrrdewe",80,80);
		}break;
		
		case 11: {	//GUI 2D图形功能
			uint8_t ocmd = buf[1];
			uint8_t *data = &buf[2];
			uint8_t dlength = length - 1;
			GUI_2DGraphicsOptions(ocmd, data, dlength);
		}break;
		
	}
}


extern WirelessDevice_class *wireless;
static void Router_Control(uint8_t ocmd, uint8_t *data, uint8_t length)
{
	switch (ocmd) {
		case 0: {	//查询 路由信息
			wireless->Router->router_init();
			
			ZProtoRouterInfo info = wireless->Router->router_getInfo();
			uint8_t sendbuf[9] = {0xa5, 0x05, 0,0,0,0,0, 0xff, 0};
			sendbuf[2] = (uint8_t)(info.id>>8);
			sendbuf[3] = (uint8_t)(info.id);
			sendbuf[4] = (uint8_t)(info.panid>>8);
			sendbuf[5] = (uint8_t)(info.panid);
			sendbuf[6] = (uint8_t)info.channel;
			I2C1_SetSendBuf(sendbuf, 9);
		}break;
		
		case 1: { //设置 路由信息
			uint16_t id = *(data);
			uint16_t panid = *(data+2);
			uint8_t channel = *(data+4);
			
			ZProtoRouterInfo info = wireless->Router->router_getInfo();
			
			info.id = id;
			info.panid = panid;
			info.channel = channel;
			wireless->Router->router_setInfo(info);
		}break;
		
		case 2: { //设置 节点信息
			uint16_t panid = *(data);
			uint8_t channel = *(data+2);
			
			wireless->Router->enddevice_setInfo(panid, channel);
		}break;
	}
}


static const uint32_t userColor_table[] = {
	0,
	GUI_BLUE,
	GUI_GREEN,
	GUI_RED,           
  GUI_CYAN,          
  GUI_MAGENTA,       
  GUI_YELLOW,        
  GUI_LIGHTBLUE,     
  GUI_LIGHTGREEN,    
  GUI_LIGHTRED,      
  GUI_LIGHTCYAN,     
  GUI_LIGHTMAGENTA,  
  GUI_LIGHTYELLOW,   
  GUI_DARKBLUE,      
  GUI_DARKGREEN,     
  GUI_DARKRED,       
  GUI_DARKCYAN,      
  GUI_DARKMAGENTA,   
  GUI_DARKYELLOW,    
  GUI_WHITE,         
  GUI_LIGHTGRAY,     
  GUI_GRAY,          
  GUI_DARKGRAY,      
  GUI_BLACK,         
  GUI_BROWN,         
  GUI_ORANGE,        
  GUI_TRANSPARENT,   	
};

static const GUI_FONT GUI_UNI_PTR * userSize_table[]={
	0,
  GUI_FONT_6X8,
	GUI_FONT_8X18,  
	GUI_FONT_24_ASCII,
	GUI_FONT_32_ASCII,
};

static const unsigned usertextmod_table[]={
	0,
	GUI_TM_NORMAL,	
	GUI_TM_REV,
	GUI_TM_TRANS,
	GUI_TM_XOR,
};
static const unsigned usertextsty_table[]={
	0,
	GUI_TS_NORMAL,
	GUI_TS_UNDERLINE,
	GUI_TS_STRIKETHRU,
	GUI_TS_OVERLINE,
};

/**************************    GUI define   *********************************/
void GUI_ShowSensorData(unsigned char num_Sensor)
{
	switch(num_Sensor)
	{
		case 1:Show_IR_Distance();break;
		case 2:Show_IR_Remote();break;
		case 3:Show_PRS();break;
		case 4:Show_DHT11();break;
		case 5:Show_PIR();break;
		case 6:Show_Potentiometer();break;
		case 7:Show_Large_Key();break;
		//case 7:Show_IR_Remote();break;
	}
	if (num_Sensor == 0) {
		Show_IR_Distance();
		Show_IR_Remote();
		Show_PRS();
		Show_DHT11();
		Show_PIR();
		Show_Potentiometer();
		Show_Large_Key();
	}
}

void GUI_Control(uint8_t ocmd, uint8_t *data, uint8_t length)
{
	switch (ocmd) {
		case 0: {	//官方 demo
			GUI_ShowSensorData(data[0]);
		}break;
		
		case 1: {	//开关用户界面编程
			user_gui_interface = data[0];
			if (user_gui_interface == 1) {	//开启
				GUI_Clear();
			}
			else{
				timer1 = 0;
				nextpag_flg = 0;
			}
		}break;
	}
}


void GUI_TextOptions(uint8_t ocmd, uint8_t *data, uint8_t length)
{
	switch (ocmd) {
		case 0: {	//指定坐标显示文字
			uint16_t x = (uint16_t)(data[0]<<8) + data[1];
			uint16_t y = (uint16_t)(data[2]<<8) + data[3];
			char *text = (char*)(&data[0]);
			GUI_DispStringAtCEOL(text, x, y);
		}break;
		
		case 1: {	//文字颜色
			uint8_t color = data[0];
			GUI_SetColor(userColor_table[color]);
			GUI_Clear();
			//GUI_DispString("landzo! ");
		}break;
		
		case 2: {	//文字字体大小
			uint8_t size = data[0];
			GUI_SetFont(userSize_table[size]);
			//GUI_DispString("landzo! ");
		}break;
		
		case 3: {	//文字显示模式
			uint8_t text_mod = data[0];
			GUI_SetTextMode(usertextmod_table[text_mod]);
			//GUI_DispStringAt("wqrrdewe",20,20);
		}break;
		
		case 4: {	//文本样式
			uint8_t text_sty = data[0];
			GUI_SetTextStyle(usertextsty_table[text_sty]);
			//GUI_DispStringAt("wqrrdewe",20,20);
		}break;
	}
}

void GUI_NumberOptions(uint8_t ocmd, uint8_t *data, uint8_t length)
{
	uint8_t jz = (uint8_t)(data[0]);
	uint16_t x = (uint16_t)(data[1]<<8) + (data[2]);
	uint16_t y = (uint16_t)(data[3]<<8) + (data[4]);
	uint32_t num = (uint32_t)(data[5]<<24) + (uint32_t)(data[6]<<16) + (uint32_t)(data[7]<<8) + (uint32_t)(data[8]);
	switch (jz) {
		case 0: {}break;//指定位置显示 10/2/16进制数字 最大4个字节
		case 1:{ 
			GUI_DispDecAt(num, x, y, 5);
		}break;
		case 2:{ 
			GUI_DispBinAt(num, x, y, 5);
		}break;
		case 3:{ 
			GUI_DispHexAt(num, x, y, 5);
		}break;
	}
}

static const unsigned drawmodesty_table[]={
	0,
	GUI_DM_NORMAL,
	GUI_DM_XOR,
};


static const unsigned drawlinesty_table[]={
	GUI_LS_SOLID ,
	GUI_LS_DASH ,
	GUI_LS_DOT,
	GUI_LS_DASHDOT,
	GUI_LS_DASHDOTDOT,
};


void GUI_2DGraphicsOptions(uint8_t ocmd, uint8_t *data, uint8_t length)
{
	//待补充
	switch (ocmd) {
		case 0: {	//指定坐标显示文字
			
		}break;
		
		case 1: {	//设置背景颜色
			uint8_t color = data[0];
			GUI_SetBkColor(userColor_table[color]);
		}break;
		
		case 2: {	//清屏
			GUI_Clear();
		}break;
		
		case 3: {	//设置绘制模式
			uint8_t draw_mode = data[0];
			GUI_SetDrawMode(drawmodesty_table[draw_mode]);
		}break;
		
		case 4: {	//设置画笔大小
			GUI_SetPenSize(data[0]);
		}break;
		
		case 5: {	//绘制矩形
			switch (data[0]){
				case 0:{
					uint8_t color0 = data[5];
					uint8_t color1 = data[6];
					GUI_DrawGradientH(data[1],data[2],data[3],data[4],userColor_table[color0],userColor_table[color1]);
				}
				
				case 1:{
					uint8_t color0 = data[5];
					uint8_t color1 = data[6];
					GUI_DrawGradientV(data[1],data[2],data[3],data[4],userColor_table[color0],userColor_table[color1]);
				}
				
				case 2:{
					uint8_t color0 = data[6];
					uint8_t color1 = data[7];
					GUI_DrawGradientRoundedH(data[1],data[2],data[3],data[4],data[5],userColor_table[color0],userColor_table[color1]);
				}
				
				case 3:{
					uint8_t color0 = data[6];
					uint8_t color1 = data[7];
					GUI_DrawGradientRoundedV(data[1],data[2],data[3],data[4],data[5],userColor_table[color0],userColor_table[color1]);
				}
				
			}
		}break;
		
		case 6: {	//画线
			uint8_t line_sty = data[0];
			GUI_SetLineStyle(drawlinesty_table[line_sty]);
			GUI_DrawLine(data[1],data[2],data[3],data[4]);
		}break;
		
		case 7: {	//画圆
			if(data[0])//实心
			{
				GUI_FillCircle(data[1],data[2],data[3]);
			}
			else{//空心
				GUI_DrawCircle(data[1],data[2],data[3]);
			}
		}break;
		
		case 8: {	//画椭圆
			if(data[0])//实心
			{
				GUI_FillEllipse(data[1],data[2],data[3],data[4]);
			}
			else{//空心
				GUI_DrawEllipse(data[1],data[2],data[3],data[4]);
			}
		}break;
		
		case 9: {	//画弧线
			GUI_DrawArc(data[0],data[1],data[2],data[3],data[4],data[5]);
		}break;
		
		case 10: {	//画饼图
			GUI_DrawPie(data[0],data[1],data[2],data[3],data[4],0);
		}break;
		
		default : break;
	
	}
}

void Show_IR_Distance(void)
{
	GUI_RECT Rect = { 10, 10, 102, 75 };
	GUI_SetColor(GUI_LIGHTBLUE);
	GUI_FillRectEx(&Rect);
	GUI_SetColor(GUI_RED);
	//GUI_DispStringInRectWrap("IR_Distance", &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	GUI_DispStringInRectWrap(str_Landzo, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	Rect.x0 += 20;
	Rect.y0 += 20;
	//GUI_DispStringInRectWrap("somebody!!!", &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	GUI_GotoXY(Rect.x0,Rect.y0);
	IRdistance_struct ird = wireless->irdistance->get();
	GUI_DispDec(ird.distance, 5);
	Rect.x0 -= 20;
	Rect.y0 -= 20;
}

void Show_IR_Remote(void)
{
	GUI_RECT Rect = { (10+103), 10, (102+103), 75 };
	GUI_SetColor(GUI_LIGHTBLUE);
	GUI_FillRectEx(&Rect);
	GUI_SetColor(GUI_RED);
	GUI_DispStringInRectWrap(str_IR_Remote, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	Rect.x0 += 20;
	Rect.y0 += 20;
	//GUI_DispStringInRectWrap("somebody!!!", &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	GUI_GotoXY(Rect.x0, Rect.y0);
	IRremote_struct irr = wireless->irremote->get();
	GUI_DispHex(irr.key, 8);
	Rect.x0 -= 20;
	Rect.y0 -= 20;
}
void Show_PRS(void)
{
	GUI_RECT Rect = { (10 + 103+103), 10, (102 + 103+103), 75 };
	GUI_SetColor(GUI_LIGHTBLUE);
	GUI_FillRectEx(&Rect);
	GUI_SetColor(GUI_RED);
	GUI_DispStringInRectWrap(str_PRS, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	Rect.x0 += 20;
	Rect.y0 += 20;
	//GUI_DispStringInRectWrap("somebody!!!", &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	GUI_GotoXY(Rect.x0, Rect.y0);
	PRS_struct show_prs = wireless->prs->get();
	GUI_DispDec(show_prs.analog, 5);
	Rect.x0 -= 20;
	Rect.y0 -= 20;
}
void Show_DHT11(void)
{
	GUI_RECT Rect = { 10, (75 + 10), 102, (75 + 10 + 66) };
	unsigned char i = 0;
	for (i = 0; i < 3; i++)
	{
		GUI_SetColor(GUI_LIGHTYELLOW);
		GUI_FillRectEx(&Rect);
		GUI_SetColor(GUI_MAGENTA);
		Rect.x0 += 20;
		if (i == 0){
			GUI_DispStringInRectWrap(str_Landzo, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
		}
		else if (i == 1){
			
			DHT11_struct show_dht11 = wireless->dht11->get();
			GUI_DispStringInRectWrap(str_Dht11, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
			Rect.x0 -= 20;
			Rect.y0 += 20;
			GUI_DispStringInRectWrap(str_Dht11_temp, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);

			GUI_GotoXY((Rect.x0+45), Rect.y0);
			GUI_DispDec(show_dht11.temperature, 2);
			//block5_x1 = Rect.x0;//
			//block5_y1 = Rect.y0;
			Rect.y0 += 20;
			GUI_DispStringInRectWrap(str_Dht11_humi, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
			GUI_GotoXY((Rect.x0 + 45), Rect.y0);
			GUI_DispDec(show_dht11.humidity, 2);
			//block5_x2 = Rect.x0;//
			//block5_y2 = Rect.y0;
			Rect.x0 += 20;
			Rect.y0 -= 40;

		}
		else{
			GUI_DispStringInRectWrap(str_Landzo, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
		}
		Rect.x0 -= 20;

		Rect.x0 += (10 + 93);
		Rect.x1 += (10 + 93);
	}
}
void Show_PIR(void)
{
	GUI_RECT Rect = { 10, (75 + 10 + 66 + 10), 102, (75 + 10 + 66 + 10 + 66) };
	GUI_SetColor(GUI_LIGHTRED);
	GUI_FillRectEx(&Rect);
	GUI_SetColor(GUI_GREEN);
	GUI_DispStringInRectWrap(str_PIR, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	Rect.x0 += 20;
	Rect.y0 += 20;
	//GUI_DispStringInRectWrap("somebody!!!", &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	GUI_GotoXY(Rect.x0, Rect.y0);
  PIR_struct show_pir = wireless->pir->get();
	GUI_DispDec(show_pir.digital, 1);
	Rect.x0 -= 20;
	Rect.y0 -= 20;
}

void Show_Potentiometer(void)
{
	GUI_RECT Rect = { (10 + 103), (75 + 10 + 66 + 10), (102+103), (75 + 10 + 66 + 10 + 66) };
	GUI_SetColor(GUI_LIGHTRED);
	GUI_FillRectEx(&Rect);
	GUI_SetColor(GUI_GREEN);
	GUI_DispStringInRectWrap(str_Potentiometer, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	Rect.x0 += 20;
	Rect.y0 += 20;
	//GUI_DispStringInRectWrap("somebody!!!", &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	GUI_GotoXY(Rect.x0, Rect.y0);
	Potentiometer_struct show_poten = wireless->potentiometer->get();
	GUI_DispDec(show_poten.analog, 5);
	Rect.x0 -= 20;
	Rect.y0 -= 20;
}

void Show_Large_Key(void)
{
	GUI_RECT Rect = { (10 + 103+103), (75 + 10 + 66 + 10), (102 + 103+103), (75 + 10 + 66 + 10 + 66) };
	GUI_SetColor(GUI_LIGHTRED);
	GUI_FillRectEx(&Rect);
	GUI_SetColor(GUI_GREEN);
	GUI_DispStringInRectWrap(str_bigkey, &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	Rect.x0 += 20;
	Rect.y0 += 20;
	//GUI_DispStringInRectWrap("somebody!!!", &Rect, GUI_TA_HORIZONTAL, GUI_WRAPMODE_WORD);
	GUI_GotoXY(Rect.x0, Rect.y0);
  BigKey_struct show_bigkey = wireless->bigkey->get();
	GUI_DispDec(show_bigkey.digital, 1);
	Rect.x0 -= 20;
	Rect.y0 -= 20;
}












