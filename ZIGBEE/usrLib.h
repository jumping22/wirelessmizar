#ifndef USR_LIB_H
#define USR_LIB_H

#include "stdint.h"

#pragma pack(1)
typedef struct
{
    uint8_t *Ptr;
    uint16_t Len;
}RxDataDef;
#pragma pack()







uint8_t IsTimOut(uint32_t tickStart, uint32_t delay);
uint16_t  LocateString(const char *const src, const char *const str, uint16_t len);
void  ShortIntToString(uint8_t *str, uint16_t val);

#endif

