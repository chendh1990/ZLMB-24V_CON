#define __KEY_C__
#include "Key.h"
#include "QMsg.h"


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

sysServerTO_t KeyScan(void)
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
		MSG_t msg;
    	msg.msgID = key_return;
		QMsgPost(&g_QMsg, msg);
    }
	return KEY_SCAN_SERVER_TICK;
}


