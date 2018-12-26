/*
 * zproto_register.h
 *
 *  Created on: Sep 28, 2014
 *      Author: toto
 */

#ifndef ZPROTO_REGISTER_H_
#define ZPROTO_REGISTER_H_

#include "zproto.h"

#pragma pack(1)

typedef enum {
	REG_TYPE_ASK = 1,
	REG_TYPE_ANSWER,
	REG_TYPE_REQUEST,
	REG_TYPE_RESPONSE,
} RegisterType;

#define ZP_REGISTER_PARAM_ASK 				FOURCC('R', 'P', 'A', 'K')

typedef struct {
	uint32 desc;
	uint8 level;
} RegisterAskParam;

typedef struct {
	uint32 desc;
	DateTime time;
	uint8 level;
} RegisterAnswerParam;

typedef struct {
	uint32 desc;
	uint8 type;
	uint8 level;
	uint8 perms;
} RegisterRequestParam;

typedef struct {
	uint16 code;
	uint8 level;
} RegisterResponseParam;

typedef struct {
	uint8 type;
	uint8 param[0];
} RegisterBody;

#pragma pack()


RegisterBody* zproto_register_body(ZProto *proto);


#endif /* ZPROTO_REGISTER_H_ */
