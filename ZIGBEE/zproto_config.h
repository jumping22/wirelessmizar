/*
 * zproto_config.h
 *
 *  Created on: Sep 26, 2014
 *      Author: toto
 */

#ifndef ZPROTO_CONFIG_H_
#define ZPROTO_CONFIG_H_

#include "zproto.h"

#pragma pack(1)

typedef enum {
	DEV_Mode_ST = 1,
	DEV_Mode_SZ,
	DEV_Mode_Z,
	DEV_Mode_DFST,
	DEV_Mode_DFINST,
	DEV_Mode_DFINSZ,
	DEV_Mode_WIFIST,
	DEV_Mode_DOORST,
	DEV_Mode_FSYST
} DeviceMode;

typedef enum {
	OPERATE_TYPE_GET = 1,
	OPERATE_TYPE_GET_RETURN,
	OPERATE_TYPE_SET,
	OPERATE_TYPE_ASK,
	OPERATE_TYPE_REINIT,
	OPERATE_TYPE_RESET,
	OPERATE_TYPE_CMD_REQ,
	OPERATE_TYPE_CMD_RSP,
	//OPERATE_TYPE_HEART_BEAT,  				//param is NULL
        OPERATE_TYPE_CMD_INFORM
} OperateType;

typedef enum {
	FSY_ROUTER_HOUSE = 1,
	FSY_ROUTER_HOUSE_HOLE,
	FSY_ROUTER_CAR_CABAIN,
	FSY_ROUTER_CAR_HEAD,
	FSY_ROUTER_CAR_TAIL
} FSYRouterType;

#define ZP_CONFIG_PARAM_ST 				FOURCC('C', 'P', 'S', 'T')
#define ZP_CONFIG_PARAM_SZ 				FOURCC('C', 'P', 'S', 'Z') // Zigbee模块
#define ZP_CONFIG_PARAM_PHONE_SZ	FOURCC('C', 'P', 'P', 'Z') // 手机Zigbee模块
#define ZP_CONFIG_PARAM_Z	 			  FOURCC('C', 'P', 'Z', 'B') // 工卡
#define ZP_CONFIG_PARAM_WZ				FOURCC('C', 'P', 'W', 'Z') // 腕带
#define ZP_CONFIG_PARAM_LZ				FOURCC('C', 'P', 'L', 'Z') // 资产标签
#define ZP_CONFIG_PARAM_EZ				FOURCC('C', 'P', 'E', 'Z') // 传感终端
#define ZP_CONFIG_PARAM_RL				FOURCC('C', 'P', 'O', 'T') // 放射源锁

#define ZP_CONFIG_PARAM_GS				FOURCC('C', 'P', 'G', 'S')
#define ZP_CONFIG_PARAM_GI				FOURCC('C', 'P', 'G', 'I')
#define ZP_CONFIG_PARAM_GC				FOURCC('C', 'P', 'G', 'C')

#define ZP_CONFIG_PARAM_DFST			FOURCC('C', 'D', 'S', 'T')
#define ZP_CONFIG_PARAM_DFSZ			FOURCC('C', 'D', 'S', 'Z')
#define ZP_CONFIG_PARAM_DFIT			FOURCC('C', 'D', 'I', 'T')
#define ZP_CONFIG_PARAM_DFIZ			FOURCC('C', 'D', 'I', 'Z')
#define ZP_CONFIG_SETTING 				FOURCC('C', 'T', 'S', 'S')
#define ZP_CONFIG_PARAM_SW 				FOURCC('C', 'T', 'S', 'W')

#define ZP_CONFIG_BS_REINIT 			FOURCC('C', 'B', 'R', 'I')
#define ZP_CONFIG_BS_RESET 				FOURCC('C', 'B', 'R', 'S')
#define ZP_CONFIG_BS_PRODUCT 			FOURCC('C', 'B', 'P', 'D')
#define ZP_CONFIG_BS_CMD				  FOURCC('C', 'B', 'C', 'D')
#define ZP_CONFIG_BS_NAME				  FOURCC('C', 'B', 'N', 'M')


#define ZP_CONFIG_BS_DHCP				  FOURCC('C', 'B', 'D', 'H')	
#define ZP_CONFIG_BS_COOR				  FOURCC('C', 'B', 'C', 'O')

#define ZP_CONFIG_INS_COCA				FOURCC('C', 'B', 'C', 'C')
#define ZP_CONFIG_PARAM_DOOR			FOURCC('C', 'C', 'P', 'D')

#define ZP_ELEC_DOOR_UNKNOW       FOURCC('Z', 'E', 'D', 'U')  //电子门默认状态
#define ZP_ELEC_DOOR_NEAR				  FOURCC('Z', 'E', 'D', 'N')	//电子门外部人靠近
#define ZP_ELEC_DOOR_OPEN				  FOURCC('Z', 'E', 'D', 'O')	//电子门外部人打开
#define ZP_ELEC_DOOR_CLOSE				FOURCC('Z', 'E', 'D', 'C')	//电子门外部人按键未开

//add by swd
#define ZP_CONFIG_STM32 				FOURCC('C', 'S', 'M', 'M')

typedef struct {
	uint8 workmode;
	uint16 id;
	uint8 mac[6];
	SockaddrIn addr;
	uint32 gw;
	uint16 peerId;
	uint8 peerMac[6];
	SockaddrIn peerAddr;
	uint16 freq;
} CfgRouterParam;

typedef struct {
	u8	workmode;
	u16 	id;
	u16     mac_head;
	u32     mac_tail;
	u32     ip;

	u16     port;
	u32     gw;
	u16	peerId;
	u16     macServer_head;
	u32     macServer_tail;
	u32     ipServer;
	u16     portServer;
	u8	frep;
	uint8_t resetFlag;
	uint8_t fiberEn;
	uint8_t areaAlarmEn;

} CfgWifiRouterParam;

typedef struct {
	u8		workmode;
	u16 	id;
	u16     mac_head;
	u32     mac_tail;
	u32     ip;

	u16     port;
	u32     gw;
	u16	peerId;
	u16     macServer_head;
	u32     macServer_tail;
	u32     ipServer;
	u16     portServer;
	u8	frep;
	u8 	level;
	u32	dataversion;
}CfgDoorRouterParam;

typedef struct {
	uint8 workmode;
	uint16 id;
	uint8 mac[6];
	SockaddrIn addr;
	uint32 gw;
	uint16 peerId;
	uint8 peerMac[6];
	SockaddrIn peerAddr;
	uint16 freq;
	uint8 fsyRouterType;   // see the enum 'FSYRouterType'
} CfgFSYRouterParam;

typedef struct {
	uint64 sn;
	uint8 index;
	uint16 id;
	uint16 panid;
	uint8 channel;
	uint16 freq;
	uint8 power;
	uint8 powersaving;
} CfgZigbeeParam;

typedef struct {
	uint64 sn;
	uint8 index;
	uint16 id;
	uint16 panid;
	uint8 channel;
	uint16 freq;
	uint8 power;
	uint8 powersaving;
	uint16 wakeTime;
} CfgZigbeeForPhoneParam;

typedef struct {
	uint8 level;
} CfgReinitParam;

typedef struct {
	uint8 type;
	uint32 count;
} CfgProduct;

typedef struct {
	uint8 count;
//	CfgProduct data[0];
} CfgProductParam;

typedef struct {
	uint8 cmd;
} CfgCmdParam;

typedef struct {
	int32 	cax;	//course
	int32 	cay;	//reserve
	int32	cox;	//x axis coordination
	int32	coy;	//y axis coordination
} CfgCOCAParam;

typedef struct {
	int8 enable;
	SockaddrIn defaultAddr;
} CfgDHCPParam;

typedef struct {
	int8 enable;
	int32 cx;
	int32 cy;
	int32 course;
} CfgCoorParam;

typedef struct {
	CfgRouterParam routerParam;
	CfgCoorParam coorParam;
} CfgDFRouterParam;

#define INS_COMMAND_NONE          0x0
#define INS_COMMAND_SET_POS       0x1
#define INS_COMMAND_CORR_POS      0x2

typedef struct {
  uint16 id;
  uint16 freq;
  uint8  command;     //0x0: 什么都不做；0x1：配置车位；0x2：校准车位
  uint8  position[8];
  uint8  realPos[8];
} CfgDFINSParam;

typedef enum {
  GSENSOR_POWER_DOWN   =		0x00,
  GSENSOR_LOW_POWER    =		0x01,
  GSENSOR_NORMAL	   =		0x02
} GSENSOR_Mode_t;

typedef enum {  
  GSENSOR_ODR_1Hz		        	=		0x01,
  GSENSOR_ODR_10Hz              	=		0x02,
  GSENSOR_ODR_25Hz		        	=		0x03,
  GSENSOR_ODR_50Hz		        	=		0x04,
  GSENSOR_ODR_100Hz		        	=		0x05,
  GSENSOR_ODR_200Hz		        	=		0x06,
  GSENSOR_ODR_400Hz		        	=		0x07,
  GSENSOR_ODR_1620Hz_LP		        =		0x08,
  GSENSOR_ODR_1344Hz_NP_5367HZ_LP   =		0x09
} GSENSOR_ODR_t;

#define GSENSOR_AXIS_X_ENABLE 0x01
#define GSENSOR_AXIS_Y_ENABLE 0x02
#define GSENSOR_AXIS_Z_ENABLE 0x04

typedef enum {
  GSENSOR_FULLSCALE_2                   =               0x00,
  GSENSOR_FULLSCALE_4                   =               0x01,
  GSENSOR_FULLSCALE_8                   =               0x02,
  GSENSOR_FULLSCALE_16                  =               0x03
} GSENSOR_Fullscale_t;

typedef struct {
	GSENSOR_Mode_t workmode;
	GSENSOR_ODR_t ORD;
	uint8 axis; 				// 位标识 X,Y,Z三轴开关
	GSENSOR_Fullscale_t fullScale;
} CfgGSensorParam;  			// DESC ZP_CONFIG_PARAM_GS

#define GSENSOR_XLIE_ENABLE 0x01
#define GSENSOR_XHIE_ENABLE 0x02
#define GSENSOR_YLIE_ENABLE 0x04
#define GSENSOR_YHIE_ENABLE 0x08
#define GSENSOR_ZLIE_ENABLE 0x10
#define GSENSOR_ZHIE_ENABLE 0x20

typedef enum {
	GSENSOR_INT_MODE_OR				= 		0x00,
	GSENSOR_INT_MODE_6D_MOVEMENT	= 		0x01,
	GSENSOR_INT_MODE_AND			= 		0x02,
	GSENSOR_INT_MODE_6D_POSITION	= 		0x03
} GSENSOR_IntMode_t;

typedef struct {
	uint8 int1Enable;
	uint8 int1HL_Active; 		// 0: interrupt active high 	1:interrupt active low
	uint8 int1Cfg;				// 位标识 X,Y,Z三轴High-Low 中断开关
	GSENSOR_IntMode_t int1Mode;
	uint8 int1Ths; 				// 0 ~ 127 一个单位大小为 FullScale/128
	uint8 int1Duration; 		// 0 ~ 127 一个单位大小为 1/ODR
} CfgGSensorInt1Param;  		//DESC ZP_CONFIG_PARAM_GI

#define GSENSOR_CLICK_XS_ENABLE		0x01
#define GSENSOR_CLICK_XD_ENABLE 	0x02
#define GSENSOR_CLICK_YS_ENABLE		0x04
#define GSENSOR_CLICK_YD_ENABLE		0x08
#define GSENSOR_CLICK_ZS_ENABLE		0x10
#define GSENSOR_CLICK_ZD_ENABLE     0x20

typedef struct {
	uint8 int2Enable;
	uint8 int2HL_Active; 		// 0: interrupt active high 	1:interrupt active low
	uint8 int2ClickCfg;
	uint8 int2ClickThs; 		// 0 ~ 127 一个单位大小为 FullScale/128	
	uint8 int2ClickTimeLimit;  	// 0 ~ 127 一个单位大小为 1/ODR
	uint8 int2ClickLatency;    	// 0 ~ 255 一个单位大小为 1/ODR
	uint8 int2ClickTimeWindow; 	// 0 ~ 255 一个单位大小为 1/ODR
} CfgGSensorInt2Param; 			// DESC ZP_CONFIG_PARAM_GC

typedef struct {
	uint8 devMode; // DeviceMode
	uint8 opType; // OperateType
//	DataUnit unit[0];
} ConfigBody;

typedef enum{
  G3_REPORT_NOMAL = 0,
  G3_REPORT_FAST,
  G3_REPOET_SLOW,
  G3_REPOET_TOO_SLOW,
}G3ReportModeDef;

typedef struct{
  uint8 workState;        //0: 正常工作模式; 1:为节能模式
  uint8 bdState;          //0: 北斗关; 1:  北斗开
  uint8 gsensorState;     //0: gsensor关; 1:  gsensor开
  uint8 zigbeeReportInv;  //zigbee上报频率，单位S
  uint8 g3ReportMode;     //3G上报频率模式， 
}CfgRadLockPara;

typedef struct{
  uint8 IP[4];
  uint16 Port;
}CfgIPPORT;
#pragma pack()


ConfigBody* zproto_config_body(ZProto *proto);

int zproto_config_dev_mode(ZProto *proto);

int zproto_config_op_type(ZProto *proto);

DataUnit* zproto_config_unit(ZProto *proto);

extern uint8_t getIpPortFlag;

extern char tcpSendCmd[32];
extern char tcpConnectCmd[64];

#endif /* ZPROTO_CONFIG_H_ */
