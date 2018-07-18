#define __TIMER_C__
#include "N76E003.h"
#include "Typedef.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Timer.h"

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

void TimerInit(void)
{
   	Timer0Init();
   	Timer1Init();
}
