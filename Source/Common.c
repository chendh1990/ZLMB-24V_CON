#define __COMMON_C__
#include "Common.h"


uint8 getCheckSum(uint8 const * const pBuffer, uint8 length)
{
	uint8 i;
	uint8 checkSum = 0;

	for(i = 0; i < length; i ++)
	{
		checkSum += pBuffer[i];
	}
	return checkSum;
}
uint8 getHex(uint8 dat)
{
	uint8 ret;
	if(dat <= 9)
	{
		ret = dat + '0';
	}
	else
	{
		ret = dat - 10 + 'A';
	}
	return ret;
}
/*****************************************************************************
�������� : hex_to_bcd
�������� : hexתbcd
������� : Value_H:���ֽ�/Value_L:���ֽ�
���ز��� : bcd_value:ת����ɺ�����
*****************************************************************************/
uint8 hex_to_bcd(uint8 Value_H, uint8 Value_L)
{
  uint8 bcd_value;
  
  if((Value_H >= '0') && (Value_H <= '9'))
    Value_H -= '0';
  else if((Value_H >= 'A') && (Value_H <= 'F'))
    Value_H = Value_H - 'A' + 10;
  else if((Value_H >= 'a') && (Value_H <= 'f'))
    Value_H = Value_H - 'a' + 10;
   
  bcd_value = Value_H & 0x0f;
  
  bcd_value <<= 4;
  if((Value_L >= '0') && (Value_L <= '9'))
    Value_L -= '0';
  else if((Value_L >= 'A') && (Value_L <= 'F'))
    Value_L = Value_L - 'a' + 10;
  else if((Value_L >= 'a') && (Value_L <= 'f'))
    Value_L = Value_L - 'a' + 10;
  
  bcd_value |= Value_L & 0x0f;

  return bcd_value;
}

/*****************************************************************************
�������� : my_strlen
�������� : ���ַ�������
������� : src:Դ��ַ
���ز��� : len:���ݳ���
*****************************************************************************/
uint16 my_strlen(uint8 const *str)  
{
  uint16 len = 0;
  if(str == NULL)
  { 
    return 0;
  }
  
  for(len = 0; *str ++ != '\0'; )
  {
    len ++;
  }
  
  return len;
}
#if 0
/*****************************************************************************
�������� : my_memset
�������� : ��src��ָ�ڴ������ǰcount���ֽ����ó��ַ�c
������� : src:Դ��ַ
           ch:�����ַ�
           count:�������ݳ���
���ز��� : src:���ݴ�������Դ��ַ
*****************************************************************************/
void *my_memset(void *src, uint8 ch, uint16 count)
{
  uint8 *tmp = (uint8 *)src;
  
  if(src == NULL)
  {
    return NULL;
  }
  
  while(count --)
  {
    *tmp ++ = ch;
  }
  
  return src;
}
#endif
/*****************************************************************************
�������� : mymemcpy
�������� : �ڴ濽��
������� : dest:Ŀ���ַ
           src:Դ��ַ
           count:���ݿ�������
���ز��� : src:���ݴ�������Դ��ַ
*****************************************************************************/
void *my_memcpy(void *dest, const void *src, uint16 count)  
{  
  uint8 *pdest = (uint8 *)dest;  
  const uint8 *psrc  = (const uint8 *)src;  
  uint16 i;
  
  if(dest == NULL || src == NULL)
  { 
    return NULL;
  }
  
  if((pdest <= psrc) || (pdest > psrc + count))
  {  
    for(i = 0; i < count; i ++)
    {  
      pdest[i] = psrc[i];  
    }  
  }
  else
  {
    for(i = count; i > 0; i --)
    {  
      pdest[i - 1] = psrc[i - 1];  
    }  
  }  
  
  return dest;  
}