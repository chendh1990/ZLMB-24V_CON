#ifndef __Log_H__
#define __Log_H__
#include <stdio.h>
#include "Uart.h"

#define DEBUG		true


#define M1(a,b) 		a##b

#ifndef Log

#if DEBUG
	#define Log			MyPrintf
	//#define Log			  printf
#else
	#define Log 		M1(/, /)
#endif

#endif


#endif
