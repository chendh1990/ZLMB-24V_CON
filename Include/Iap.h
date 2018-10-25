#ifndef __IAP_H__
#define __IAP_H__
#include "Typedef.h"


#ifndef __IAP_C__
	#define IAP_EXTERN	extern
#else
	#define IAP_EXTERN
#endif


/**************************宏定义****************************/

#define PAGE_SIZE				(128)

#define BYTE_WRITE_AP			(0x21)
#define BYTER_READ_AP			(0x00)
#define PAGE_ERASE_AP			(0x22)

/***********************数据结构定义************************/


/**********************全局变量定义与定义***********************/



/**********************函数声明**************************/
IAP_EXTERN void IAP_ByteWrite(uint16 byteAddr, uint8 dat);
IAP_EXTERN uint8 IAP_ByteRead(uint16 byteAddr);
IAP_EXTERN void IAP_PageWrite(uint16 pageAddr, const uint8 dat[], uint16 length);
IAP_EXTERN void IAP_PageRead(uint16 pageAddr, uint8 dat[], uint16 length);
IAP_EXTERN void IAP_PageErase(uint16 pageAddr);
IAP_EXTERN void IAP_PageEraseAllAppRom(void);
IAP_EXTERN void ProgramCONFIG(uint8 addr, uint8 value);
IAP_EXTERN void SoftResetLdRomStart(void);

#endif

