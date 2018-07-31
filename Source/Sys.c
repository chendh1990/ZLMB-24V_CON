#define __SYS_C__
#include <stdio.h>
#include "Common.h"
#include "Key.h"
#include "Led.h"
#include "Timer.h"
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


static void ShowSoftInfo(void);
static void SystemInitStatus(void);
static void SystemRunStatus(void);
static void SystemHandle(MSG_t XDATA *pMsg);


static void ShowSoftInfo(void)
{
	Log("***************************************\r\n");
	Log("Build at: %s  %s\r\n", __DATE__, __TIME__);  
	Log("FW_VER:1.%d\r\n", (uint16)(FW_VER));
	Log("----------------------------------------\r\n");
}

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
	UartInit();
	QueueInit();
	WifiInit();
	TimerUnitInit(&g_TimerServer);
	QMsgInit(&g_QMsg, &g_MsgArray, MSG_NUM_MAX);
	ShowSoftInfo();
	
	PeriphralInit();
	TimerInit();
	QMsgPostSimple(&g_QMsg, SYS_MSG_INIT_ID, 0);
	g_SystemVar.SystemCurrentStatus = SYSTEM_RUN_STATUS;
}

static void SystemRunStatus(void)
{
	Log("\r\nEntry system Run status.\r\n");
	g_SystemVar.SystemCurrentStatus = g_SystemVar.SystemNextStatus;
	while(g_SystemVar.SystemCurrentStatus == g_SystemVar.SystemNextStatus)
	{	
		if(QMsgPend(&g_QMsg, &g_Msg) == 0)
		{
			switch(g_Msg.msgID)
			{
				case SYS_MSG_INIT_ID:	
					g_RunState[0].sta = 0;
					g_RunState[0].BitState.closed = 1;
				
					QMsgPostSimple(&g_QMsg, SYS_MSG_WINDOW_ID, WINDOW_OPENING);
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
