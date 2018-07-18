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
#include "Key.h"
#include "Log.h"

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

XDATA testDef_t  volatile  g_t;

void test1(testDef_t *t);
static void ShowSoftInfo(void);
void main(void)
{
	UartInit();
	SystemVarInit();
	TimerUnitInit(&g_TimerServer);
	QMsgInit(&g_QMsg, &g_Msg, MSG_NUM_MAX);
	KeyInit();
	TimerInit();
	ShowSoftInfo();
	//xtmp = 200;

	//tmp = xtmp;
	//LOG("xtmp:%d\r\n", xtmp);
	//LOG("tmp:%d\r\n", tmp);
	LOG("1 :%d\r\n", (uint16)1);
	LOG("2 :%d\r\n", (uint16)2);
	LOG("sizeof(testDef_t):%d\r\n", (uint16)(sizeof(testDef_t)));
	LOG("sizeof(testDef_t *):%d\r\n", (uint16)(sizeof(testDef_t *)));
	g_t.a = 1;
	g_t.b = 1;
	LOG("Start a: %d  b: %d\r\n", (uint16)g_t.a, (uint16)g_t.b);
	while(true)
	{
		SystemStatusMachine(g_SystemVar.SystemLastStatus);
		//test1(&g_t);

	}
}

static void ShowSoftInfo(void)
{
	LOG("***************************************\r\n");
	LOG("Build at: %s  %s\r\n", __DATE__, __TIME__);  
	LOG("FW_VER:1.%d\r\n", (uint16)(FW_VER));
	LOG("----------------------------------------\r\n");
}
void test1(testDef_t *t)
{
	t->a++;
	t->b++;
	LOG("a: %d  b: %d\r\n", (uint16)(t->a), (uint16)(t->b));
}
