#ifndef __TIMER_H__
#define __TIMER_H__

#ifndef __TIMER_C__
	#define TIMER_EXTERN	extern
#else
	#define TIMER_EXTERN
#endif

TIMER_EXTERN void TimerInit(void);
TIMER_EXTERN void Timer1_Delay10ms(unsigned int u32CNT);


#endif

