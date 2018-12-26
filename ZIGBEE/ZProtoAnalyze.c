#include "ZProtoAnalyze.h"
#include "usrLib.h"
#include "zproto.h"
#include "zproto_report.h"
#include "zproto_config.h"

#include <string.h>
#include <stdio.h>

typedef struct {
  uint8 type;
  uint8 data[4];
} RepInputDataParam;

typedef struct {
	ZHeader head;
	uint8_t cmd;
	uint8_t type;
	uint8_t size;
	uint8_t data[4];
	ZTailer tail;
} ZProtoSensor;


typedef struct {
	uint8_t *buf;
} DataBufParam;

typedef struct {
  ZHeader header;  //12
  ReportBody rb;   //2
  DataUnit dbdu;    //5
  DataBufParam db; //4
  ZTailer tailer; //1
} ZProtoDeviceConfirm;  //24



//数据解析
static int analyze(uint8_t* data, uint16_t size);
static void send_to_device(uint16_t id, uint8_t type, uint8_t cmd, uint8_t *data);
static void register_sendFunc(Zproto_sendFunc sendfunc);
static void register_reportCBFunc(ZProto_reportCBFunc report_callbackfunc);
static void register_confirmCBFunc(ZProto_confirmCBFunc confirm_callbackfunc);
static void router_init(void);
static ZProtoRouterInfo router_getInfo(void);
static int router_setInfo(ZProtoRouterInfo para);
static int enddevice_setInfo(uint16_t panID, uint8_t channel);


static const uint8_t getRouterInfo_sendbuf[] = {0x57,0x0d,0x26,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x02,0x01,0x43,0x50,0x53,0x5a,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54};
static const uint8_t setRouterInfo_sendbuf[19] = {0x57,0x0d,0x26,0x00,0x00,0x2f,0xdb,0x00,0x01,0x00,0x00,0x00,0x02,0x03,0x43,0x50,0x53,0x5a,0x12};
static uint8_t setEnddeviceInfo_sendbuf[] = {0x57,0x0D,0x26,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x43,0x50,0x53,0x5A,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x04,0x6e,0x00,0x14,0x00,0x00,0x00,0x00,0x54};

static uint8_t ZProtoBuf[ZPROTO_ANALYSE_BUF_SIZE];
static uint16_t ZProtoBuf_index = 0;

static ZProtoRouter_struct routerInfo;
static ZProtoRouter_class router_class = {
		.router_init = router_init,
		.router_getInfo = router_getInfo,
		.router_setInfo = router_setInfo,
		.enddevice_setInfo = enddevice_setInfo,
};

static ZProtoAnalyze_class zproto_analyze = {
		.analyze = analyze,
		.register_sendFunc = register_sendFunc,
		.register_reportCBFunc = register_reportCBFunc,
		.register_confirmCBFunc = register_confirmCBFunc,
		.send = send_to_device,
};


static int _analyze_rawData(uint8_t data)
{
	static uint8_t _state = 0;
	static uint16_t _length = 0;

	switch(_state) {
		case 0: {
			if (data == 0x57) {
				_state = 1;
				_length = 0;
				ZProtoBuf_index = 0;
				ZProtoBuf[ZProtoBuf_index++] = data;
			}
		} break;
		case 1: {
			_state = 2;
			ZProtoBuf[ZProtoBuf_index++] = data;
		} break;
		case 2: {
			_length = data - 1; // sub:tail
			_state = 3;
			ZProtoBuf[ZProtoBuf_index++] = data;
		} break;
		case 3: {
			ZProtoBuf[ZProtoBuf_index++] = data;
			if (ZProtoBuf_index >= _length) {
				_state = 4;
			}
		} break;
		case 4: {
			_state = 0;
			if (data == 0x54) {
				ZProtoBuf[ZProtoBuf_index++] = data;
				return 1;
			} else {
				ZProtoBuf_index = 0;
				return -1;
			}
		} break;
		default:_state = 0;break;
	}
	return 0;
}

static uint8_t _analyze_zproto(uint8_t *data, uint16_t len)
{
  ZProto zProtoData;
  ZHeader *zhead = NULL;
  ReportBody *rptBody = NULL;
  DataUnit *rdDu = NULL;
  RepZigbeeParam *opara = NULL;
  RepInputDataParam *para = NULL;
  uint8_t duOffset = 0;
  uint8_t i = 0;
  uint8_t type = 0;

  ConfigBody *pcb = NULL;
  CfgZigbeeParam *czp = NULL;
  uint8_t *pdata = NULL;

  if(zproto_parseFromArray(&zProtoData, data, len) <= 0) {
    //*offset = 1;
    return 1;
  }
  //*offset = zProtoData.size;
  type = zproto_type(&zProtoData);
  switch(type)
  {
    case ZPROTO_TYPE_REPORT:
      zhead = (ZHeader *)data;
      rptBody = (ReportBody *)&data[sizeof(ZHeader)];
      if( rptBody->type == 7 ) {
        duOffset = 0;
        for(i=0; i<rptBody->count; i++) {
          rdDu = (DataUnit *)&data[sizeof(ZHeader)+sizeof(ReportBody)+duOffset];
          if( rdDu->desc == ZP_REPORT_PARAM_Z ) //R P Z B
          {
            opara = (RepZigbeeParam *)&data[sizeof(ZHeader)+sizeof(ReportBody)+duOffset+sizeof(DataUnit)];
            //opara->freq;
            //opara->vdd;
            //printf("Freq =%d \r\nVdd =%d\r\n", opara->freq, opara->vdd);
          }
          if(rdDu->desc == LZ_REPORT_DATA_Z)      //R P D D
          {
            para = (RepInputDataParam*)&data[sizeof(ZHeader)+sizeof(ReportBody)+duOffset+sizeof(DataUnit)];
            if (zproto_analyze.reportCBfunc) {
                zproto_analyze.reportCBfunc(zhead->srcId, para->type, para->data, 4);
            }
            break;
          }
          duOffset += (sizeof(DataUnit)+rdDu->length);
        }
      }
      break;
      
    case ZPROTO_TYPE_CONFIG: {  //0d router info
      zhead = (ZHeader *)data;
      pcb = (ConfigBody *)&data[sizeof(ZHeader)];
      if (pcb->devMode == DEV_Mode_SZ && pcb->opType == OPERATE_TYPE_GET_RETURN) {
				rdDu = (DataUnit *)&data[sizeof(ZHeader)+sizeof(ConfigBody)];
				if( rdDu->desc == ZP_CONFIG_PARAM_SZ && rdDu->length == 26) { // C P S Z
						czp = (CfgZigbeeParam *)&data[sizeof(ZHeader)+sizeof(ConfigBody)+sizeof(DataUnit)];
						routerInfo.info.id = czp->id;
						routerInfo.info.panid = czp->panid;
						routerInfo.info.channel = czp->channel;
						routerInfo.info.freq = czp->freq;
						routerInfo.info.power = czp->power;
								routerInfo.result = 1;
				}
      }
    }break;
    
    case ZPROTO_TYPE_CNF: {     //c9
    	zhead = (ZHeader *)data;
    	if (zhead->srcId == 0 && zhead->destId == 0) {	//router confirm
    		routerInfo.result = 1;
    	} else {																				//device confirm
    		//do nothing
    		rptBody = (ReportBody *)&data[sizeof(ZHeader)];
    		if ( rptBody->type == 7 ) {
					rdDu = (DataUnit *)&data[sizeof(ZHeader)+sizeof(ReportBody)];
					if( rdDu->desc == LZ_DEVICECOMFRIM_DATA_Z ) //C M D B
					{
						//pdata = (DataBufParam *)&data[sizeof(ZHeader)+sizeof(ReportBody)+sizeof(DataUnit)];
						pdata = (uint8_t *)&data[sizeof(ZHeader)+sizeof(ReportBody)+sizeof(DataUnit)];
						if (zproto_analyze.confirmCBfunc) {
								zproto_analyze.confirmCBfunc(zhead->srcId, pdata, rdDu->length);
						}
					}
    		}
    	}
    }break;
    
    default:
      break;
  }
  return type;
}

int analyze(uint8_t* data, uint16_t size)
{
  uint16_t i=0;

	for(; i<size; i++) {
		if (_analyze_rawData(data[i]) == 1) {
			_analyze_zproto(ZProtoBuf, ZProtoBuf_index);
		}
	}

	return 0;
}


static void send_to_device(uint16_t id, uint8_t type, uint8_t cmd, uint8_t *data)
{
	ZProtoSensor zpdev;

	zpdev.head.signature = ZPROTO_HEADER_SIGNATURE;
	zpdev.head.type = ZPROTO_TYPE_CNF;
	zpdev.head.length = sizeof(ZHeader) + sizeof(ZTailer) + 7;
	zpdev.head.srcId = ROUTER_ADDRESS;
	zpdev.head.destId = id;
	zpdev.head.seq = 0;

	zpdev.cmd = cmd;
	zpdev.type = type;
	zpdev.size = 4;
	memcpy(zpdev.data, data, 4);

	zpdev.tail.signature = ZPROTO_TAILER_SIGNATURE;

	if (zproto_analyze.sendfunc != NULL) {
		zproto_analyze.sendfunc((uint8_t*)&zpdev, zpdev.head.length);
	}
}



static void router_init(void)
{
	if (zproto_analyze.sendfunc != NULL) {
		zproto_analyze.sendfunc((uint8_t*)getRouterInfo_sendbuf, getRouterInfo_sendbuf[2]);
	}
}


/*
 * Get router info
 * include: id panid channel freq power
 */
static ZProtoRouterInfo router_getInfo(void)
{
	return routerInfo.info;
}


/*
 * Set router info
 * include: id panid channel freq power
 */
static int router_setInfo(ZProtoRouterInfo para)
{
	uint8_t tail[] = {0x54};

	if (zproto_analyze.sendfunc != NULL) {
		zproto_analyze.sendfunc((uint8_t*)setRouterInfo_sendbuf, 19);
		zproto_analyze.sendfunc((uint8_t*)&para, sizeof(CfgZigbeeParam));
		zproto_analyze.sendfunc(tail, 1);
	}
	routerInfo.info = para;
	return 0;
}



static int enddevice_setInfo(uint16_t panID, uint8_t channel)
{
	setEnddeviceInfo_sendbuf[30] = (uint8_t)panID;
	setEnddeviceInfo_sendbuf[31] = (uint8_t)(panID >> 8);
	setEnddeviceInfo_sendbuf[32] = channel;

	if (zproto_analyze.sendfunc != NULL) {
		zproto_analyze.sendfunc(setEnddeviceInfo_sendbuf, setEnddeviceInfo_sendbuf[2]);
	}
	return 0;
}


static void register_sendFunc(Zproto_sendFunc sendfunc)
{
	zproto_analyze.sendfunc = sendfunc;
}
static void register_reportCBFunc(ZProto_reportCBFunc report_callbackfunc)
{
	zproto_analyze.reportCBfunc = report_callbackfunc;
}
static void register_confirmCBFunc(ZProto_confirmCBFunc confirm_callbackfunc)
{
	zproto_analyze.confirmCBfunc = confirm_callbackfunc;
}


ZProtoAnalyze_class* ZProtoAnalyze_Init(void)
{
    return &zproto_analyze;
}

ZProtoRouter_class* ZProtoRouter_Init(void)
{
	return &router_class;
}


#if defined(UNITY_TEST)

#include "unity.h"
#include "unity_fixture.h"

static ZProtoAnalyze_class *zproto;
static ZProtoRouter_class *router;

static uint8_t routerInfo_buf[] = {1,2,3,4,6,7,8,5,0x57,0x0D,0x2E,0x00,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x43,0x50,0x53,0x5A,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD0,0x6B,0x00,0x11,0xF4,0x01,0xE5,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x54,0x57,0xC9,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x57,0x0C,0x21,0x57,0x04,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x07,0x02,0x52,0x50,0x5A,0x42,0x03,0x00,0x00,0x00,0x52,0x50,0x44,0x44,0x05,0x67,0x00,0x00,0x00,0xFF,0x54,0x57,0x0C,0x21,0x57,0x04,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x07,0x02,0x52,0x50,0x5A,0x42,0x03,0x00,0x00,0x00,0x52,0x50,0x44,0x44,0x05,0x67,0x00,0x00,0x00,0xFF,0x54};
static uint8_t testdata_buf[] = {87,12,33,80,4,255,255,184,11,0,0,0,7,2,82,80,90,66,3,28,244,1,82,80,68,68,5,104,1,255,255,255,84};

static void sendfunc(uint8_t *data, uint16_t size)
{
	uint16_t i=0;

	printf("Send:");
	for (; i<size; i++) {
		printf("%2x ", data[i]);
	}
	printf("\r\n");
}

static uint8_t reportCBFunc(uint16_t id, uint8_t type, uint8_t* data, uint16_t data_size)
{
	printf("Report package id:%d, type:%d, ", id, type);
	sendfunc(data, data_size);
	return 0;
}

static uint8_t confirmCBFunc(uint16_t id, uint8_t* data, uint16_t data_size)
{
	printf("Confirm package id:%d, ", id);
	sendfunc(data, data_size);
	return 0;
}

TEST_GROUP( ZProtoAnalyze_test );

TEST_SETUP( ZProtoAnalyze_test )
{
	zproto = ZProtoAnalyze_Init();

    zproto->register_sendFunc(sendfunc);
    zproto->register_reportCBFunc(reportCBFunc);
    zproto->register_confirmCBFunc(confirmCBFunc);

    router = ZProtoRouter_Init();
    router->router_init();
}

TEST_TEAR_DOWN( ZProtoAnalyze_test )
{
    
}

TEST( ZProtoAnalyze_test, ZProtoAnalyze_test_all)
{
	zproto->analyze(routerInfo_buf, sizeof(routerInfo_buf));
	ZProtoRouterInfo routerinfo = router->router_getInfo();

	TEST_ASSERT_EQUAL(0xd000,  routerinfo.id);
	TEST_ASSERT_EQUAL(0x006b,  routerinfo.panid);
	TEST_ASSERT_EQUAL(0x11,  routerinfo.channel);

	ZProtoRouterInfo param;
	param.id = 0xfff1;
	param.panid = 110;
	param.channel = 10;
	param.freq = 1000;
	param.power = 0xf5;

	router->router_setInfo(param);
	routerinfo = router->router_getInfo();

    TEST_ASSERT_EQUAL(0xfff1,  routerinfo.id);
    TEST_ASSERT_EQUAL(110,  routerinfo.panid);
    TEST_ASSERT_EQUAL(10,  routerinfo.channel);

    zproto->analyze(testdata_buf, sizeof(testdata_buf));

}


TEST_GROUP_RUNNER(ZProtoAnalyze_test)
{
	RUN_TEST_CASE(ZProtoAnalyze_test, ZProtoAnalyze_test_all);
}


void Run_ZProtoAnalyze_test(void)
{
	RUN_TEST_GROUP(ZProtoAnalyze_test);
}

#endif

