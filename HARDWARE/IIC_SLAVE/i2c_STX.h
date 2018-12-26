#ifndef __I2C_STX_H
#define __I2C_STX_H
#include "stdio.h"
#include <stdint.h>


typedef int (*i2c_recv_callback)(uint8_t);

#define I2Cx_RCC				RCC_APB1Periph_I2C1
#define I2Cx						I2C1
#define I2C_GPIO_RCC		RCC_APB2Periph_GPIOB
#define I2C_GPIO				GPIOB
#define I2C_PIN_SDA			GPIO_Pin_7
#define I2C_PIN_SCL			GPIO_Pin_6
#define I2Cx_EV_IRQn	  I2C1_EV_IRQn

#define I2CSLAVE_ADDR			0x20

/*******************************************************************/
#define I2C_CLOCK_FRQ   				100000    // I2C-Frq in Hz (100 kHz)
#define I2C_MODE_WAITING				0    // Waiting for commands
#define I2C_MODE_SLAVE_ADR_WR		1	 // Received slave address (writing)
#define I2C_MODE_ADR_BYTE				2    // Received ADR byte
#define I2C_MODE_DATA_BYTE_WR		3    // Data byte (writing)
#define I2C_MODE_SLAVE_ADR_RD		4 	 // Received slave address (to read)
#define I2C_MODE_DATA_BYTE_RD		5    // Data byte (to read)
/*******************************************************************/

#define I2C1_SEND_BUF_SIZE			20


void I2C1_Slave_init(void);
void I2C1_SetRecvCallback(i2c_recv_callback cb);
void I2C1_SetSendBuf(uint8_t *sendbuf, uint8_t length);
	
#endif
