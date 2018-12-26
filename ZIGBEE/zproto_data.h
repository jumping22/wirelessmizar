/*
 * zproto_data.h
 *
 *  Created on: Sep 26, 2014
 *      Author: toto
 */

#ifndef ZPROTO_DATA_H_
#define ZPROTO_DATA_H_

#include "zproto.h"

#pragma pack(1)

#define ZP_TELEPHONE_FONT					FOURCC('T', 'P', 'Z', 'D')
#define ZP_TELEPHONE_CALL_RING				FOURCC('T', 'P', 'C', 'D')
#define ZP_TELEPHONE_MSG_RING				FOURCC('T', 'P', 'M', 'D')
#define ZP_TELEPHONE_SETTING				FOURCC('T', 'P', 'S', 'D')
#define ZP_DR_WHITE_LIST					FOURCC('D', 'R', 'W', 'L')
#define ZP_DATA_ZUL             			FOURCC('D', 'Z', 'U', 'L')
#define ZP_DATA_SBL_Z           			FOURCC('D', 'S', 'L', 'Z')
#define ZP_DATA_SBL_ST          			FOURCC('D', 'S', 'L', 'S')
#define ZP_QT_DATA_REQUEST					FOURCC('D', 'Q', 'R', 'Q')    /*潜艇数据请求*/
#define ZP_QT_DATA 							FOURCC('D', 'Q', 'D', 'T')    /*潜艇数据发送*/

typedef enum{
	dataOk = 0,
	dataBeginError,
	dataExtra,
	dataCrcError
}DataConfirmType;

typedef struct{
	uint8_t dataLen;
}DataPackageParam;

typedef struct{
	DataConfirmType Confirm;
}DataConfirmParam;

typedef struct {
	uint32 desc;
	uint32 size;
	uint32 offset;
	DataParam data;
	uint32 crc;//  前面所有字节的异或校验，包括header等
} DataBody;

typedef struct
{
    uint16 zid;
}DoorWhiteList;

#pragma pack()

DataBody* zproto_data_body(ZProto *proto);

#endif /* ZPROTO_DATA_H_ */
