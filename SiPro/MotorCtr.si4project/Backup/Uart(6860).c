#define __UART_C__
#include <stdio.h>
#include <stdarg.h>
#include "Typedef.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Uart.h"


//----------------------------------------------------------------------------------
// UART0 baud rate initial setting 
//----------------------------------------------------------------------------------
static void InitialUART0_Timer1(UINT32 u32Baudrate)    //T1M = 1, SMOD = 1
{
	P06_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
	P07_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
	
    SCON = 0x50;     	//UART0 Mode1,REN=1,TI=1
    TMOD |= 0x20;    	//Timer1 Mode1
    
    set_SMOD;        	//UART0 Double Rate Enable
    set_T1M;
    clr_BRCK;        	//Serial port 0 baud rate clock source = Timer1
 
#ifdef FOSC_160000
    TH1 = 256 - (1000000/u32Baudrate+1);               /*16 MHz */
#endif    	
#ifdef FOSC_166000
    TH1 = 256 - (1037500/u32Baudrate);         		     /*16.6 MHz */
#endif
    set_TR1;
	set_TI;						//For printf function must setting TI = 1
}

//---------------------------------------------------------------
static void InitialUART0_Timer3(UINT32 u32Baudrate) //use timer3 as Baudrate generator
{
	P06_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
	P07_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit	
	
    SCON = 0x50;     //UART0 Mode1,REN=1,TI=1
    set_SMOD;        //UART0 Double Rate Enable
    T3CON &= 0xF8;   //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
    set_BRCK;        //UART0 baud rate clock source = Timer3

#ifdef FOSC_160000
		RH3    = HIBYTE(65536 - (1000000/u32Baudrate)-1);  		/*16 MHz */
		RL3    = LOBYTE(65536 - (1000000/u32Baudrate)-1);			/*16 MHz */
#endif
#ifdef FOSC_166000
    RH3    = HIBYTE(65536 - (1037500/u32Baudrate)); 			/*16.6 MHz */
    RL3    = LOBYTE(65536 - (1037500/u32Baudrate)); 			/*16.6 MHz */
#endif
    set_TR3;         //Trigger Timer3
	set_TI;					 //For printf function must setting TI = 1
}

//----------------------------------------------------------------------------------
// UART1 baud rate initial setting 
//----------------------------------------------------------------------------------
static void InitialUART1_Timer3(UINT32 u32Baudrate) //use timer3 as Baudrate generator
{
		P02_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
		P16_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
	
	  SCON_1 = 0x50;   	//UART1 Mode1,REN_1=1,TI_1=1
//    T3CON = 0x08;   	//T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1), UART1 in MODE 1
	//	clr_BRCK;
	
#ifdef FOSC_160000
		RH3    = HIBYTE(65536 - (1000000/u32Baudrate)-1);  		/*16 MHz */
		RL3    = LOBYTE(65536 - (1000000/u32Baudrate)-1);			/*16 MHz */
#endif
#ifdef FOSC_166000
		RH3    = HIBYTE(65536 - (1037500/u32Baudrate));  			/*16.6 MHz */
		RL3    = LOBYTE(65536 - (1037500/u32Baudrate));				/*16.6 MHz */
#endif
    set_TR3;         //Trigger Timer3
    set_TI_1;
}

static void InitialUART0_Int()
{
/**********************设置串口0位最高优先级********************/
	IP |= BIT(4);	//中断优先级低位
	IPH |= BIT(4);	//中断优先级高位
/***************************************************************/
/***********************使能串口0中断***************************/
	set_ES; 	//使能串口0中断
/***************************************************************/

/**************************使能总中断***************************/
	set_EA; 	//使能总中断
/***************************************************************/
	
}
void UartInit(void)
{		
	InitialUART0_Timer3(9600);
	InitialUART1_Timer3(9600);
//	InitialUART0_Int();
}
#if 0 //printf for uart1
char putchar(char c)
{
	while(!TI_1);  /* wait until transmitter ready */
	TI_1 = 0;
	SBUF_1 = c;      /* output character */
	return (c);
}
#else	//printf for uart0
char putchar(char c)
{
	while(!TI);  /* wait until transmitter ready */
	TI = 0;
	SBUF = c;      /* output character */
	return (c);
}
#endif

char putchar1(char c)
{
	while(!TI);  /* wait until transmitter ready */
	TI = 0;
	SBUF = c;      /* output character */
	return (c);
}

static void UartWriteString(UartPort_t uport, uint8 const XDATA *str)
{
	if(!str)
	{
		return ;
	}
	switch (uport)
	{
		case UART_WIFI:
			{
				while(*str)
				{
					putchar(*str++);
				}
			}
			break;
		case UART_DEBUG:
			{
				while(*str)
				{
					putchar1(*str++);
				}
			}
			break;
		default:
			break;
	}
}
void MyPrintf(const char *fmt, ...)
{
	va_list ap;
	uint8 XDATA string[100];
	va_start(ap, fmt);  
	vsprintf(string, fmt, ap);//此处也可以使用sprintf函数，用法差不多，稍加修改即可，此处略去
	va_end(ap);		
	UartWriteString(UART_WIFI, string);
}
