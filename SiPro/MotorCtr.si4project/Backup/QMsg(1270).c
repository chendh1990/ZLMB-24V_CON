#define __QMSG_C__
#include "Msg.h"
#include "Common.h"
#include "Log.h"

uint8 QMsgInit(MSG_Q_t *const this, const MSG_t *const msgBuffer, uint8 cap)
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
uint8 QMsgPost(MSG_Q_t *const this, MSG_t msg)
{
	if(!this)
	{
		LOG("[QMsgPost] this is null\r\n");
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
		LOG("QMsgPost Qmsgid=%d, Qparam=%d\r\n", (uint16)(this->pStart[this->In-1].msgID), (uint16)(this->pStart[this->In-1].Param));
		LOG("QMsgPost msgid=%d, param=%d\r\n", (uint16)(msg.msgID), (uint16)(msg.Param));
		LOG("QMsgPost QSize:%d\r\n", (uint16)(this->Size));
		LOG("QMsgPost QIn:%d\r\n\r\n", (uint16)(this->In));
		return 0;
	}
	return 2;
}

uint8 QMsgPend(   MSG_Q_t *const this, MSG_t *const pMsg)
{
	MSG_t msg;

	if(!this)
	{
		LOG("[QMsgPend] this is null\r\n");
		return 1;
	}
	if(!pMsg)
	{
		return 2;
	}
	if(this->Size > 0)
	{
		if(this->Out >= this->Capacity)
		{
			this->Out = 0;
		}
		pMsg->msgID = this->pStart[this->Out].msgID;
		pMsg->Param = this->pStart[this->Out].Param;
		msg.msgID = this->pStart[this->Out].msgID;
		msg.Param = this->pStart[this->Out].Param;
		this->Out++;
		ENTER_CRITICAL();
		this->Size--;
		EXIT_CRITICAL();
		LOG("QMsgPend: this = %x\r\n", (uint16)this);
		LOG("QMsgPend:&g_Qmsg = %x\r\n", (uint16)(&g_QMsg));
		LOG("QMsgPend Qmsgid=%d, Qparam=%d\r\n", (uint16)(this->pStart[this->Out-1].msgID), (uint16)(this->pStart[this->Out-1].Param));
		LOG("QMsgPend msgid=%d, param=%d\r\n", (uint16)(pMsg->msgID), (uint16)(pMsg->Param));
		LOG("QMsgPend interal msgid=%d, param=%d\r\n", (uint16)(msg.msgID), (uint16)(msg.Param));
		LOG("QMsgPend QSize:%d\r\n", (uint16)(this->Size));
		LOG("QMsgPend QOut:%d\r\n\r\n", (uint16)(this->Out));
		return 0;
	}
	return 3;
}

uint8 QMsgSize(const MSG_Q_t *const this)
{
	return this->Size;
}
uint8 QMsgCapacity(const MSG_Q_t *const this)
{
	return this->Capacity;
}

