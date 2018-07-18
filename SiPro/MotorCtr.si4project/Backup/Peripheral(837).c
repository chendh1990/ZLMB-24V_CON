#define __PERIPHERAL_C__

#include "N76E003.h"
#include "Function_define.h"
#include "Typedef.h"
#include "Key.h"
#include "Peripheral.h"

static void KeyInit(void)
{
	P03_Input_Mode;
	P04_Input_Mode;
	P05_Input_Mode;
}
static void LedInit(void)
{
	P01_PushPull_Mode;
	P15_PushPull_Mode;
}
static void JKInit(void)
{
	P00_PushPull_Mode;
	P10_PushPull_Mode;
}
void PeriphralInit(void)
{
	KeyInit();
	LedInit(); 
	JKInit();
}

void MotorCtr(uint8 ch, uint8 cmd)
{
	if(ch == 0)
	{
		switch (cmd)
		{
			case 0x00:				//电机停止
				JK1_CTR_OFF;
				JK2_CTR_OFF;
				break;
			case 0x01:				//电机正转
				JK1_CTR_OFF;
				JK2_CTR_OFF;
				JK1_CTR_ON;
				break;
			case 0x02:				//电机反转
				JK1_CTR_OFF;
				JK2_CTR_OFF;
				JK2_CTR_ON;
				break;
			default:
				break;
		}
	}
}


