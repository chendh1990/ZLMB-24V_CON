#ifndef __KEY_H__
#define __KEY_H__

#ifndef __KEY_C__
	#define KEY_EXTERN	extern
#else
	#define KEY_EXTERN
#endif




#define KEY_SCAN_TIMES		(2)  	//按键防抖的扫描次数
#define KEY_SHORT_TIMES		(5)
#define KEY_LONG_TIMES		(300)	//长按键的次数
#define KEY_HOLD_TIMES      (50)    //连按的频率次数


#define NO_KEY          0xff
							 
#define KEY_SHORT_UP    0x10
#define KEY_LONG		0x20
#define KEY_LONG_UP     0x30
#define KEY_HOLD		0x40


#define KEY_K1			0x01
#define PR_K1			KEY_K1
#define PS_K1			(KEY_SHORT_UP | KEY_K1)
#define PL_K1			(KEY_LONG | KEY_K1)
#define PU_K1			(KEY_LONG_UP | KEY_K1)
#define PH_K1			(KEY_HOLD | KEY_K1)

#define KEY_K2			0x02
#define PR_K2			KEY_K2
#define PS_K2			(KEY_SHORT_UP | KEY_K2)
#define PL_K2			(KEY_LONG | KEY_K2)
#define PU_K2			(KEY_LONG_UP | KEY_K2)
#define PH_K2			(KEY_HOLD | KEY_K2)

#define KEY_K3			0x04
#define PR_K3			KEY_K3
#define PS_K3			(KEY_SHORT_UP | KEY_K3)
#define PL_K3			(KEY_LONG | KEY_K3)
#define PU_K3			(KEY_LONG_UP | KEY_K3)
#define PH_K3			(KEY_HOLD | KEY_K3)


#define KEY_SCAN_SERVER_TICK		10

KEY_EXTERN sysServerTO_t KeyScan(void);

#endif
