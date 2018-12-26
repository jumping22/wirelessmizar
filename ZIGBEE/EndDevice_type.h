#ifndef _ENDDEVICE_TYPE_H
#define _ENDDEVICE_TYPE_H

#include <stdint.h>




//匿名联合体
#pragma anon_unions

typedef union {
	struct {
		uint16_t distance;
		uint16_t dummy;
	};
	uint8_t buf[4];
}IRdistance_struct;		//infrared distance measurement 红外测距

typedef union {
	struct {
		uint32_t key;
	};
	struct {
		uint8_t cmd;
		uint8_t dummy[3];
	};
	uint8_t buf[4];
}IRremote_struct;			//红外遥控 按键码

typedef union {
	struct {
		uint16_t analog;
		uint16_t dummy;
	};
	uint8_t buf[4];
}PRS_struct, Potentiometer_struct;		//光敏电阻		电位器

typedef union {
	struct {
		uint16_t temperature;
		uint16_t humidity;
	};
	uint8_t buf[4];
}DHT11_struct;



typedef union {
	struct {
		uint16_t digital;
		uint16_t dummy;
	};
	uint8_t buf[4];
}PIR_struct, BigKey_struct;


//#define Sensor_ZProto_to_sensorData( dev )		((EndDeviceSensor_s*)(dev)->recvbuf)

#endif
