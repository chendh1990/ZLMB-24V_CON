#ifndef __UART_H__
#define __UART_H__

#ifndef __UART_C__
	#define UART_EXTERN	extern
#else
	#define UART_EXTERN
#endif

/**********************数据结构定义***********************/
typedef enum
{	
	UART_WIFI = 0,
	UART_DEBUG
}UartPort_t;
/***********************宏定义****************************/




/**********************函数声明**************************/
UART_EXTERN void UartInit(void);
UART_EXTERN	char putchar(char c);
UART_EXTERN void MyPrintf(const char *fmt, ...);
#endif
