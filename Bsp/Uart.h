#ifndef __UART_H__
#define __UART_H__

#ifndef __UART_C__
	#define UART_EXTERN	extern
#else
	#define UART_EXTERN
#endif

/**********************���ݽṹ����***********************/
typedef enum
{	
	UART_WIFI = 0,
	UART_DEBUG
}UartPort_t;
/***********************�궨��****************************/




/**********************��������**************************/
UART_EXTERN void UartInit(void);
UART_EXTERN	char putchar(char c);
UART_EXTERN void UartSendString(UartPort_t uport, uint8 const *str);
UART_EXTERN void UartSendBuffer(UartPort_t Port, const uint8 *const Buffer, uint8 length);
UART_EXTERN void UartSendHexBuffer(UartPort_t Port, const uint8 *des, const uint8 *const buf , uint8 len, uint8 dec, uint8 en);
UART_EXTERN void MyPrintf(const char *fmt, ...);
#endif
