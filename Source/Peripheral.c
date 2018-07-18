#define __PERIPHERAL_C__

#include "N76E003.h"
#include "Function_define.h"
#include "Typedef.h"
#include "Key.h"
#include "Led.h"
#include "Sys.h"
#include "Peripheral.h"
#include "Wifi.h"
#include "Log.h"


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
	LED_PAUSE_OFF;
	LED_WIFI_OFF;
	JK1_CTR_ON;
	JK2_CTR_OFF;
}

void MotorCtr(uint8 ch, uint8 cmd)
{
	if(ch == 0)
	{
		switch (cmd)
		{
			case MOTOR_POS_TURN:	//电机正转
				JK1_CTR_OFF;
				JK2_CTR_OFF;
				break;
			case MOTOR_NEG_TURN:	//电机反转
				JK1_CTR_ON;
				JK2_CTR_ON;
				break;
			case MOTOR_STOP_TURN:	//电机停转
				JK1_CTR_OFF;
				JK2_CTR_ON;
				break;
			default:
				break;
		}
	}
}

void WindowHandle(const MSG_t *const pMsg)
{
	uint16 XDATA TO;
	static uint16 XDATA TObak;
	MSG_t XDATA msg;
	if(!pMsg)
	{
		return ;
	}
//	Log("msgParm:%bx\r\n", (uint8)(pMsg->Param));
	switch (pMsg->Param)
	{
		case WINDOW_OPENING:			//正在打开
			Log("WINDOW_OPENING\r\n");
			if(g_RunState[0].BitState.wait)
			{
				break;
			}
			if(g_RunState[0].BitState.pause)		//暂停状态 -> 打开运行状态
			{
				if(g_RunState[0].BitState.opening) //打开运行状态-> 暂停状态 -> 打开运行状态
				{
					MotorCtr(0, MOTOR_POS_TURN);	
					TimerUnitEnable(&g_TimerServer, TIMER_WINDOW_CTR_ID, true);
				}
				if(g_RunState[0].BitState.closing) //关闭运行状态-> 暂停状态 -> 打开运行状态
				{
					TO = WINDOW_CTL_TIME - TimerUnitGetTO(&g_TimerServer, TIMER_WINDOW_CTR_ID);					
					msg.msgID = SYS_MSG_WINDOW_ID;
					msg.Param = WINDOW_OPENED;
					TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);
					MotorCtr(0, MOTOR_POS_TURN);
				}
				g_RunState[0].sta = 0;
				g_RunState[0].BitState.opening = 1;
						
				LedSetLevel(LED_CLOSE_ID, LOW, true);
				LedSetLevel(LED_OPEN_ID, HIGH, true);
				LedSetLevel(LED_PAUSE_ID, LOW, true);
			}
			else
			{
				if((g_RunState[0].BitState.closed) || (g_RunState[0].BitState.closing)) //从关闭或关闭运行状态到打开运行状态
				{
					if(g_RunState[0].BitState.closed)	//关闭状态 -> 打开运行状态
					{
						TO = WINDOW_CTL_TIME;
						msg.msgID = SYS_MSG_WINDOW_ID;
						msg.Param = WINDOW_OPENED;
						TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);
						MotorCtr(0, MOTOR_POS_TURN);
					}
					else		//关闭运行状态 -> 打开运行状态
					{
						TObak = WINDOW_CTL_TIME - TimerUnitGetTO(&g_TimerServer, TIMER_WINDOW_CTR_ID);
						TO = WINDOW_WAIT_TIME;
						msg.msgID = SYS_MSG_WINDOW_ID;
						msg.Param = WINDOW_WAIT;
						TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);
						MotorCtr(0, MOTOR_STOP_TURN);
						g_RunState[0].BitState.wait = 1;
					}
					g_RunState[0].sta = 0;
					g_RunState[0].BitState.opening = 1;
					
					LedSetLevel(LED_CLOSE_ID, LOW, true);
					LedSetLevel(LED_OPEN_ID, HIGH, true);
				}
			}
			break;
		case WINDOW_CLOSING:		//正在关闭
			Log("WINDOW_CLOSING\r\n");
			if(g_RunState[0].BitState.wait)
			{
				break;
			}
			if(g_RunState[0].BitState.pause)		//暂停状态 -> 关闭运行状态
			{
				if(g_RunState[0].BitState.closing) //关闭运行状态-> 暂停状态 -> 关闭运行状态
				{
					MotorCtr(0, MOTOR_NEG_TURN);	
					TimerUnitEnable(&g_TimerServer, TIMER_WINDOW_CTR_ID, true);
				}
				if(g_RunState[0].BitState.opening) //打开运行状态-> 暂停状态 -> 关闭运行状态
				{
					TO = WINDOW_CTL_TIME - TimerUnitGetTO(&g_TimerServer, TIMER_WINDOW_CTR_ID);					
					msg.msgID = SYS_MSG_WINDOW_ID;
					msg.Param = WINDOW_CLOSED;
					TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);
					MotorCtr(0, MOTOR_NEG_TURN);
				}
				g_RunState[0].sta = 0;
				g_RunState[0].BitState.closing = 1;
				
				LedSetLevel(LED_OPEN_ID, LOW, true);
				LedSetLevel(LED_CLOSE_ID, HIGH, true);
				LedSetLevel(LED_PAUSE_ID, LOW, true);
			}
			else
			{
				if((g_RunState[0].BitState.opened) || (g_RunState[0].BitState.opening))	//从打开或打开运行状态到关闭运行状态
				{
					if(g_RunState[0].BitState.opened)
					{
						TO = WINDOW_CTL_TIME;
						msg.msgID = SYS_MSG_WINDOW_ID;
						msg.Param = WINDOW_CLOSED;
						TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);
						MotorCtr(0, MOTOR_NEG_TURN);
					}
					else			//打开运行状态 -> 关闭运行状态
					{
						TObak = WINDOW_CTL_TIME - TimerUnitGetTO(&g_TimerServer, TIMER_WINDOW_CTR_ID);

						TO = WINDOW_WAIT_TIME;
						msg.msgID = SYS_MSG_WINDOW_ID;
						msg.Param = WINDOW_WAIT;
						TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);
						MotorCtr(0, MOTOR_STOP_TURN);
						g_RunState[0].BitState.wait = 1;
					}
					g_RunState[0].sta = 0;
					g_RunState[0].BitState.closing = 1;
					
					LedSetLevel(LED_OPEN_ID, LOW, true);
					LedSetLevel(LED_CLOSE_ID, HIGH, true);
				}
			}
			break;
			
		case WINDOW_OPENED:				//打开完成
			Log("WINDOW_OPENED\r\n");
			g_RunState[0].sta = 0;
			g_RunState[0].BitState.opened = 1;
			MotorCtr(0, MOTOR_STOP_TURN);
			QMsgPostSimple(&g_QMsg, SYS_MSG_WIFI_ID, WIFI_UPLOAD);
	
			LedSetLevel(LED_OPEN_ID, LOW, true);
			LedSetLevel(LED_CLOSE_ID, LOW, true);
			break;
			
		case WINDOW_CLOSED:				//关闭完成
			Log("WINDOW_CLOSED\r\n");
			g_RunState[0].sta = 0;
			g_RunState[0].BitState.closed = 1;
			MotorCtr(0, MOTOR_STOP_TURN);
			QMsgPostSimple(&g_QMsg, SYS_MSG_WIFI_ID, WIFI_UPLOAD);
			
			LedSetLevel(LED_OPEN_ID, LOW, true);
			LedSetLevel(LED_CLOSE_ID, LOW, true);
			break;

		case WINDOW_PAUSE:
			Log("WINDOW_PAUSE\r\n");
			if(g_RunState[0].BitState.pause)
			{
				Log("UnPause\r\n");
				if((g_RunState[0].BitState.opening) || (g_RunState[0].BitState.closing))
				{
					if(g_RunState[0].BitState.opening) //打开运行状态-> 暂停状态 ->           取消暂停 -> 打开运行状态
					{
						MotorCtr(0, MOTOR_POS_TURN);	
						LedSetLevel(LED_OPEN_ID, HIGH, true);
						
					}
					else					 //关闭运行状态-> 暂停状态 -> 取消暂停               -> 关闭运行状态
					{
						MotorCtr(0, MOTOR_NEG_TURN);
						LedSetLevel(LED_CLOSE_ID, HIGH, true);
						
					}
					TimerUnitEnable(&g_TimerServer, TIMER_WINDOW_CTR_ID, true);
				}			
				g_RunState[0].BitState.pause = 0;			
				LedSetLevel(LED_PAUSE_ID, LOW, true);
			}
			else
			{
				if((g_RunState[0].BitState.opening) || (g_RunState[0].BitState.closing))
				{
					Log("Pause\r\n");
					g_RunState[0].BitState.pause = 1;
					MotorCtr(0, MOTOR_STOP_TURN);	
					TimerUnitEnable(&g_TimerServer, TIMER_WINDOW_CTR_ID, false);
					
					LedSetLevel(LED_PAUSE_ID, HIGH, true);
					LedSetLevel(LED_OPEN_ID, LOW, true);
					LedSetLevel(LED_CLOSE_ID, LOW, true);
				}
			}
			break;
		case WINDOW_WAIT:
			if((g_RunState[0].BitState.opening) || (g_RunState[0].BitState.closing))
			{
				TO = TObak;
				msg.msgID = SYS_MSG_WINDOW_ID;
				if(g_RunState[0].BitState.opening)
				{
					msg.Param = WINDOW_OPENED;
					TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);	
					MotorCtr(0, MOTOR_POS_TURN);
				}
				else
				{
					msg.Param = WINDOW_CLOSED;
					TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);	
					MotorCtr(0, MOTOR_NEG_TURN);
				}
			}
			g_RunState[0].BitState.wait = 0;
			break;
			
		default:
			break;
	}
}


