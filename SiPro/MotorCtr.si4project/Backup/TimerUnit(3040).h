#ifndef __TIMER_UNIT_H__
#define __TIMER_UNIT_H__
#include "Typedef.h"
#include "Msg.h"

#ifndef __TIMER_UNIT_C__
	#define TIMER_UNIT_EXTERN	extern
#else
	#define TIMER_UNIT_EXTERN
#endif

/***********************�궨��****************************/
#define TIMER_UNIT_SERVER_TICK	10

#define TIMER_TABLE_MAX			6

#define  TEST0_TIMER			"H0"
#define  TEST1_TIMER			"H1"

/***********************���ݽṹ����************************/
typedef struct{
	MSG_Q_t	*pQmsg;
	MSG_t msg;
	uint16 TO;
	uint8 *pName;
}Timer_t;

typedef struct{
	Timer_t TServer[TIMER_TABLE_MAX];
	uint8	Total;
}TimerServer_t;

/********************ȫ�ֱ��������붨��***********************/
TIMER_UNIT_EXTERN XDATA TimerServer_t g_TimerServer;


/***************************��������***************************/
TIMER_UNIT_EXTERN sysServerTO_t SysTimerUnitServer(void);
TIMER_UNIT_EXTERN void TimerUnitInit(TimerServer_t *this);
TIMER_UNIT_EXTERN void TimerUnitDel(TimerServer_t *this, unsigned char *pname);
TIMER_UNIT_EXTERN void TimerUnitAdd(TimerServer_t *this, uint8 *pname, MSG_Q_t *pqmsg, MSG_t *pmsg, uint16 TO);

#endif
