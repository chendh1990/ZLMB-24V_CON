#define __TIMER_C__
#include "N76E003.h"
#include "Typedef.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Timer.h"
#include "Log.h"


static bit BIT_TMP;

/************************************************************************************************************
*    Main function 
************************************************************************************************************/
static void Timer0Init(void)
{   	
	TIMER0_MODE1_ENABLE;
	P2S &= ~BIT(2);	//禁止定时器0输出
    clr_T0M;
	
	TL0 = LOBYTE(TIMER_DIV12_VALUE_10ms);		//Find	define in "Function_define.h" "TIMER VALUE"
	TH0 = HIBYTE(TIMER_DIV12_VALUE_10ms);	
    set_ET0;                                    //enable Timer0 interrupt
    set_EA;                                     //enable interrupts
    set_TR0;                                    //Timer0 run
}
static void Timer1Init(void)
{   	
	TIMER1_MODE1_ENABLE;
	P2S &= ~BIT(3);	//禁止定时器0输出
    clr_T1M;
	
	TL1 = LOBYTE(TIMER_DIV12_VALUE_10ms);		//Find	define in "Function_define.h" "TIMER VALUE"
	TH1 = HIBYTE(TIMER_DIV12_VALUE_10ms);	
    set_ET1;                                    //enable Timer0 interrupt
    set_EA;                                     //enable interrupts
    set_TR1;                                    //Timer0 run
}

static bool WDTIsEnableConfig(void)
{
	uint8 reg;
	set_IAPEN;
	IAPAL = LOBYTE(0x0004);
	IAPAH = HIBYTE(0x0004);
	IAPCN = 0xc0;
	set_IAPGO;
	reg = IAPFD;
	clr_IAPEN;

	Log("config4 reg:%bd\r\n", reg);
	
	return (reg == 0x0f);
}

static void WDTEnableConfig(void)
{
	uint8 cf0,cf1,cf2,cf3,cf4;
#define     CFG_READ            0xc0
#define     CFG_ERASE           0xe2
#define     CFG_BYTE_PROGRAM    0xe1

	set_IAPEN;
	IAPAL = 0x00;
	IAPAH = 0x00;
	IAPCN = CFG_READ;
	set_IAPGO;                                  //Storage CONFIG0 data
	cf0 = IAPFD;
	IAPAL = 0x01;
	set_IAPGO;                                  //Storage CONFIG1 data
	cf1 = IAPFD;
	IAPAL = 0x02;
	set_IAPGO;                                  //Storage CONFIG2 data
	cf2 = IAPFD;
	IAPAL = 0x03;
	set_IAPGO;                                  //Storage CONFIG3 data
	cf3 = IAPFD;
	IAPAL = 0x04;
	set_IAPGO;                                  //Storage CONFIG4 data
	cf4 = IAPFD;
	cf4 = 0x0f;									//Moidfy Storage CONFIG4 data enable WDT reset

	set_CFUEN;	
	IAPCN = CFG_ERASE;													//Erase CONFIG all
	IAPAH = 0x00;
	IAPAL = 0x00;
	IAPFD = 0xFF;
	set_IAPGO;

	IAPCN = CFG_BYTE_PROGRAM;										//Write CONFIG
	IAPFD = cf0;
	set_IAPGO;
	IAPAL = 0x01;
	IAPFD = cf1;
	set_IAPGO;
	IAPAL = 0x02;
	IAPFD = cf2;
	set_IAPGO;
	IAPAL = 0x03;
	IAPFD = cf3;
	set_IAPGO;
	IAPAL = 0x04;
	IAPFD = cf4;
	set_IAPGO;

	clr_CFUEN;
	clr_IAPEN;
}

void WDTEnable(void)
{
	set_WPS0;
	set_WPS1;
	set_WPS2;

	clr_WDTRF;
	clr_WDTF;
	set_WDCLR;									//Clear WDT timer
	set_WDTR;	
}

void WDTFeed(void)
{
	set_WDCLR;
}
void TimerInit(void)
{
	if(!WDTIsEnableConfig())
	{
		WDTEnableConfig();
	}
	WDTEnable();
   	Timer0Init();
   	Timer1Init();
}
