#ifndef __PERIPHERAL_H__
#define __PERIPHERAL_H__
#include "N76E003.h"
#include "Function_define.h"

#ifndef __PERIPHERAL_C__
	#define PERIPHERAL_EXTERN extern
#else
	#define PERIPHERAL_EXTERN
#endif

/***********************∫Í∂®“Â****************************/
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
#define LED_WIFI_PIN		P01
#define LED_WORK_PIN		P15

//#define JK1_CTR_OUT			JK1_CTR_PIN##_PushPull_Mode
#define JK1_CTR_ON			JK1_CTR_PIN = ON
#define JK1_CTR_OFF			JK1_CTR_PIN = OFF

#define JK2_CTR_ON			JK2_CTR_PIN = ON
#define JK2_CTR_OFF			JK2_CTR_PIN = OFF

#define LED_WIFI_ON			LED_WIFI_PIN = ON
#define LED_WIFI_OFF		LED_WIFI_PIN = OFF

#define LED_WORK_ON			LED_WORK_PIN = ON
#define LED_WORK_OFF		LED_WORK_PIN = OFF



PERIPHERAL_EXTERN void KeyInit(void);

#endif

