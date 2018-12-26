/*
 * zproto_sync.h
 *
 */

#ifndef ZPROTO_SYNC_H_
#define ZPROTO_SYNC_H_

#include "zproto.h"

#pragma pack(1)


#define ZP_TELEPHONE_CONTACTS               FOURCC('T', 'S', 'C', 'D')
#define ZP_TELEPHONE_ZB_CONFIG              FOURCC('T', 'S', 'Z', 'C')
#define ZP_TELEPHONE_CONFIRM                FOURCC('T', 'S', 'D', 'C')
#define ZP_DR_WHITELIST						FOURCC('D', 'S', 'W', 'L')

enum {
    SYNC_REQUEST = 1,  
    SYNC_RESPONSE,
    SYNC_DONE,
};

typedef struct {
    uint32 desc;
}SYNCRequestParam;

typedef struct {
    uint32 desc;
    uint8 st;
}SYNCResponseParam;

typedef struct {
    uint32 desc;
    uint32 dataversion;//数据版本号
}SYNCDoneParam;

typedef struct {
    uint8 option;
    uint8 param[1];
} SYNCBody;


#pragma pack()

//SyncBody* zproto_sync_body(ZProto *proto);

#endif /* ZPROTO_SYNC_H_ */
