#ifndef __SYS_H__
#define __SYS_H__

#ifndef __SYS_C__
	#define SYS_EXTERN	extern
#else
	#define SYS_EXTERN
#endif

/**************************宏定义****************************/
#define SERVER_DISABLE		0xff
#define SERVER_TO_RUN		0x00


#define	WINDOW_CH_MAX		1

/**********************数据结构定义**************************/

typedef enum {
	SYSTEM_NULL_STATUS = 0,
	SYSTEM_INIT_STATUS,	
	SYSTEM_RUN_STATUS,
}System_Status_t;
	
typedef enum {
	SYS_KEY_SCAN_SERVER_ID = 0,
	SYS_LED_SHOW_SERVER_ID,
	SYS_TIMER_UNIT_SERVER_ID,
	SYS_TEST_SERVER_ID,
	SYS_MAX_SERVER_ID,
}System_Server_ID_t;


typedef enum{
	SYS_MSG_NULL_ID = 0X00,
	SYS_MSG_KEY_ID,
	SYS_MSG_WINDOW_ID,
	SYS_MSG_WIFI_ID,
	SYS_MSG_MAX_ID,
 }System_Msg_t;


typedef struct{
	sysServerTO_t	TO;
	sysServer_t		server;
}System_Server_t;

typedef struct{
	uint8 SystemLastStatus;
	uint8 SystemCurrentStatus;
	uint8 SystemNextStatus;
}System_Var_t;


typedef	struct
{
	uint8 opened: 1;
	uint8 closed: 1;
	uint8 opening: 1;
	uint8 closing: 1;
	uint8 pause: 1;
	uint8 wait: 1;
}BitState_t;

typedef union{
	uint8 sta;
	BitState_t BitState;
}RunState_t;
/**********************全局变量定义与定义***********************/
SYS_EXTERN XDATA System_Var_t g_SystemVar;	
 
SYS_EXTERN XDATA RunState_t g_RunState[WINDOW_CH_MAX];


/**********************函数声明**************************/
SYS_EXTERN void SystemVarInit(void);
SYS_EXTERN void SystemStatusMachine(unsigned char SystemStatus);
SYS_EXTERN void SystemServerRun(void);

#endif

