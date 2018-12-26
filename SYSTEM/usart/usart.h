#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include <stdint.h>


#define USART_RX_BUF_SIZE				128

typedef int (*recv_callback)(uint8_t *, uint16_t);

extern uint8_t USART_RX_BUF[USART_RX_BUF_SIZE];     //���ջ���,���63���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint8_t USART_RX_STA;         //����״̬���	

void uart_init(uint32_t bound);
void uart1_SetRecvCallback(recv_callback cb);
void uart_send(uint8_t *buf, uint16_t size);
	
#endif
