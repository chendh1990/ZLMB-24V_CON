#define __LED_C__
#include "Led.h"
#include "Log.h"

static Led_Status_t XDATA s_LedStatus[LED_MAX_ID];

void LedInit(void)
{
	uint8 i;
	P01_PushPull_Mode;
	P11_PushPull_Mode;
	P12_PushPull_Mode;
	P15_PushPull_Mode;

	LED_OPEN_OFF;
	LED_CLOSE_OFF;
	LED_PAUSE_OFF;

	for(i = 0; i < LED_MAX_ID; i++)
	{
	    s_LedStatus[i].count = 0;
	    s_LedStatus[i].timerOn = 0;
	    s_LedStatus[i].timerOff = 0;
	    s_LedStatus[i].cnt = 0;
	}
}

sysServerTO_t LedShowServer(void)
{
	uint8 i;
	for(i = 0; i < LED_MAX_ID; i++)
	{
		if(s_LedStatus[i].cnt > 0)
		{
		    s_LedStatus[i].cnt--;
		    if(s_LedStatus[i].cnt == s_LedStatus[i].timerOff)
			{
		        LedSetLevel(i, LOW, false);
			}
		}
		else
		{
		    if(s_LedStatus[i].count == 0xFF)
		    {
		        LedSetLevel(i, HIGH, false);
		        s_LedStatus[i].cnt = s_LedStatus[i].timerOn + s_LedStatus[i].timerOff;
		    }
		    else if(s_LedStatus[i].count > 0)
		    {
		        LedSetLevel(i, HIGH, false);
		        s_LedStatus[i].count--;
		        s_LedStatus[i].cnt = s_LedStatus[i].timerOn + s_LedStatus[i].timerOff;
		    }
		}
	}
	return LED_SHOW_SERVER_TICK;
}


//count:闪烁次数,	其中count = 0xff为连续
//on_time:开时间,
//off_time关时间(单位10ms),
void LedSetFlash(uint8 id, uint8 count, uint16 ontime, uint16 offtime)     
{
	if(id > LED_MAX_ID)
	{
		return;
	}	
    s_LedStatus[id].count = count;
    s_LedStatus[id].timerOn = ontime;
    s_LedStatus[id].timerOff = offtime;
    s_LedStatus[id].cnt = 0;	
    LedSetLevel(id, LOW, false);
}


void LedSetLevel(uint8 id, uint8 level, uint8 flag)
{
	if(id > LED_MAX_ID)
	{
		return ;
	}
	switch (id)
	{
		case LED_OPEN_ID:			
		    if(level)
		    {
		    	LED_OPEN_ON ;
		    }
		    else
			{
		    	LED_OPEN_OFF;
			}
			break;
			
		case LED_CLOSE_ID:			
		    if(level)
		    {
		    	LED_CLOSE_ON ;
		    }
		    else
			{
		    	LED_CLOSE_OFF;
			}
			break;
			
		case LED_PAUSE_ID:			
		    if(level)
		    {
		    	LED_PAUSE_ON ;
		    }
		    else
			{
		    	LED_PAUSE_OFF;
			}
			break;
			
		case LED_WIFI_ID:
		    if(level)
		    {
		    	LED_WIFI_ON;
		    }
		    else
			{
		    	LED_WIFI_OFF;
			}
			break;
			
		default:
			break;
	}
	if(flag)
	{
		s_LedStatus[id].count = 0;
		s_LedStatus[id].timerOn = 0;
		s_LedStatus[id].timerOff = 0;
		s_LedStatus[id].cnt = 0;	
	}
}

