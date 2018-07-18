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
	uint16 XDATA dat = 90;
	Log("\r\nEntry system On status.\r\n");
	g_Msg.msgID = 305;
	g_Msg.Param = 306;
	//TimerUnitAdd(&g_TimerServer, TIMER_TEST0_ID, &g_QMsg, &g_Msg, 20);
	//TimerUnitAdd(&g_TimerServer, TEST1_TIMER, &g_QMsg, &g_Msg, 25);
	//QMsgPost(&g_QMsg, msg);
	//QMsgPost(&g_QMsg, g_Msg);
	g_Msg.msgID = 0;
	g_Msg.Param = 0;
	//Log("g_QMsg.size:%d\r\n", (uint16)(g_QMsg.Size));
	//Log("g_QMsg.out:%d\r\n", (uint16)(g_QMsg.Out));
	//Log("g_QMsg.in:%d\r\n", (uint16)(g_QMsg.In));
	while(g_SystemVar.SystemCurrentStatus == g_SystemVar.SystemNextStatus)
	{
	
		//Log("[Main] &g_Qmsg = %x\r\n", (uint16)(&g_QMsg));
	//	Log("[Main] &g_QmsgSize = %d\r\n", (uint16)(QMsgSize(&g_QMsg)));
		if(QMsgPend(&g_QMsg, &g_Msg) == 0)
		{
			Log("[Main]msgID:%d\tmsgParam:%d\r\n", g_Msg.msgID, g_Msg.Param);
			//continue;
			//Log("Main: &g_Qmsg:%d\r\n", (uint16)(&g_QMsg));
			//if(QMsgSize(&g_QMsg) == 0)
			{	dat++;
				g_Msg.msgID = dat;
				g_Msg.Param = dat;
				Log("[Main]---dat:%d\tmsgID:%d\tmsgParam:%d\r\n", dat, g_Msg.msgID, g_Msg.Param);
				//TimerUnitAdd(&g_TimerServer, TIMER_TEST0_ID, &g_QMsg, &g_Msg, 25);
				//TimerUnitAdd(&g_TimerServer, TIMER_TEST1_ID, &g_QMsg, &g_Msg, 20);
				//QMsgPost(&g_QMsg, g_Msg);
				//dat++;
				g_Msg.msgID = dat;
				g_Msg.Param = dat;
				//QMsgPost(&g_QMsg, g_Msg);
				g_Msg.msgID = 0;
				g_Msg.Param = 0;
			}
		}
	//	wifi_uart_service();
		//if(QueuePend(&tmp) == 0)
		{
		//	putchar(tmp);
		}
		WifiPacketParse();
	}
}
static void SystemOffStatus(void)
{
	Log("\r\nEntry system Off status.\r\n");

	while(g_SystemVar.SystemCurrentStatus == g_SystemVar.SystemNextStatus)
	{
	}
}
static void SystemPendStatus(void)
{
	Log("\r\nEntry system Pend status.\r\n");
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
