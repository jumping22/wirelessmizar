#ifndef __MICROBIT_IIC_H
#define __MICROBIT_IIC_H
#include <stdint.h>


#define MB_RECV_BUF_SIZE		120
#define MB_HEAD		0xa4

extern unsigned char ShowSensorData_flg;
void microbit_recv_analyze(uint8_t recvdata);

#endif


