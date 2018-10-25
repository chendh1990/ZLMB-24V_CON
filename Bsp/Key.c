#define __KEY_C__

#include "SFR_Macro.h"
#include "Key.h"
#include "Sys.h"
#include "Peripheral.h"
#include "Wifi.h"
#include "Led.h"
#include "Iap.h"
#include "Log.h"


static void DealFirmwareUpdate(uint8 keyValue);

void KeyInit(void)
{
	P03_Input_Mode;
	P04_Input_Mode;
	P05_Input_Mode;
}
static uint8 ReadKey(void)
{
	uint8 key = 0x00;

	if(KEY_K1_PIN == 0)
	{
		key |= KEY_K1;
	}
	if(KEY_K2_PIN == 0)
	{
		key |= KEY_K2;
	}
	if(KEY_K3_PIN == 0)
	{
		key |= KEY_K3;
	}
	return key;
}

static uint8 KeyDetect(void)
{
	uint8 key;
	key = ReadKey();

	if(key == 0x00) 
	{
		key = NO_KEY;
	}
	return key;
}

sysServerTO_t KeyScanServer(void)
{
    static uint8 keyValue = 0;
	static uint16 keyCounter = 0;
    uint8 keyTmp = NO_KEY;
    uint8 key_return = NO_KEY;
	
    keyTmp = KeyDetect();
	if(keyTmp == NO_KEY )
	{
		if (keyCounter >= KEY_LONG_TIMES)  //长按抬键
		{
			key_return = keyValue | KEY_LONG_UP;
		}
		else if (keyCounter >= KEY_SCAN_TIMES)   //短按抬键
		{
			key_return = keyValue | KEY_SHORT_UP;
		}
		keyValue = keyTmp;
		keyCounter = 0;
	}
    else if(keyTmp != keyValue)
    {
		keyValue = keyTmp;
		keyCounter = 0;
    }	
    else
    {
		keyCounter++;
		if (keyCounter == KEY_SCAN_TIMES)			//去抖
		{
			key_return = keyValue;
		}
		else if (keyCounter == KEY_LONG_TIMES)				//长按
		{
			key_return = keyValue | KEY_LONG;
		}
		else if (keyCounter == (KEY_LONG_TIMES + KEY_HOLD_TIMES) )			//连按
		{
			key_return = keyValue | KEY_HOLD;
			keyCounter = KEY_LONG_TIMES;
		}
    }
    if (key_return != NO_KEY)
    {
		QMsgPostSimple(&g_QMsg, SYS_MSG_KEY_ID, key_return);
    }
	return KEY_SCAN_SERVER_TICK;
}
void KeyHandle(const MSG_t *const pMsg)
{
	if(!pMsg)
	{
		return ;
	}
	//return ;
	//Log("msgParm:%bx\r\n", (uint8)(pMsg->Param));
	switch (pMsg->Param)
	{
		case PS_K1:				//打开
			Log("PS_K1\r\n");
			//DealFirmwareUpdate(PS_K1);
			QMsgPostSimple(&g_QMsg, SYS_MSG_WINDOW_ID, WINDOW_CLOSING);
			break;
			
		case PS_K2:				//暂停
			Log("PS_K2\r\n");
			//DealFirmwareUpdate(PS_K2);
			QMsgPostSimple(&g_QMsg, SYS_MSG_WINDOW_ID, WINDOW_PAUSE);
			break;
			
		case PS_K3:				//关闭
			Log("PS_K3\r\n");	
			//DealFirmwareUpdate(PS_K3);
			QMsgPostSimple(&g_QMsg, SYS_MSG_WINDOW_ID, WINDOW_OPENING);
			break;
			
		case PL_K1:				//smartconfig
			Log("PL_K1\r\n");
			//DealFirmwareUpdate(PL_K1);
			QMsgPostSimple(&g_QMsg, SYS_MSG_WIFI_ID, WIFI_SMART_CONFIG);
			break;
			
		case PL_K2:				//smartconfig
			Log("PL_K2\r\n");
			//DealFirmwareUpdate(PL_K2);
			QMsgPostSimple(&g_QMsg, SYS_MSG_WIFI_ID, WIFI_SMART_CONFIG);
			break;
			
		case PL_K3:				//smartconfig
			Log("PL_K3\r\n");
			//DealFirmwareUpdate(PL_K3);
			QMsgPostSimple(&g_QMsg, SYS_MSG_WIFI_ID, WIFI_SMART_CONFIG);
			break;
			
		default:
			break;
	}
}
//static void DealFirmwareUpdate(uint8 keyValue)
//{
//	static uint8 FirmwareUpdateK;
//	if(keyValue == PS_K2)
//	{
//		FirmwareUpdateK++;
//	}
//	else if(keyValue == PL_K2)
//	{
//		if(FirmwareUpdateK == 6)
//		{
//			uint8 i;
//			for(i = 0; i < 10; i++)
//			{
//				LedSetLevel(LED_OPEN_ID, HIGH, true);
//				LedSetLevel(LED_PAUSE_ID, HIGH, true);
//				LedSetLevel(LED_CLOSE_ID, HIGH, true);
//				delay(50);
//				LedSetLevel(LED_OPEN_ID, LOW, true);
//				LedSetLevel(LED_PAUSE_ID, LOW, true);
//				LedSetLevel(LED_CLOSE_ID, LOW, true);
//				delay(50);
//			}
//			Log("FirmwareUpdate \r\n");
//			SoftResetLdRomStart();
//		}
//		else
//		{
//			FirmwareUpdateK = 0;
//		}
//	}
//	else 
//	{
//		FirmwareUpdateK = 0;
//		
//	}
//}


