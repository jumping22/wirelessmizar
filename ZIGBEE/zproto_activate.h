/*
 * zproto_activate.h
 *
 *  Created on: Sep 28, 2014
 *      Author: toto
 */

#ifndef ZPROTO_ACTIVATE_H_
#define ZPROTO_ACTIVATE_H_

#include "zproto.h"

#pragma pack(1)

typedef enum {
	ACTIVATE_REQUEST = 1,
	ACTIVATE_RESPONSE,
	ACTIVATE_DONE,
	ACTIVITE_RSP_N_CONFIG
} ActivateType;

#define ZP_ACTIVATE_PARAM_ST 				FOURCC('A', 'P', 'S', 'T')
#define ZP_ACTIVATE_PARAM_Z 				FOURCC('A', 'P', 'Z', 'B')
#define ZP_ACTIVATE_PARAM_INS				FOURCC('A', 'P', 'I', 'N')

#define ZP_ACTIVATE_PARAM_PH				FOURCC('A', 'P', 'P', 'H')

typedef struct {
	uint32 desc;
	uint64 mid;
} ActivateRequestParam;

typedef struct {
	uint64 sn;
} ActivateResponseParam;

typedef struct {
	uint64 sn;
	uint16 id;
	uint16 panid;
	uint8 channel;
	uint16 freq;
	uint8 power;
	uint8 powersaving;
} ActiviteRspNConfigParam;

typedef struct {
	uint8 type;
	uint8 param[0];
} ActivateBody;

#pragma pack()


ActivateBody* zproto_activate_body(ZProto *proto);

#endif /* ZPROTO_ACTIVATE_H_ */
