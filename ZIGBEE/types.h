/*
 * types.h
 *
 *  Created on: Sep 23, 2014
 *      Author: toto
 */

#ifndef TYPES_H_
#define TYPES_H_

/** Signed 32bit integer. */
typedef int int32;

/** Unsigned 32bit integer. */
typedef unsigned int uint32;
typedef unsigned int u32;

/** Signed 16bit integer. */
typedef short int16;

/** Unsigned 16bit integer. */
typedef unsigned short uint16;
typedef unsigned short u16;

/** Signed 8bit integer. */
typedef signed char int8;

/** Unsigned 8bit integer. */
typedef unsigned char	uint8;
typedef unsigned char	u8;

/** Boolean. */
//typedef int bool;

typedef long long int64;
typedef unsigned long long uint64;

#define FALSE 0
#define TRUE 1
#define false 0
#define true 1

#define ASSERT(x)

#define FOURCC(C1, C2, C3, C4) (C4<<24 | C3<<16 | C2<<8 | C1)

#pragma pack(1)

typedef struct
{
	uint32 sec;
	uint32 msec;
} DateTime;

typedef struct
{
	uint32 address;
	uint16 port;
} SockaddrIn;

typedef struct
{
	uint8 length;
//	uint8 data[0];
} DataParam;

typedef struct {
	uint32 desc; // description in fourcc
	uint8 length;
//	uint8 param[0];
} DataUnit;

#pragma pack()

#endif /* TYPES_H_ */
