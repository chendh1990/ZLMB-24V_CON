#ifndef __QMSG_H__
#define __QMSG_H__
#include "Typedef.h"

#ifndef __QMSG_C__
	#define QMSG_EXTERN	extern
#else
	#define QMSG_EXTERN
#endif

/**************************宏定义****************************/
#define MSG_NUM_MAX		20

/**********************数据结构定义**************************/
typedef struct{
	uint16 msgID;
	uint16 Param;
} MSG_t;

typedef struct{
	MSG_t *pStart;
	MSG_t *pEnd;
	uint8 In;
	uint8 Out;
	uint8 Size;
	uint8 Capacity;
} MSG_Q_t;	

/********************全局变量声明与定义************************/

QMSG_EXTERN	XDATA MSG_t	g_MsgArray[MSG_NUM_MAX];
QMSG_EXTERN	XDATA MSG_Q_t g_QMsg;
QMSG_EXTERN	XDATA MSG_t g_Msg;


/************************函数声明******************************/
QMSG_EXTERN uint8 QMsgInit(MSG_Q_t XDATA *const this, const MSG_t XDATA *const msgBuffer, uint8 cap);
QMSG_EXTERN uint8 QMsgPostSimple(MSG_Q_t XDATA *const this, uint16 XDATA msgID, uint16 XDATA param);
QMSG_EXTERN uint8 QMsgPost(MSG_Q_t XDATA *const this, MSG_t msg);
QMSG_EXTERN uint8 QMsgPend(MSG_Q_t XDATA *const this, MSG_t XDATA *const pMsg);
QMSG_EXTERN uint8 QMsgSize(const MSG_Q_t XDATA *const this);
QMSG_EXTERN uint8 QMsgCapacity(const MSG_Q_t XDATA *const this);

#endif

