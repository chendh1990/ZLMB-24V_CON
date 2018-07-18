#define __SYS_C__
#include <stdio.h>
#include "Typedef.h"
#include "Key.h"
#include "TimerUnit.h"
#include "Log.h"
#include "Sys.h"
#include "PacketQueue.h"
#include "Wifi.h"


uint8 XDATA g_SystemNull _at_ 0x0000;

static sysServerTO_t testServer(void);

System_Server_t g_SystemServer[SYS_MAX_SERVER_ID] =
{
	{
		1,
		KeyScan
	},
	{
		10,
		SysTimerUnitServer,
	},
	{
		10,
		testServer,
	}
};


static void SystemInitStatus(void);
static void SystemRunStatus(void);


void SystemVarInit(void)
{
	g_SystemVar.SystemCurrentStatus = SYSTEM_INIT_STATUS;
	g_SystemVar.SystemLastStatus = SYSTEM_INIT_STATUS;
	g_SystemVar.SystemNextStatus = SYSTEM_INIT_STATUS;
}
void SystemStatusMachine(unsigned char SystemStatus)
{
	switch(SystemStatus)
	{			
		case SYSTEM_INIT_STATUS:
			SystemInitStatus();
			break;
		case SYSTEM_RUN_STATUS:
			SystemRunStatus();
			break;
		
		default:
			SystemRunStatus();
			break;
	}
	g_SystemVar.SystemLastStatus = g_SystemVar.SystemCurrentStatus;
	g_SystemVar.SystemCurrentStatus = SYSTEM_NULL_STATUS;
}

static void SystemInitStatus(void)
{
	g_SystemVar.SystemCurrentStatus = SYSTEM_RUN_STATUS;
}

static void SystemRunStatus(void)
{
	Log("\r\nEntry system On status.\r\n");
	g_SystemVar.SystemCurrentStatus = g_SystemVar.SystemNextStatus;
	
	while(g_SystemVar.SystemCurrentStatus == g_SystemVar.SystemNextStatus)
	{	
		if(QMsgPend(&g_QMsg, &g_Msg) == 0)
		{
			Log("[Main]msgID:%d\tmsgParam:%d\r\n", g_Msg.msgID, g_Msg.Param);
			switch(g_Msg.msgID)
			{
				case PS_K1:
					break;
					
				case PS_K2:
					break;
					
				case PS_K3:
					break;
					
				case PL_K1:
					break;
					
				case PL_K2:
					break;
					
				case PL_K3:
					break;
					
				default:
					break;
			}
		}
		WifiPacketParse();
	}
}
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

static sysServerTO_t testServer(void)
{
	static XDATA MSG_t msg = {100, 100};
	if(QMsgSize(&g_QMsg) == 0)
	{
	//	QMsgPost(&g_QMsg, msg);
	}
	//Log("QMsgSize:%d\r\n", (uint16)QMsgSize(&g_QMsg));
	msg.msgID++;
	msg.Param++;
	return 50;
}
