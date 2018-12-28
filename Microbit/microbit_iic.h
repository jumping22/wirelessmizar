#ifndef __MICROBIT_IIC_H
#define __MICROBIT_IIC_H
#include <stdint.h>


#define MB_RECV_BUF_SIZE		120
#define MB_HEAD		0xa4



//extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong;

extern const char str_Landzo[] ;
extern const char str_IR_Distance[] ;
extern const char str_PRS[] ;
extern const char str_Dht11[] ;
extern const char str_Dht11_temp[] ;
extern const char str_Dht11_humi[] ;
extern const char str_PIR[]  ;
extern const char str_bigkey[] ;
extern const char str_Potentiometer[] ;
extern const char str_IR_Remote[] ;
extern const char str_stop[] ;
extern const char str_back[] ;
extern const char str_next[] ;
extern const char str_begin[] ;



void microbit_recv_analyze(uint8_t recvdata);

#endif


