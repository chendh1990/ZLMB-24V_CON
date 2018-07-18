#ifndef __QMSG_H__
#define __QMSG_H__
#include "Typedef.h"

#ifndef __QMSG_C__
	#define QMSG_EXTERN	extern
#else
	#define QMSG_EXTERN
#endif

/**************************�궨��****************************/
#define MSG_NUM_MAX		3

/**********************���ݽṹ����**************************/
typedef struct{
	uint8 msgID;
	uint8 Param;
} MSG_t;

typedef struct{
	MSG_t *pStart;
	MSG_t *pEnd;
	uint8 In;
	uint8 Out;
	uint8 Size;
	uint8 Capacity;
} MSG_Q_t;	

/********************ȫ�ֱ��������붨��************************/

QMSG_EXTERN   MSG_t	g_Msg[MSG_NUM_MAX];
QMSG_EXTERN   MSG_Q_t g_QMsg;

/************************��������******************************/
QMSG_EXTERN uint8 QMsgInit(MSG_Q_t *const this, const MSG_t *const msgBuffer, uint8 cap);
QMSG_EXTERN uint8 QMsgPost(MSG_Q_t *const this, MSG_t msg);
QMSG_EXTERN uint8 QMsgPend(MSG_Q_t *const this, MSG_t *const pMsg);
QMSG_EXTERN uint8 QMsgSize(const MSG_Q_t *const this);
QMSG_EXTERN uint8 QMsgCapacity(const MSG_Q_t *const this);

#endif

