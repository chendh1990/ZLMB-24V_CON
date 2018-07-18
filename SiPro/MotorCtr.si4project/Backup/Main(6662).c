#include "N76E003.h"
#include "Typedef.h"
#include "Common.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Sys.h"
#include "Uart.h"
#include "Timer.h"
#include "TimerUnit.h"
#include "Msg.h"
#include "Key.h"
#include "Log.h"

#define FW_VER				0001


static void ShowSoftInfo(void);
void main(void)
{
	UartInit();
	TimerInit();
	SystemVarInit();
	KeyInit();
	ShowSoftInfo();
	TimerUnitInit(&g_TimerServer);
	QMsgInit(&g_QMsg, &g_Msg, MSG_NUM_MAX);
	while(true)
	{
		SystemStatusMachine(g_SystemVar.SystemLastStatus);
	}
}

static void ShowSoftInfo(void)
{
	LOG("***************************************\r\n");
	LOG("Build at: %s  %s\r\n", __DATE__, __TIME__);  
	LOG("FW_VER:1.%d\r\n", (uint16)(FW_VER));
	LOG("----------------------------------------\r\n");
}
