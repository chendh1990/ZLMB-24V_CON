#define __TIMER_UNIT_C__
#include "TimerUnit.h"
#include "Common.h"
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
			Log("\t\[%bd] Post: msgid:%d param:%d\r\n", g_TimerServer.TServer[i].Id, g_TimerServer.TServer[i].msg.msgID, g_TimerServer.TServer[i].msg.Param);
			QMsgPost(g_TimerServer.TServer[i].pQmsg, g_TimerServer.TServer[i].msg);
			
			if(i != g_TimerServer.Total - 1)
			{
				g_TimerServer.TServer[i].Id = g_TimerServer.TServer[g_TimerServer.Total-1].Id;
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
#if 0
void TimerUnitInit(TimerServer_t *this)
{
	if(!this)
	{
		return ;
	}
	g_TimerServer.Total = 0;
}

void TimerUnitDel(TimerServer_t *this, unsigned char *pname)
{
	uint8 i;
	for(i = 0; i < g_TimerServer.Total; i++)
	{	
		if((g_TimerServer.TServer.pName[0] == pname[0]) && (g_TimerServer.TServer.pName[1] == pname[1]))
		{
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
}

void TimerUnitAdd(TimerServer_t *this, uint8 *pname, MSG_Q_t *pqmsg, MSG_t *pmsg, uint16 TO)
{
	Log("\t\tTimerUnitAdd msgid:%d param:%d\r\n", pmsg->msgID, pmsg->Param);
	if((!this) || (!pname) || (!pmsg))
	{
		return ;
	}
	if(g_TimerServer.Total >= TIMER_TABLE_MAX)
	{
		return ;
	}
	else
	{
		g_TimerServer.TServer[g_TimerServer.Total].pName = pname;
		g_TimerServer.TServer[g_TimerServer.Total].TO = TO;
		g_TimerServer.TServer[g_TimerServer.Total].pQmsg = pqmsg;
		g_TimerServer.TServer[g_TimerServer.Total].msg.msgID = pmsg->msgID;
		g_TimerServer.TServer[g_TimerServer.Total].msg.Param = pmsg->Param;
		g_TimerServer.Total++;
	}
}
#else
void TimerUnitInit(TimerServer_t *this)
{
	if(!this)
	{
		return ;
	}
	g_TimerServer.Total = 0;
}

void TimerUnitDel(TimerServer_t *this, uint8 Id)
{
	uint8 i;
	if(!this)
	{
		return ;
	}
	for(i = 0; i < g_TimerServer.Total; i++)
	{	
		if(g_TimerServer.TServer[i].Id == Id)
		{
			ENTER_CRITICAL();
			if(i != g_TimerServer.Total - 1)
			{
				g_TimerServer.TServer[i].Id = g_TimerServer.TServer[g_TimerServer.Total-1].Id;
				g_TimerServer.TServer[i].pQmsg = g_TimerServer.TServer[g_TimerServer.Total-1].pQmsg;
				g_TimerServer.TServer[i].msg.msgID = g_TimerServer.TServer[g_TimerServer.Total-1].msg.msgID;
				g_TimerServer.TServer[i].msg.Param = g_TimerServer.TServer[g_TimerServer.Total-1].msg.Param;
				g_TimerServer.TServer[i].TO = g_TimerServer.TServer[g_TimerServer.Total-1].TO;
			}
			if(g_TimerServer.Total != 0)
			{
				g_TimerServer.Total--;
			}
			EXIT_CRITICAL();
		}
	}
}

void TimerUnitAdd(TimerServer_t *this, uint8 Id, MSG_Q_t *pqmsg, MSG_t *pmsg, uint16 TO)
{
	uint8 i;
	Log("\t\tTimerUnitAdd msgid:%d param:%d\r\n", pmsg->msgID, pmsg->Param);
	if((!this) || (!pmsg))
	{
		return ;
	}
	if(g_TimerServer.Total >= TIMER_TABLE_MAX)
	{
		return ;
	}
	else
	{	
		ENTER_CRITICAL();
		for(i = 0; i < g_TimerServer.Total; i++)
		{	
			if(g_TimerServer.TServer[i].Id == Id)
			{
				g_TimerServer.TServer[i].pQmsg = pqmsg;
				g_TimerServer.TServer[i].msg.msgID = pmsg->msgID;
				g_TimerServer.TServer[i].msg.Param = pmsg->Param;
				g_TimerServer.TServer[i].TO = TO;
				break;
			}
		}
		if(i == g_TimerServer.Total)
		{
			g_TimerServer.TServer[g_TimerServer.Total].Id = Id;
			g_TimerServer.TServer[g_TimerServer.Total].TO = TO;
			g_TimerServer.TServer[g_TimerServer.Total].pQmsg = pqmsg;
			g_TimerServer.TServer[g_TimerServer.Total].msg.msgID = pmsg->msgID;
			g_TimerServer.TServer[g_TimerServer.Total].msg.Param = pmsg->Param;
			g_TimerServer.Total++;
		}
		EXIT_CRITICAL();
	}
}
#endif
