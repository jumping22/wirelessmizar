#include "EndDevice.h"
#include "ZProtoAnalyze.h"
#include "EndDeviceNode.h"
#include "EndDevice_type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


EndDeviceNode_class *enddeviceNode;
ZProtoAnalyze_class *zprotoAnalyze;


static uint8_t reportCBFunc(uint16_t id, uint8_t type, uint8_t* data, uint16_t data_size);
static uint8_t confirmCBFunc(uint16_t id, uint8_t* data, uint16_t data_size);
static int _checkOnlineByID(uint16_t id);


/* 			sensor device class			 */
/*
#define make_sensor_new(name, deivce_struct, _id, _type) \
typedef struct { \
    uint16_t id; \
    uint8_t type; \
    deivce_struct data; \
    deivce_struct (*get)(void); \
    void (*save)(deivce_struct); \
}Wireless_##name##_struct; \
deivce_struct get##name(void); \
void save##name(deivce_struct data); \
Wireless_##name##_struct name = { \
    .id = _id, \
    .type = _type, \
    .get = get##name, \
    .save = save##name, \
}; \
deivce_struct get##name(void) \
{ \
    return name.data; \
} \
void save##name(deivce_struct data) \
{ \
    name.data = data; \
}
*/

/*
#define make_sensor_new(name, deivce_struct, _id, _type) \
typedef struct { \
    EndDeviceNode data; \
    deivce_struct (*get)(void); \
    void (*save)(deivce_struct); \
}Wireless_##name##_struct; \
deivce_struct get##name(void); \
void save##name(deivce_struct data); \
Wireless_##name##_struct name = { \
    .data.id = _id, \
    .data.data.type = _type, \
    .get = get##name, \
    .save = save##name, \
}; \
deivce_struct get##name(void) \
{ \
    return *(deivce_struct*)&(name.data.data.data); \
} \
void save##name(deivce_struct data) \
{ \
	memcpy(name.data.data.data, data.buf, 4); \
	enddeviceNode->update(&name.data); \
}
*/


/*
*		传感器初始化  传感器和输出设备分开了
*
*/
//#define make_sensor_new(name, deivce_struct, _id, _type) \
//deivce_struct get##name(void); \
//void save##name(deivce_struct data); \
//Wireless_##name##_struct name = { \
//    .data.id = _id, \
//    .data.data.type = _type, \
//    .get = get##name, \
//    .save = save##name, \
//}; \
//deivce_struct get##name(void) \
//{ \
//    return *(deivce_struct*)&(name.data.data.data); \
//} \
//void save##name(deivce_struct data) \
//{ \
//	memcpy(name.data.data.data, data.buf, 4); \
//	enddeviceNode->update(&name.data); \
//}


///* 			output device class			 */
//#define make_output_new(name, deivce_struct, _id, _type) \
//deivce_struct get##name(void); \
//void set##name(deivce_struct data); \
//Wireless_##name##_struct name = { \
//    .id = _id, \
//    .type = _type, \
//    .get = get##name, \
//    .set = set##name, \
//}; \
//deivce_struct get##name(void) \
//{ \
//    return name.data; \
//} \
//void set##name(deivce_struct data) \
//{ \
//    name.data = data; \
//}


make_sensor_new(IRDISTANCE, 			IRdistance_struct, 			WL_IRDISTANCE);
make_sensor_new(IRREMOTE, 				IRremote_struct, 				WL_IRREMOTE);
make_sensor_new(PRS, 							PRS_struct, 						WL_PRS);
make_sensor_new(DHT11, 						DHT11_struct, 					WL_DHT11);
make_sensor_new(PIR, 							PIR_struct, 						WL_PIR);
make_sensor_new(POTENTIOMETER, 		Potentiometer_struct, 	WL_POTENTIOMETER);
make_sensor_new(BIGKEY, 					BigKey_struct, 					WL_BIGKEY);

//make_output_new(RGBLED, 				RGBLED_struct, 					WL_RGBLED);
//make_output_new(ACTIVE_BUZZER, 	ACTIVE_BUZZER_struct, 	WL_ACTIVE_BUZZER);
//make_output_new(STEPPER_MOTOR, 	STEPPER_MOTOR_struct, 	WL_STEPPER_MOTOR);




static WirelessDevice_class _wireless = {
	.irdistance = &IRDISTANCE,
	.irremote = &IRREMOTE,
	.prs = &PRS,
	.dht11 = &DHT11,
	.pir = &PIR,
	.potentiometer = &POTENTIOMETER,
	.bigkey = &BIGKEY,
	.checkOnlineByID = _checkOnlineByID,
};




WirelessDevice_class* WirelessDevice_Init(Zproto_sendFunc sendfunc)
{
	zprotoAnalyze = ZProtoAnalyze_Init();
	_wireless.Router = ZProtoRouter_Init();
	enddeviceNode = EndDeviceNode_Init();
    
	_wireless.analyze = zprotoAnalyze->analyze;

	zprotoAnalyze->register_sendFunc(sendfunc);		//需要用户来注册发送函数
	zprotoAnalyze->register_reportCBFunc(reportCBFunc);
	zprotoAnalyze->register_confirmCBFunc(confirmCBFunc);
    
	//_wireless.Router->router_init();

    return &_wireless;
}

static int _checkOnlineByID(uint16_t id)
{
	return enddeviceNode->getByID(id)?1:0;
}


static uint8_t reportCBFunc(uint16_t id, uint8_t type, uint8_t* data, uint16_t data_size)
{
	//printf("\r\nReport package id:%d, type:%d, ", id, type);

	switch (type) {
	case GetTypebyID(WL_DHT11): {
		_wireless.dht11->saveRecvData(*(DHT11_struct*)data);	//保存enddevice发来的数据
		}break;

	case GetTypebyID(WL_PRS): {
		_wireless.prs->saveRecvData(*(PRS_struct*)data);
		}break;

	case GetTypebyID(WL_IRDISTANCE): {
		_wireless.irdistance->saveRecvData(*(IRdistance_struct*)data);
		}break;

	case GetTypebyID(WL_PIR): {
		_wireless.pir->saveRecvData(*(PIR_struct*)data);
		}break;

	case GetTypebyID(WL_IRREMOTE): {
		IRremote_struct send = _wireless.irremote->getConfirmData();		//获取要发送给enddevice的数据
		_wireless.irremote->saveRecvData(*(IRremote_struct*)data);
		zprotoAnalyze->send(id, type, 1, send.buf);					//发送该数据
		}break;

	case GetTypebyID(WL_POTENTIOMETER): {
		_wireless.potentiometer->saveRecvData(*(Potentiometer_struct*)data);
		}break;

	case GetTypebyID(WL_BIGKEY): {
		_wireless.bigkey->saveRecvData(*(BigKey_struct*)data);
		}break;

	}
	return 0;
}

static uint8_t confirmCBFunc(uint16_t id, uint8_t* data, uint16_t data_size)
{
	//printf("Confirm package id:%d, ", id);
	//sendfunc(data, data_size);
	return 0;
}




#if defined(UNITY_TEST)

#include "unity.h"
#include "unity_fixture.h"


WirelessDevice_class *test_wireless;

static uint8_t routerInfo_buf[] = {1,2,3,4,6,7,8,5,0x57,0x0D,0x2E,0x00,0xD0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x43,0x50,0x53,0x5A,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD0,0x6B,0x00,0x11,0xF4,0x01,0xE5,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x54, \
		0x57,0xC9,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54, \
		0x57,0x0C,0x21,0xf2,0x03,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x07,0x02,0x52,0x50,0x5A,0x42,0x03,0x00,0x00,0x00,0x52,0x50,0x44,0x44,0x05,10,0x01,0x00,0x02,0x00,0x54, \
		0x57,0x0C,0x21,0xfc,0x03,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x07,0x02,0x52,0x50,0x5A,0x42,0x03,0x00,0x00,0x00,0x52,0x50,0x44,0x44,0x05,20,100,0x00,0x00,0x00,0x54,};

static uint8_t rgb_report_buf[] = {0x57,0x0C,0x21,0x42,0x04,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x07,0x02,0x52,0x50,0x5A,0x42,0x03,0x00,0x00,0x00,0x52,0x50,0x44,0x44,0x05,90,0x00,0x00,0x00,0x00,0x54};
static uint8_t testdata_buf[] = {87,12,33,80,4,255,255,184,11,0,0,0,7,2,82,80,90,66,3,28,244,1,82,80,68,68,5,104,1,255,255,255,84};

TEST_GROUP( EndDevice_test );

TEST_SETUP( EndDevice_test )
{
	test_wireless = WirelessDevice_Init();
}

TEST_TEAR_DOWN( EndDevice_test )
{

}

TEST( EndDevice_test, EndDevice_test_all)
{
	TEST_ASSERT_EQUAL(0, test_wireless->checkOnlineByID(1010));
	TEST_ASSERT_EQUAL(0, test_wireless->checkOnlineByID(1020));

	test_wireless->analyze(routerInfo_buf, sizeof(routerInfo_buf));

	TEST_ASSERT_EQUAL(1, test_wireless->checkOnlineByID(1010));
	TEST_ASSERT_EQUAL(1, test_wireless->checkOnlineByID(1020));

	DHT11_struct _dht11 = test_wireless->dht11->get();
	TEST_ASSERT_EQUAL(1, _dht11.temperature);
	TEST_ASSERT_EQUAL(2, _dht11.humidity);

	PRS_struct _prs = test_wireless->prs->get();
	TEST_ASSERT_EQUAL(100, _prs.analog);

	uint8_t result1[] = {0x57,0xc9,0x14,0xff,0xff,0x42,0x04,0,0,0,0,0,0x01,90, 0x04, 10,20,30,0, 0x54};
	RGBLED_struct _rgb = {
			.r = 10,
			.g = 20,
			.b = 30,
			.dummy = 0,
	};
	test_wireless->rgbled->set(_rgb);
	test_wireless->analyze(rgb_report_buf, sizeof(rgb_report_buf));


//	ZProtoRouterInfo routerinfo = router->router_getInfo();
//
//	TEST_ASSERT_EQUAL(0xd000,  routerinfo.id);
//	TEST_ASSERT_EQUAL(0x006b,  routerinfo.panid);
//	TEST_ASSERT_EQUAL(0x11,  routerinfo.channel);
//
//	ZProtoRouterInfo param;
//	param.id = 0xfff1;
//	param.panid = 110;
//	param.channel = 10;
//	param.freq = 1000;
//	param.power = 0xf5;
//
//	router->router_setInfo(param);
//	routerinfo = router->router_getInfo();
//
//    TEST_ASSERT_EQUAL(0xfff1,  routerinfo.id);
//    TEST_ASSERT_EQUAL(110,  routerinfo.panid);
//    TEST_ASSERT_EQUAL(10,  routerinfo.channel);
//
//    zproto->analyze(testdata_buf, sizeof(testdata_buf));

}


TEST_GROUP_RUNNER(EndDevice_test)
{
	RUN_TEST_CASE(EndDevice_test, EndDevice_test_all);
}


void Run_EndDevice_test(void)
{
	RUN_TEST_GROUP(EndDevice_test);
}

#endif







