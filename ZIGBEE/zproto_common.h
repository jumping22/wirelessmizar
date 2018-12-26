/*
 * zproto_common.h
 *
 *  Created on: Sep 26, 2014
 *      Author: toto
 */

#ifndef ZPROTO_COMMON_H_
#define ZPROTO_COMMON_H_

#include "zproto.h"

#pragma pack(1)

#define ZP_CONFIRM_FSY_ROUTER				FOURCC('C', 'F', 'F', 'R')

typedef struct {
	uint16 code;
} ConfirmBody;


typedef struct {
	uint8 routerType;  // see the enum 'FSYRouterType' in file 'zproto_config.h' 
} FSYConfirmParam;

#pragma pack()

#endif /* ZPROTO_COMMON_H_ */
