#include "usrLib.h"
#include <string.h>

/*        ��λ�ַ���
*��ڣ�src�������һ�����ʼ��ַ
*      str: Ҫ���ҵ��ַ���
*      len: �����һ���ĳ���
*���ڣ�0:ʧ��  ����ֵ�����ҵ����ƫ����
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

/*      ������ת�����ַ���
*��ڣ�str: Ŀ�껺��
*      val: ������ֵ
*���ڣ���
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
