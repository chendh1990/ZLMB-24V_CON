#ifndef __LED_H__
#define __LED_H__

#include "Peripheral.h"
#include "Typedef.h"


#ifndef __LED_C__
	#define LED_EXTERN	extern
#else
	#define LED_EXTERN
#endif


/**************************宏定义****************************/
#define LED_SHOW_SERVER_TICK		10

#define LED_FLASH_ALWAYS			0xff

/***********************数据结构定义************************/
typedef struct{
	uint16 timerOn;		//开时间
	uint16 timerOff;	//关时间
	uint16 cnt;			//计数器
	uint16 count;		//闪烁次数
}Led_Status_t;

typedef enum{
	LED_OPEN_ID = 0,
	LED_CLOSE_ID,
	LED_PAUSE_ID,
	LED_WIFI_ID,
	LED_MAX_ID
}Led_Id_t;

/**********************全局变量定义与定义***********************/



/**********************函数声明**************************/
LED_EXTERN void LedInit(void);

LED_EXTERN sysServerTO_t LedShowServer(void);

LED_EXTERN void LedSetFlash(uint8 id, uint8 count, uint16 on_time, uint16 off_time);    

LED_EXTERN void LedSetLevel(uint8 id, uint8 level, uint8 flag);

#endif
