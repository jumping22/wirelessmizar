/*
 * zproto_tel.h
 *
 *  Created on: Sep 24, 2014
 *      Author: toto
 */

#ifndef ZPROTO_TEL_H_
#define ZPROTO_TEL_H_

#include "zproto.h"

#pragma pack(1)

/******************** ZPROTO_TYPE_TEL_BS ********************/
typedef enum {
	BS_CONNECT = 1,
	BS_DISCONNECT,
	BS_MSG,
	BS_CALL,
	BS_HANGUP,
	BS_ANSWER,
	BS_CONFIG,
	BS_STATUS,
	BS_MAINTAIN,
	BS_DIAGNOSE,
} BsOption;

typedef struct {
	uint16 id;
	uint16 signalId;
	uint16 dataId;
} BsConnectParam;

typedef struct {
} BsDisconnectParam;

typedef struct {
	uint16 id;
	DataParam uri;
//	DataParam content;
} BsMsgParam;

typedef struct {
	DataParam uri;
} BsCallParam;

typedef struct {
} BsHangupParam;

typedef struct {
	uint16 code;
} BsAnswerParam;

typedef struct {

	uint16 id;
	uint32 duration;
	uint32 temperature;
	uint32 voltage;
	int32 command;
	int32 errorCode;
	int32 rfCode;
} BsStatusParam;

typedef struct {
	uint16 id;
	uint32 mdversion;
	char data[1];
//	DataParam log;
} BsMaintainParam;

typedef struct {
	uint16 id;
	uint32 signalFlow;
	uint32 dataFlow;
	uint32 registers;
	uint32 calls;
	uint32 cache;
	uint32 delay;
} BsDiagnoseParam;

typedef struct {
	quint16 id;
	quint16 sid;
	quint16 spanid;
	quint8 schannel;
	quint8 spower;
	quint16 mid;
	quint16 mpanid;
	quint8 mchannel;
	quint8 mpower;
	quint16 heartbeat;
	char data[0];
//	DataParam name;
//	DataParam factory;
} BsConfigParam;

/******************** ZPROTO_TYPE_TEL_DEV ********************/
typedef enum {
	DEV_CONNECT = 1,
	DEV_CALL,
	DEV_MSG,
	DEV_INCOMING_CALL,
	DEV_INCOMING_MSG,
	DEV_CONFIG,
	DEV_STATUS,
	DEV_MAINTAIN,
	DEV_DIAGNOSE,
} DevOption;

typedef struct {
	uint16 id;
	uint16 signalId;
    uint8 signalCh;
    uint16 signalPid;
	uint16 dataId;
    uint8 dataCh;
    uint16 dataPid;
	uint16 code;
} DevConnectParam;

typedef enum {
	CALL_DISCONNECTED,
	CALL_CONNECTING,
	CALL_CONFIRMED,
	CALL_TRYING,
} CallState;

typedef struct {
	uint8 state; // CallState
	uint16 code;
} DevCallParam;

typedef struct {
	uint16 id;
	uint16 code; // StatusCode
} DevMsgParam;

typedef struct {
	uint8 data[0]
//	DataParam uri;
} DevIncomingCallParam;

typedef struct {
	uint8 data[0]
//	DataParam uri;
//	DataParam content;
} DevIncomingMsgParam;

typedef struct {
} DevStatusParam;

typedef struct {
	uint8 option;
	uint8 param[0];
} TelBody;

typedef struct {
	uint16 samples_cnt;                     // SampleCount
	uint8  length;
    uint32 index;
//    uint64 time;
    uint8  media[1];
} MediaBody;

typedef struct {
	ZHeader header;
	TelBody body;
	ZTailer tailer;
} PacketResponse;

typedef struct {
	ZHeader header;
	MediaBody body;
	ZTailer tailer;
} PacketMedia;

typedef struct {
	uint8 size;
	uint8 data[1];
}ContactDate;

typedef struct {
	uint8 size;
	ContactDate name;
	ContactDate phoneNo;
}ContactBody;

#pragma pack()

TelBody* zproto_tel_body(ZProto *proto);

TelMedia* zproto_tel_media(ZProto *proto);


#endif /* ZPROTO_TEL_H_ */
