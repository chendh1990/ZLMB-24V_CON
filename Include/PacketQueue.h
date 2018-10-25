#ifndef __PACKET_QUEUE_H__
#define __PACKET_QUEUE_H__
#include "Typedef.h"

#ifndef __PACKET_QUEUE_C__
	#define PACKET_QUEUE_EXTERN	extern
#else
	#define PACKET_QUEUE_EXTERN
#endif

/***********************�궨��****************************/
#define PACKET_QUEUE_MAX		20


/***********************���ݽṹ����************************/

typedef struct
{
	uint8 buffer[PACKET_QUEUE_MAX];
#if PACKET_QUEUE_MAX > 255
		uint16 in;
		uint16 out;
		uint16 size;
#else
		uint8 in;
		uint8 out;
		uint8 size;
#endif

}PacketQueue_t;


/********************ȫ�ֱ��������붨��***********************/
PACKET_QUEUE_EXTERN PacketQueue_t XDATA g_PacketQueue;


/***************************��������***************************/
PACKET_QUEUE_EXTERN void QueueInit(void);
PACKET_QUEUE_EXTERN uint8 QueuePost(uint8 dat);
PACKET_QUEUE_EXTERN uint8 QueuePend(uint8 *dat);
PACKET_QUEUE_EXTERN uint8 QueueSize(void);
#endif

