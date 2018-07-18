#define __TIMER_UNIT_C__
#include "TimerUnit.h"
#include "Log.h"

sysServerTO_t SysTimerUnitServer(void)
{
	uint8 i;
	for (i = 0; i < g_TimerServer.Total; i++)
	{
		if(g_TimerServer.TServer[i].TO > 0)
		{
			g_TimerServer.TServer[i].TO--;
		}
		else
		{
			if(g_TimerServer.TServer[i].pName[0] == 'H')
			{
				QMsgPost(g_TimerServer.TServer[i].pQmsg, g_TimerServer.TServer[i].msg);
			}
			if(i != g_TimerServer.Total - 1)
			{
				g_TimerServer.TServer[i].pName = g_TimerServer.TServer[g_TimerServer.Total-1].pName;
				g_TimerServer.TServer[i].pQmsg = g_TimerServer.TServer[g_TimerServer.Total-1].pQmsg;
				g_TimerServer.TServer[i].msg.msgID = g_TimerServer.TServer[g_TimerServer.Total-1].msg.msgID;
				g_TimerServer.TServer[i].msg.Param = g_TimerServer.TServer[g_TimerServer.Total-1].msg.Param;
				g_TimerServer.TServer[i].TO = g_TimerServer.TServer[g_TimerServer.Total-1].TO;
			}
			if(g_TimerServer.Total != 0)
			{
				g_TimerServer.Total--;
			}
		}
	}
	return TIMER_UNIT_SERVER_TICK;
}
void TimerUnitInit(TimerServer_t *this)
{
	if(!this)
	{
		return ;
	}
	this->Total = 0;
}

void TimerUnitDel(TimerServer_t *this, unsigned char *pname)
{
	uint8 i;
	for(i=0; i < this->Total; i++)
	{	
		if((this->TServer->pName[0] == pname[0]) && (this->TServer->pName[1] == pname[1]))
		{
			if(i != this->Total - 1)
			{
				this->TServer[i].pName = this->TServer[this->Total-1].pName;
				this->TServer[i].pQmsg = this->TServer[this->Total-1].pQmsg;
				this->TServer[i].msg.msgID = this->TServer[this->Total-1].msg.msgID;
				this->TServer[i].msg.Param = this->TServer[this->Total-1].msg.Param;
				this->TServer[i].TO = this->TServer[this->Total-1].TO;
			}
			if(this->Total != 0)
			{
				this->Total--;
			}
		}
	}
}

void TimerUnitAdd(TimerServer_t *this, uint8 *pname, MSG_Q_t *pqmsg, MSG_t *pmsg, uint16 TO)
{
	LOG("\t\tTimerUnitAdd msgid:%d param:%d\r\n", pmsg->msgID, pmsg->Param);
	if((!this) || (!pname) || (!pmsg))
	{
		return ;
	}
	if(this->Total >= TIMER_TABLE_MAX)
	{
		return ;
	}
	else
	{
		this->TServer[this->Total].pName = pname;
		this->TServer[this->Total].TO = TO;
		this->TServer[this->Total].pQmsg = pqmsg;
		this->TServer[this->Total].msg.msgID = pmsg->msgID;
		this->TServer[this->Total].msg.Param = pmsg->Param;
		this->Total++;
	}
}
