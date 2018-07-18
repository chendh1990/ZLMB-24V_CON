#define __QMSG_C__
#include "Msg.h"
#include "Common.h"

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
		return 0;
	}
	return 2;
}

uint8 QMsgPend(MSG_Q_t *this, MSG_t *pMsg)
{
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
		if(this->Out >= this->Capacity)
		{
			this->Out = 0;
		}
		pMsg->msgID = this->pStart[this->Out].msgID;
		pMsg->Param = this->pStart[this->Out].Param;
		this->Out++;
		ENTER_CRITICAL();
		this->Size--;
		EXIT_CRITICAL();
		return 0;
	}
	return 3;
}

uint8 QMsgSize(MSG_Q_t *this)
{
	return this->Size;
}
