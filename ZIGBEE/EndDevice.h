#ifndef __ENDDEVICE_H
#define __ENDDEVICE_H

#include "ZProtoAnalyze.h"
#include "EndDeviceNode.h"
#include "EndDevice_type.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*Zproto_sendFunc)(uint8_t*, uint16_t);
#define GetTypebyID(id)         (id-1000)
	
	
	
#define WL_IRDISTANCE	                1010
#define WL_IRREMOTE	                  1020
#define WL_PRS	                			1030
#define WL_DHT11	                    1040
#define WL_PIR	            					1050
#define WL_POTENTIOMETER	            1060
#define WL_BIGKEY	                    1070


/****************************************************************************/

/*
*		传感器初始化  传感器和输出设备分开了
*
*/
#define make_sensor_new(name, Sdeivce, _id) \
Sdeivce get##name(void); \
void save##name(Sdeivce data); \
void set##name##Confirm(Sdeivce data); \
Sdeivce get##name##Confirm(void); \
Swireless_##name name = { \
	.node.id = _id, \
	.node.data.type = _id-1000, \
	.get = get##name, \
	.saveRecvData = save##name, \
	.setConfirmData = set##name##Confirm, \
	.getConfirmData = get##name##Confirm, \
}; \
Sdeivce get##name(void) \
{ \
    return *(Sdeivce*)&(name.node.data.data); \
} \
void save##name(Sdeivce data) \
{ \
	memcpy(name.node.data.data, data.buf, 4); \
	enddeviceNode->update(&name.node); \
} \
void set##name##Confirm(Sdeivce data) \
{ \
	name.confirmdata = data; \
} \
Sdeivce get##name##Confirm(void) \
{ \
	return name.confirmdata; \
}

//先改结构体
#define make_device_struct(name)	Swireless_##name

#define make_sensor_struct(name, Sdeivce) \
typedef struct { \
	EndDeviceNode node; \
	Sdeivce confirmdata; \
	Sdeivce (*get)(void); \
	void (*saveRecvData)(Sdeivce); \
	void (*setConfirmData)(Sdeivce); \
	Sdeivce (*getConfirmData)(void); \
}Swireless_##name;



/****************************************************************************/




//#define make_device_struct(name)	Wireless_##name##_struct

//#define make_sensor_struct(name, deivce_struct) \
//typedef struct { \
//    EndDeviceNode data; \
//    deivce_struct (*get)(void); \
//    void (*save)(deivce_struct); \
//}Wireless_##name##_struct;

//#define make_output_struct(name, deivce_struct) \
//typedef struct { \
//    uint16_t id; \
//    uint8_t type; \
//    deivce_struct data; \
//    EndDeviceNode_class *node; \
//    deivce_struct (*get)(void); \
//    void (*set)(deivce_struct); \
//}Wireless_##name##_struct;


make_sensor_struct(IRDISTANCE, 				IRdistance_struct);
make_sensor_struct(IRREMOTE, 					IRremote_struct);
make_sensor_struct(PRS, 							PRS_struct);
make_sensor_struct(DHT11, 						DHT11_struct);
make_sensor_struct(PIR, 							PIR_struct);
make_sensor_struct(POTENTIOMETER, 		Potentiometer_struct);
make_sensor_struct(BIGKEY, 						BigKey_struct);

//make_output_struct(RGBLED, 			RGBLED_struct);
//make_output_struct(ACTIVE_BUZZER, 	ACTIVE_BUZZER_struct);
//make_output_struct(STEPPER_MOTOR, 	STEPPER_MOTOR_struct);


typedef struct  {
	//每个设备的数据回掉函数
	make_device_struct(IRDISTANCE) 				*irdistance;
	make_device_struct(IRREMOTE) 					*irremote;
	make_device_struct(PRS) 							*prs;
	make_device_struct(DHT11) 						*dht11;
	make_device_struct(PIR) 							*pir;
	make_device_struct(POTENTIOMETER) 		*potentiometer;
	make_device_struct(BIGKEY) 						*bigkey;
	//
	ZProtoRouter_class 					*Router;
	int (*checkOnlineByID)(uint16_t);
	ZProto_analyze          		analyze;
	//ZProto_register_sendFunc			register_sendFunc;
}WirelessDevice_class;

/*		WirelessDevice_Init函数		*/
/*
 * 		该函数返回wireless类的实例，包括如下函数：
 * 		传感器：
 * 			DHT11:温湿度传感器
 * 			PRS:光强传感器
 * 			SMOKE:气体传感器
 * 			PIR:人体红外传感器
 * 			ULTRASONIC:超声波传感器
 * 			VIBRATION:震动传感器
 * 		每个传感器包含如下方法：
 * 			get();	获取传感器的值
 *
 * 		输出设备：
 * 			RGBLED:RGB灯
 * 			ACTIVE_BUZZER:有源蜂鸣器
 * 			STEPPER_MOTOR:步进电机
 * 		每个传感器包含如下方法：
 * 			set();	设置输出设备的值
 *
 * 		Router:路由
 * 			router_getInfo:获取路由配置信息
 * 			router_setInfo:设置路由配置信息
 * 			enddevice_setInfo:设置该路由下所有节点的配置信息
 * 		其他函数：
 * 			analyze:数据解析函数，接收到数据包时调用
 * 			register_sendFunc:注册数据发送函数
 * 			checkOnlineByID:通过ID查询设备是否在线
 */
//WirelessDevice_class* WirelessDevice_Init(void);
WirelessDevice_class* WirelessDevice_Init(Zproto_sendFunc sendfunc);



#if defined(UNITY_TEST)
void Run_EndDevice_test(void);
#endif


#ifdef __cplusplus
}
#endif

#endif
