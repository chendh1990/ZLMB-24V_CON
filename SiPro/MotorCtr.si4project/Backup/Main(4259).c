#include "N76E003.h"
#include "Typedef.h"
#include "Common.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Sys.h"
#include "Uart.h"
#include "Timer.h"
#include "TimerUnit.h"
#include "QMsg.h"
#include "Peripheral.h"
#include "Log.h"
#include "PacketQueue.h"
#include "Wifi.h"

#define FW_VER				0001

 XDATA uint16 xtmp;
//uint16 tmp;

//#pragma pack(1) 
typedef struct{
	struct testDef_t *pt;
	uint8 a;
	uint8 b;
}testDef_t;
//#pragma pack() 

static void ShowSoftInfo(void);
void main(void)
{
	UartInit();
	SystemVarInit();
//	wifi_protocol_init();
	QueueInit();
	WifiPacketInit();

	PeriphralInit();
	TimerUnitInit(&g_TimerServer);
	QMsgInit(&g_QMsg, &g_MsgArray, MSG_NUM_MAX);
	TimerInit();
	ShowSoftInfo();
	//xtmp = 200;

	//tmp = xtmp;
	//Log("xtmp:%d\r\n", xtmp);
	//Log("tmp:%d\r\n", tmp);
	Log("1 :%d\r\n", (uint16)1);
	Log("2 :%d\r\n", (uint16)2);
	Log("sizeof(testDef_t):%d\r\n", (uint16)(sizeof(testDef_t)));
	Log("sizeof(testDef_t *):%d\r\n", (uint16)(sizeof(testDef_t *)));
	while(true)
	{
		SystemStatusMachine(g_SystemVar.SystemLastStatus);
		//test2(&g_t);

	}
}

static void ShowSoftInfo(void)
{
	Log("***************************************\r\n");
	Log("Build at: %s  %s\r\n", __DATE__, __TIME__);  
	Log("FW_VER:1.%d\r\n", (uint16)(FW_VER));
	Log("----------------------------------------\r\n");
}
