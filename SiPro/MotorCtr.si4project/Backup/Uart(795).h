#ifndef __UART_H__
#define __UART_H__

#ifndef __UART_C__
	#define UART_EXTERN	extern
#else
	#define UART_EXTERN
#endif

UART_EXTERN void UartInit(void);
UART_EXTERN	char putchar(char c);
#endif