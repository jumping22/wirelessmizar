#include "usrLib.h"
#include <string.h>

/*        定位字符串
*入口：src：被查找缓冲起始地址
*      str: 要查找的字符串
*      len: 被查找缓冲的长度
*出口：0:失败  其他值：查找到后的偏移量
*/
uint16_t  LocateString(const char *const src, const char *const str, uint16_t len)
{
  uint16_t  i, off;

  i = strlen(str);
  off = 0;
  while(i <= len) 
  {
    if (memcmp(&src[off], &str[0], i) == 0) 
    {
      return (off+i);
    }
    off++;
    len--;
  }
  return 0;
}

/*      短整形转换成字符串
*入口：str: 目标缓冲
*      val: 短整形值
*出口：无
*/
void  ShortIntToString(uint8_t *str, uint16_t val)
{
  if ((val/10000) != 0){
    *str++ = (val/10000) + '0';
    *str++ = ((val%10000)/1000) + '0';
    *str++ = ((val%1000)/100) + '0';
    *str++ = ((val%100)/10) + '0';
    *str++ = (val%10) + '0';
  }else if((val/1000) != 0){
    *str++ = (val/1000) + '0';
    *str++ = ((val%1000)/100) + '0';
    *str++ = ((val%100)/10) + '0';
    *str++ = (val%10) + '0';
  }else if((val/100) != 0){
    *str++ = (val/100) + '0';
    *str++ = ((val%100)/10) + '0';
    *str++ = (val%10) + '0';
  }else if((val/10) != 0){
    *str++ = (val/10) + '0';
    *str++ = (val%10) + '0';
  }else{
    *str++ = (val%10) + '0';
  }
  *str = '\0';
}
