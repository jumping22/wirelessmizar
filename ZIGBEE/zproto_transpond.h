/*
 * zproto_transpond.h
 *
 *  Created on: Sep 29, 2014
 *      Author: toto
 */

#ifndef ZPROTO_TRANSPOND_H_
#define ZPROTO_TRANSPOND_H_

#include "zproto.h"

#pragma pack(1)

#define MAX_PATH_COUNT 5

typedef struct
{
	uint8 signature;
	uint8 type;
	uint8 length;
	uint16 srcId;
	uint16 destId;
	uint8 rssi;
	uint16 pathId[MAX_PATH_COUNT];
} ZTransHeader;

#pragma pack()

ZProto zproto_transpond_generate(void *data, uint8 size, ZProto *proto);

#endif /* ZPROTO_TRANSPOND_H_ */
