/*
 * zproto_report.h
 *
 *  Created on: Sep 26, 2014
 *      Author: toto
 */

#ifndef ZPROTO_REPORT_H_
#define ZPROTO_REPORT_H_

#include "zproto.h"

#pragma pack(1)

#define GPS_PARAM_LEN           24

typedef enum {
	REPORT_PARAMETER = 0x1,
	REPORT_SAMPLING = 0x2,
	REPORT_BUSSINESS = 0x4,
} ReportType;

#define ALARM_KEY						0x01
#define ALARM_DEV_UNLOCK				0x02
#define ALARM_FALL						0x04

#define ZP_REPORT_PARAM_Z				    FOURCC('R', 'P', 'Z', 'B')

#define ZP_REPORT_SP_TEMPERATURE 		FOURCC('R', 'S', 'T', 'P')
#define ZP_REPORT_SP_ACCELERATION 	FOURCC('R', 'S', 'A', 'C')
#define ZP_REPORT_SP_BLOB				    FOURCC('R', 'S', 'B', 'L')
#define ZP_REPORT_SP_RH					    FOURCC('R', 'S', 'R', 'H')
#define ZP_REPORT_SP_GPS				    FOURCC('R', 'S', 'G', 'P')
#define ZP_REPORT_SP_RAD				    FOURCC('R', 'S', 'R', 'A')
#define ZP_REPORT_SP_STEPS				  FOURCC('R', 'S', 'S', 'T')

#define ZP_REPORT_BS_COMMAND			  FOURCC('R', 'B', 'C', 'D')
#define ZP_REPORT_BS_PRODUCT			  FOURCC('R', 'B', 'P', 'D')
#define ZP_REPORT_BS_ROUTER			 	  FOURCC('R', 'B', 'R', 'T')
#define ZP_REPORT_BS_PHONEPARAM			FOURCC('R', 'B', 'P', 'P')
#define ZP_REPORT_BS_MAG				    FOURCC('R', 'B', 'M', 'G')
#define ZP_REPORT_BS_PHONENUM			  FOURCC('R', 'B', 'P', 'N')

#define ZP_REPORT_BS_QT_DATA        FOURCC('R', 'B', 'Q', 'T') // by wrg 2016-2-26  QT上报的工卡人员配置信息
#define ZP_REPORT_BS_QT_WD_DATA     FOURCC('R', 'B', 'Q', 'W') // by wrg 2016-2-26  QT上报的腕带人员配置信息
#define ZP_REPORT_BS_QT_NAME			  FOURCC('R', 'B', 'N', 'M')

#define ZP_REPORT_INS					      FOURCC('R', 'I', 'I', 'N')
#define ZP_REPORT_VIN					      FOURCC('R', 'V', 'I', 'N')

#define ZP_REPORT_BS_OTHPARA			FOURCC('W', 'L', 'L', 'B')

typedef struct {
	uint8 vdd;
	uint16 freq;
} RepZigbeeParam;



typedef struct {
	uint32 ax;
	uint32 ay;
	uint32 az;
} RepAccelParam;

typedef struct {
	uint32 steps;
} RepStepsParam;

typedef struct {
	DataParam data;
} RepBlobParam;

typedef struct {
	uint8 cmd;
} RepCmdParam;

typedef struct {
	uint8 type;
	uint32 count;
} RepProduct;

typedef struct {
	uint8 count;
//	RepProduct data[0];
} RepProductParam;

typedef struct {
	uint16 id;
	uint8 rssi;
} RepRouter;

typedef struct {
	uint8 count;
//	RepRouter data[0];
} RepRouterParam;

typedef struct {
	int32 x;		//position x
	int32 y;		//position y
	int32 z;		//position z
	int32 c;		//course
	int32 ax;		//accelerate raw x
	int32 ay;		//accelerate raw y
	int32 az;		//accelerate raw z
	int32 gx;		//gyroscope raw x
	int32 gy;		//gyroscope raw y
	int32 gz;		//gyroscope raw z
	uint8 gps[GPS_PARAM_LEN];  //gps??????:????,????,??(DDDMM.MMMMM)
	uint8 retCode;	//??vin??,0x0: success; 0xff: error; 0x1: ok; 0x2:cancel
} RepInNaviParam; 	//Inertia Navigation

typedef struct {
	uint8 type;      //  CAR_IN： 1, CAR_STAY_IN： 2, CAR_OUT：3
	uint16 id;		//上报压线车辆id
	uint8 rssi;		//上报压线车辆rssi
} RepMAGParam;

typedef struct {
	uint16 st;
} RepTempParam;

typedef struct {
	uint16 srh;
} RepRHParam;

typedef struct {

  uint8 Workstate;
  uint8 Location;
  uint8 Lockstate;
} RepZigbeeOtherParam;

typedef struct {
	char lon[12]; //ddd.dddddddd	3d.8d
	char lat[11]; //dd.dddddddd  	2d.8d
} RepGPSParam;

typedef struct {
	char phoneNum[11];
} RepPhoneNumParam;

typedef struct {
	uint32 rad;
} RepRadParam;

typedef struct {
  uint8 moveFlag;
} RepAccelParam1;

typedef struct {
	uint8 type;
	uint8 count;
//	DataUnit unit[0];
} ReportBody;

#pragma pack()


ReportBody* zproto_report_body(ZProto *proto);

void zproto_report_append_unit(ZProto *proto, ReportType type, const DataUnit *unit);

int zproto_report_unit_count(ZProto *proto);

DataUnit* zproto_report_unit(ZProto *proto, int i);


#endif /* ZPROTO_REPORT_H_ */
