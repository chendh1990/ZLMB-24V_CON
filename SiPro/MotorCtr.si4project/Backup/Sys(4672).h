#ifndef __SYS_H__
#define __SYS_H__

#ifndef __SYS_C__
	#define SYS_EXTERN	extern
#else
	#define SYS_EXTERN
#endif

/**************************�궨��****************************/
#define SERVER_DISABLE		0xff
#define SERVER_TO_RUN		0x00
/**********************���ݽṹ����**************************/

typedef struct{
	uint8 SystemLastStatus;
	uint8 SystemCurrentStatus;
	uint8 SystemNextStatus;
}System_Var_t;

typedef struct{
	uint8 occ; 
} System_Param_t;

typedef enum {
	SYSTEM_NULL_STATUS = 0,
	SYSTEM_ON_STATUS,	
	SYSTEM_OFF_STATUS,
	SYSTEM_PEND_STATUS
}System_Status_t;
	
typedef enum {
	SYS_KEY_SERVER_ID = 0,
	SYS_TIMER_UNIT_SERVER_ID,
	SYS_MAX_SERVER_ID
}System_Server_ID_t;



typedef struct{
	sysServerTO_t	TO;
	sysServer_t		server;
}System_Server_t;

/**********************ȫ�ֱ��������붨��***********************/
SYS_EXTERN System_Var_t g_SystemVar;	
 


/**********************��������**************************/
SYS_EXTERN void SystemVarInit(void);
SYS_EXTERN void SystemStatusMachine(unsigned char SystemStatus);
SYS_EXTERN void SystemServerRun(void);

#endif

