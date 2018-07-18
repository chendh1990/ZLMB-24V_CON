#ifndef __LOG_H__
#define __LOG_H__
#include "Uart.h"

#define DEBUG		true


#define M1(a,b) 		a##b

#ifndef LOG

#if DEBUG
	#define LOG			printf
#else
	#define LOG 		M1(/, /)
#endif

#endif


#endif
