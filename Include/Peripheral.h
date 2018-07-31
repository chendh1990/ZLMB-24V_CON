#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__
#include "N76E003.h"
#include "Function_define.h"
#include "TimerUnit.h"
#include "QMsg.h"

#ifndef __PERIPHERAL_C__
	#define PERIPHERAL_EXTERN extern
#else
	#define PERIPHERAL_EXTERN
#endif

/***********************宏定义****************************/
#ifndef ON
#define ON					1
#endif
#ifndef OFF
#define OFF					0
#endif

#define KEY_K1_PIN			P03
#define KEY_K2_PIN			P04
#define KEY_K3_PIN			P05

#define JK1_CTR_PIN			P10
#define JK2_CTR_PIN			P00
#define LED_OPEN_PIN		P11
#define LED_CLOSE_PIN		P12
#define LED_PAUSE_PIN		P15
#define LED_WIFI_PIN		P01

#define JK1_CTR_ON			JK1_CTR_PIN = ON
#define JK1_CTR_OFF			JK1_CTR_PIN = OFF

#define JK2_CTR_ON			JK2_CTR_PIN = ON
#define JK2_CTR_OFF			JK2_CTR_PIN = OFF

#define LED_OPEN_ON 		LED_OPEN_PIN = ON
#define LED_OPEN_OFF 		LED_OPEN_PIN = OFF

#define LED_CLOSE_ON 		LED_CLOSE_PIN = ON
#define LED_CLOSE_OFF 		LED_CLOSE_PIN = OFF

#define LED_PAUSE_ON 		LED_PAUSE_PIN = ON
#define LED_PAUSE_OFF		LED_PAUSE_PIN = OFF

#define LED_WIFI_ON			LED_WIFI_PIN = OFF
#define LED_WIFI_OFF		LED_WIFI_PIN = ON



#define WINDOW_CTL_TIME		SEC(20)
#define WINDOW_WAIT_TIME	SEC(10)

/***********************数据结构定义************************/

typedef enum{
	WINDOW_OPENED = 0x01,
	WINDOW_CLOSED,
	WINDOW_OPEN,
	WINDOW_CLOSE,
	WINDOW_OPENING,
	WINDOW_CLOSING,
	WINDOW_PAUSE,
	WINDOW_WAIT
}Window_State_t;


typedef enum{
	MOTOR_POS_TURN = 1,
	MOTOR_NEG_TURN,
	MOTOR_STOP_TURN
}Motor_State_t;
/********************全局变量声明与定义***********************/


PERIPHERAL_EXTERN void PeriphralInit(void);
PERIPHERAL_EXTERN void WindowHandle(const MSG_t *const pMsg);

#endif

