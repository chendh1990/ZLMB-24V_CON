#define __QMSG_C__
#include "Msg.h"
#include "Common.h"
#include "Log.h"

uint8 QMsgInit(MSG_Q_t *this, const MSG_t *msgBuffer, uint8 cap)
{
	if(!this)
	{
		return 1;
	}
	if(!msgBuffer)
	{
		return 2;
	}
	if(cap == 0)
	{
		return 3;
	}
	this->pStart = msgBuffer;
	this->pEnd = msgBuffer + cap;
	this->In = 0;
	this->Out = 0;
	this->Size = 0;
	this->Capacity = cap;
	return 0;
}
uint8 QMsgPost(MSG_Q_t *this, MSG_t msg)
{
	if(!this)
	{
		return 1;
	}
	if(this->Size < this->Capacity)
	{
		if(this->In >= this->Capacity)
		{
			this->In = 0;
		}
		this->pStart[this->In].msgID = msg.msgID;
		this->pStart[this->In].Param = msg.Param;
		this->In++;
		ENTER_CRITICAL();
		this->Size++;
		EXIT_CRITICAL();
		LOG("QMsgPost: this = %x\r\n", this);
		LOG("QMsgPost:&g_Qmsg = %x\r\n", &g_QMsg);
		LOG("QMsgPost Qmsgid=%d, Qparam=%d\r\n", this->pStart[this->In-1].msgID, this->pStart[this->In-1].Param);
		LOG("QMsgPost msgid=%d, param=%d\r\n", msg.msgID, msg.Param);
		LOG("QMsgPost QSize:%d\r\n", (uint16)(this->Size));
		LOG("QMsgPost QIn:%d\r\n\r\n", (uint16)(this->In-1));
		return 0;
	}
	return 2;
}

uint8 QMsgPend(  MSG_Q_t XDATA *this, MSG_t *pMsg)
{
	XDATA MSG_Q_t *pq = this;	
	LOG("--QMsgPend: pq  = %x\r\n", pq );
	LOG("--QMsgPend: this = %x\r\n", this);
//	LOG("--QMsgPend:&g_Qmsg = %x\r\n", &g_QMsg);
//	LOG("--QMsgPend Qmsgid=%d, Qparam=%d\r\n", this->pStart[this->Out].msgID, this->pStart[this->Out].Param);
//	LOG("--QMsgPend msgid=%d, param=%d\r\n", pMsg->msgID, pMsg->Param);
	LOG("--QMsgPend QSize:%d\r\n", (uint16)(this->Size));
	LOG("--QMsgPend QOut:%d\r\n\r\n", (uint16)(this->Out));
	if(!this)
	{
		return 1;
	}
	if(!pMsg)
	{
		return 2;
	}
	if(this->Size > 0)
	{
	LOG("--1QMsgPend QOut:%d\r\n\r\n", (uint16)(this->Out));
		if(this->Out >= this->Capacity)
		{
			this->Out = 0;
		}
		LOG("--2QMsgPend QOut:%d\r\n\r\n", (uint16)(this->Out));
		pMsg->msgID = this->pStart[this->Out].msgID;
		pMsg->Param = this->pStart[this->Out].Param;
		LOG("--3QMsgPend QOut:%d\r\n\r\n", (uint16)(this->Out));
		this->Out++;
		LOG("--4QMsgPend QOut:%d\r\n\r\n", (uint16)(this->Out));
		ENTER_CRITICAL();
		this->Size--;
		EXIT_CRITICAL();
		LOG("QMsgPend: this = %x\r\n", this);
		LOG("QMsgPend:&g_Qmsg = %x\r\n", &g_QMsg);
		LOG("QMsgPend Qmsgid=%d, Qparam=%d\r\n", this->pStart[this->Out-1].msgID, this->pStart[this->Out-1].Param);
		LOG("QMsgPend msgid=%d, param=%d\r\n", pMsg->msgID, pMsg->Param);
		LOG("QMsgPend QSize:%d\r\n", (uint16)(this->Size));
		LOG("QMsgPend QOut:%d\r\n\r\n", (uint16)(this->Out-1));
		return 0;
	}
	return 3;
}

uint8 QMsgSize(MSG_Q_t *this)
{
	return this->Size;
}
