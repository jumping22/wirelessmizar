/*
 * zproto.h
 *
 *  Created on: Sep 23, 2014
 *      Author: toto
 */

#ifndef ZPROTO_H_
#define ZPROTO_H_

#include "types.h"

#define ZPROTO_HEADER_SIGNATURE 'W'
#define ZPROTO_TAILER_SIGNATURE 'T'
#define ZPROTO_BROADCAST_ID 0xFFFF
#define ZPROTO_BUFFER_MAX_SIZE 110

#pragma pack(1)

typedef enum {
	ZPROTO_TYPE_UNKNOWN = 0,
	ZPROTO_TYPE_ACTIVATE = 10, // Proto start from here
	ZPROTO_TYPE_REGISTER,
	ZPROTO_TYPE_REPORT,
	ZPROTO_TYPE_CONFIG,
	ZPROTO_TYPE_DATA,
	ZPROTO_TYPE_TRANSPOND,
	ZPROTO_TYPE_SYNC,
	ZPROTO_TYPE_TEL_BS = 100, // Telephony start from here
	ZPROTO_TYPE_TEL_DEV,
	ZPROTO_TYPE_TEL_MEDIA,
	ZPROTO_TYPE_MAC = 200, // sequence used for functionality from here
	ZPROTO_TYPE_CNF,
	ZPROTO_TYPE_LOOP,
        ZPROTO_TYPE_GETIPORT,
} ZProtoType;

#define ZPROTO_HEADER_LEN_OFFSET      2

typedef struct
{
	uint8 signature;
	uint8 type;
	uint8 length;
	uint16 srcId;
	uint16 destId;
	uint8 rssi;
	uint32 seq;
//	uint16 checksum;
} ZHeader;

typedef struct
{
	uint8 signature;
} ZTailer;

#pragma pack()

typedef struct
{
	void *data;
	uint8 size;
} ZProto;

int zproto_checkValidity(const void *data, uint8 size);

int zproto_parseFromArray(ZProto *proto, void *data, uint8 size);

ZHeader* zproto_header(ZProto *proto);

int zproto_type(ZProto *proto);

#endif /* ZPROTO_H_ */
