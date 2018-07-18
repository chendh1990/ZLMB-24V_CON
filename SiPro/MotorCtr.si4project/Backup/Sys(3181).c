#define __SYS_C__
#include <stdio.h>
#include "Typedef.h"
#include "Key.h"
#include "TimerUnit.h"
#include "Log.h"
#include "Sys.h"

System_Server_t g_SystemServer[SYS_MAX_SERVER_ID] =
{
	{
		1,
		KeyScan
	},
	{
		10,
		SysTimerUnitServer,
	}
};


static void SystemOnStatus(void);
static void SystemOffStatus(void);
static void SystemPendStatus(void);

void SystemVarInit(void)
{
	g_SystemVar.SystemCurrentStatus = SYSTEM_ON_STATUS;
	g_SystemVar.SystemLastStatus = SYSTEM_ON_STATUS;
	g_SystemVar.SystemNextStatus = SYSTEM_ON_STATUS;
}
void SystemStatusMachine(unsigned char SystemStatus)
{
	switch(SystemStatus)
	{			
		case SYSTEM_ON_STATUS:
			SystemOnStatus();
			break;
			
		case SYSTEM_OFF_STATUS:
			SystemOffStatus();
			break;
			
		case SYSTEM_PEND_STATUS:
			SystemPendStatus();
			break;
		
		default:
			SystemPendStatus();
			break;
	}
	g_SystemVar.SystemLastStatus = g_SystemVar.SystemCurrentStatus;
	g_SystemVar.SystemCurrentStatus = SYSTEM_NULL_STATUS;
}



static void SystemOnStatus(void)
{
	XDATA MSG_t msg;
	LOG("\r\nEntry system On status.\r\n");
	msg.msgID = 305;
	msg.Param = 306;
	///TimerUnitAdd(&g_TimerServer, TEST0_TIMER, &g_QMsg, &msg, 20);
	//TimerUnitAdd(&g_TimerServer, TEST1_TIMER, &g_QMsg, &msg, 25);
	QMsgPost(&g_QMsg, msg);
	//QMsgPost(&g_QMsg, msg);
	msg.msgID = 0;
	msg.Param = 0;
	while(g_SystemVar.SystemCurrentStatus == g_SystemVar.SystemNextStatus)
	{
		if(QMsgPend(&g_QMsg, &msg) == 0)
		{
			LOG("msgID:%d\tmsgParam:%d\r\n", msg.msgID, msg.Param);
	
			if(QMsgSize(&g_QMsg) == 0)
			{
				msg.msgID++;
				msg.Param++;
				//TimerUnitAdd(&g_TimerServer, TEST0_TIMER, &g_QMsg, &msg, 20);
				//TimerUnitAdd(&g_TimerServer, TEST1_TIMER, &g_QMsg, &msg, 25);
				QMsgPost(&g_QMsg, msg);
		//		QMsgPost(&g_QMsg, msg);
				msg.msgID = 0;
				msg.Param = 0;
			}
		}
	}
}
static void SystemOffStatus(void)
{
	LOG("\r\nEntry system Off status.\r\n");

	while(g_SystemVar.SystemCurrentStatus == g_SystemVar.SystemNextStatus)
	{
	}
}
static void SystemPendStatus(void)
{
	LOG("\r\nEntry system Pend status.\r\n");
	while(g_SystemVar.SystemCurrentStatus == g_SystemVar.SystemNextStatus)
	{
	}
}

/*
void SystemSever(MSG_t *pMsg)
{
	switch(pMsg->msgID)
	{
		case 0:
			break;
		default:
			break;
	}
}*/


void SystemServerRun(void)
{
	uint8 id;
	for(id = 0; id < SYS_MAX_SERVER_ID; id++)
	{
		if(g_SystemServer[id].TO != SERVER_DISABLE)
		{
			if(g_SystemServer[id].TO > 0)
			{
				g_SystemServer[id].TO--;
			}
			else
			{
				if(g_SystemServer[id].server != null)
				{
					g_SystemServer[id].TO = (*g_SystemServer[id].server)();
				}
			}
		}
	}
}
