#include "N76E003.h"
#include "Typedef.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Timer.h"
#include "Uart.h"
#include "Sys.h"
#include "PacketQueue.h"

/************************************************************************************************************
*    TIMER 0 interrupt subroutine
************************************************************************************************************/
static void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    TL0 = LOBYTE(TIMER_DIV12_VALUE_10ms); 
    TH0 = HIBYTE(TIMER_DIV12_VALUE_10ms);
	SystemServerRun();
}

/************************************************************************************************************
*    TIMER 1 interrupt subroutine
************************************************************************************************************/
static void Timer1_ISR (void) interrupt 3  //interrupt address is 0x001B
{
    TL1 = LOBYTE(TIMER_DIV12_VALUE_10ms); 
    TH1 = HIBYTE(TIMER_DIV12_VALUE_10ms);
}

static void Uart0_ISR(void) interrupt 4
{
    if (RI) 
    {                   /* if reception occur */
        clr_RI;         /* clear reception flag for next reception */
		QueuePost(SBUF);
    }
    if(TI) 				/* if emission occur */
    {
         clr_TI;
		 txOver = true;	                          
    }
}
static void BOD_ISR(void) interrupt 8
{

}
