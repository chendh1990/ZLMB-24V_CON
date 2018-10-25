#define __TIMER_C__
#include "N76E003.h"
#include "Typedef.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Timer.h"
#include "Iap.h"
#include "Log.h"


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
		ProgramCONFIG(0x04, 0x0f);
	}
	WDTEnable();
   	Timer0Init();
   	Timer1Init();
}
