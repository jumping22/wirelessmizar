#ifndef ZPROTO_ANALYZE_H_
#define ZPROTO_ANALYZE_H_

#include <stdint.h>
#include "zproto.h"
#include "zproto_report.h"
#include "zproto_config.h"
#include "usrLib.h"



#ifdef __cplusplus
extern "C" {
#endif





#define ZPROTO_ANALYSE_BUF_SIZE		100
#define ZProtoRouterInfo			CfgZigbeeParam
#define ROUTER_ADDRESS				0xffff
#define LZ_REPORT_DATA_Z            FOURCC('R', 'P', 'D', 'D')
#define LZ_DEVICECOMFRIM_DATA_Z     FOURCC('C', 'M', 'D', 'B')



typedef uint8_t (*ZProto_reportCBFunc)(uint16_t id, uint8_t type, uint8_t* data, uint16_t data_size);
typedef uint8_t (*ZProto_confirmCBFunc)(uint16_t id, uint8_t* data, uint16_t data_size);
typedef void (*Zproto_sendFunc)(uint8_t*, uint16_t);
typedef int (*ZProto_analyze)(uint8_t*, uint16_t);
typedef void (*ZProto_send_to_device)(uint16_t id, uint8_t type, uint8_t cmd, uint8_t *data);

typedef void (*ZProto_register_sendFunc)(Zproto_sendFunc sendfunc);
typedef void (*ZProto_register_reportCBFunc)(ZProto_reportCBFunc report_callbackfunc);
typedef void (*ZProto_register_confirmCBFunc)(ZProto_confirmCBFunc confirm_callbackfunc);


typedef struct {
    ZProto_analyze          		analyze;
    Zproto_sendFunc         		sendfunc;
    ZProto_reportCBFunc     		reportCBfunc;
    ZProto_confirmCBFunc    		confirmCBfunc;
    ZProto_send_to_device			send;
    ZProto_register_sendFunc		register_sendFunc;
    ZProto_register_reportCBFunc	register_reportCBFunc;
    ZProto_register_confirmCBFunc	register_confirmCBFunc;
}ZProtoAnalyze_class;

typedef struct  {
	void (*router_init)(void);
	ZProtoRouterInfo (*router_getInfo)(void);
	int (*router_setInfo)(ZProtoRouterInfo para);
	int (*enddevice_setInfo)(uint16_t panID, uint8_t channel);
}ZProtoRouter_class;

typedef struct {
    CfgZigbeeParam info;
    uint8_t result;
}ZProtoRouter_struct;


ZProtoAnalyze_class* ZProtoAnalyze_Init(void);
ZProtoRouter_class* ZProtoRouter_Init(void);



#if defined(UNITY_TEST)

void Run_ZProtoAnalyze_test(void);

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TYPES_H_ */
