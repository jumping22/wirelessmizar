/*
 * zproto_mac.h
 *
 *  Created on: Sep 28, 2014
 *      Author: toto
 */

#ifndef ZPROTO_MAC_H_
#define ZPROTO_MAC_H_

#include "zproto.h"

#pragma pack(1)

/* MAC Send status*/
#define SEND_ZIGBEE_BUF_FULL    0x21
#define SEND_UART_BUF_FULL      0x22
#define SEND_SPI_BUF_FULL       0x23
/* MAC Associate status*/
#define ASSO_PAN_NO_MATCH       0x24

typedef enum {
	MAC_TYPE_SCAN = 1,
	MAC_TYPE_JOIN,
	MAC_TYPE_AUTHENTICATE,
	MAC_TYPE_ASSOCIATE,
	MAC_TYPE_RESET,
	MAC_TYPE_START,
	MAC_TYPE_SEND,
} MacType;

typedef struct {
	uint8 type;
	uint16 code; // See mac_api.h 'mac status' and the top of this file 'Send status'
} MacBody;

#pragma pack()


MacBody* zproto_mac_body(ZProto *proto);

#endif /* ZPROTO_MAC_H_ */
