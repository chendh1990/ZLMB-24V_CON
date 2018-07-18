#define __SYS_C__
#include <stdio.h>
#include "Typedef.h"
#include "Key.h"
#include "Led.h"
#include "TimerUnit.h"
#include "Log.h"
#include "Sys.h"
#include "PacketQueue.h"
#include "Peripheral.h"
#include "Wifi.h"


uint8 XDATA g_SystemNull _at_ 0x0000;		//防止指针指向其他XDATA变量，指向0x0000地址，

static sysServerTO_t testServer(void);

System_Server_t g_SystemServer[SYS_MAX_SERVER_ID] =
{
	{
		KEY_SCAN_SERVER_TICK,
		KeyScanServer
	},
	{
		LED_SHOW_SERVER_TICK,
		LedShowServer
	},
	{
		TIMER_UNIT_SERVER_TICK,
		SysTimerUnitServer,
	},
	{
		100,
		testServer,
	}
};


static void SystemInitStatus(void);
static void SystemRunStatus(void);
static void SystemHandle(MSG_t XDATA *pMsg);


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

	g_RunState[0].BitState.closed = 1;
}

static void SystemRunStatus(void)
{
	Log("\r\nEntry system Run status.\r\n");
	g_SystemVar.SystemCurrentStatus = g_SystemVar.SystemNextStatus;
	while(g_SystemVar.SystemCurrentStatus == g_SystemVar.SystemNextStatus)
	{	
		if(QMsgPend(&g_QMsg, &g_Msg) == 0)
		{
		//	Log("[Main]msgID:%d\tmsgParam:%d\r\n", g_Msg.msgID, g_Msg.Param);
			switch(g_Msg.msgID)
			{
				case SYS_MSG_MAX_ID:
					break;
					
				default:
					SystemHandle(&g_Msg);
					break;
			}
		}
		WifiPacketParse();
		WifiStatusShow();
	}
}
static void SystemHandle(MSG_t XDATA *pMsg)
{
	if(!pMsg)
	{
		return ;
	}
//	Log("pMsg->msgID:%x\r\n", pMsg->msgID);
	switch (pMsg->msgID)
	{
		case SYS_MSG_KEY_ID:
			KeyHandle(pMsg);
			break;	
		case SYS_MSG_WINDOW_ID:
			WindowHandle(pMsg);
			break;
		case SYS_MSG_WIFI_ID:
			WifiHandle(pMsg);
			break;
		default:
			break;
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
				if(--g_SystemServer[id].TO == 0)
				{
					if(g_SystemServer[id].server != null)
					{
						g_SystemServer[id].TO = (*g_SystemServer[id].server)();
					}
				}
			}
		}
	}
}

static sysServerTO_t testServer(void)
{
/*
	static uint8 XDATA flash = 0;

	if(flash&1)
	{
		LedSetLevel(LED_WORK_ID, HIGH, true);
	}
	else
	{
		LedSetLevel(LED_WORK_ID, LOW, true);
	}
	flash++;
*/
	return 100;
}
