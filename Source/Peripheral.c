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
			case MOTOR_POS_TURN:	//�����ת
				JK1_CTR_OFF;
				JK2_CTR_OFF;
				break;
			case MOTOR_NEG_TURN:	//�����ת
				JK1_CTR_ON;
				JK2_CTR_ON;
				break;
			case MOTOR_STOP_TURN:	//���ͣת
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
		case WINDOW_OPENING:			//���ڴ�
			Log("WINDOW_OPENING\r\n");
			if(g_RunState[0].BitState.wait)
			{
				break;
			}
			if(g_RunState[0].BitState.pause)		//��ͣ״̬ -> ������״̬
			{
				if(g_RunState[0].BitState.opening) //������״̬-> ��ͣ״̬ -> ������״̬
				{
					MotorCtr(0, MOTOR_POS_TURN);	
					TimerUnitEnable(&g_TimerServer, TIMER_WINDOW_CTR_ID, true);
				}
				if(g_RunState[0].BitState.closing) //�ر�����״̬-> ��ͣ״̬ -> ������״̬
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
				if((g_RunState[0].BitState.closed) || (g_RunState[0].BitState.closing)) //�ӹرջ�ر�����״̬��������״̬
				{
					if(g_RunState[0].BitState.closed)	//�ر�״̬ -> ������״̬
					{
						TO = WINDOW_CTL_TIME;
						msg.msgID = SYS_MSG_WINDOW_ID;
						msg.Param = WINDOW_OPENED;
						TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);
						MotorCtr(0, MOTOR_POS_TURN);
					}
					else		//�ر�����״̬ -> ������״̬
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
		case WINDOW_CLOSING:		//���ڹر�
			Log("WINDOW_CLOSING\r\n");
			if(g_RunState[0].BitState.wait)
			{
				break;
			}
			if(g_RunState[0].BitState.pause)		//��ͣ״̬ -> �ر�����״̬
			{
				if(g_RunState[0].BitState.closing) //�ر�����״̬-> ��ͣ״̬ -> �ر�����״̬
				{
					MotorCtr(0, MOTOR_NEG_TURN);	
					TimerUnitEnable(&g_TimerServer, TIMER_WINDOW_CTR_ID, true);
				}
				if(g_RunState[0].BitState.opening) //������״̬-> ��ͣ״̬ -> �ر�����״̬
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
				if((g_RunState[0].BitState.opened) || (g_RunState[0].BitState.opening))	//�Ӵ򿪻������״̬���ر�����״̬
				{
					if(g_RunState[0].BitState.opened)
					{
						TO = WINDOW_CTL_TIME;
						msg.msgID = SYS_MSG_WINDOW_ID;
						msg.Param = WINDOW_CLOSED;
						TimerUnitAdd(&g_TimerServer, TIMER_WINDOW_CTR_ID, &g_QMsg, &msg, TO);
						MotorCtr(0, MOTOR_NEG_TURN);
					}
					else			//������״̬ -> �ر�����״̬
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
			
		case WINDOW_OPENED:				//�����
			Log("WINDOW_OPENED\r\n");
			g_RunState[0].sta = 0;
			g_RunState[0].BitState.opened = 1;
			MotorCtr(0, MOTOR_STOP_TURN);
			QMsgPostSimple(&g_QMsg, SYS_MSG_WIFI_ID, WIFI_UPLOAD);
	
			LedSetLevel(LED_OPEN_ID, LOW, true);
			LedSetLevel(LED_CLOSE_ID, LOW, true);
			break;
			
		case WINDOW_CLOSED:				//�ر����
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
					if(g_RunState[0].BitState.opening) //������״̬-> ��ͣ״̬ ->           ȡ����ͣ -> ������״̬
					{
						MotorCtr(0, MOTOR_POS_TURN);	
						LedSetLevel(LED_OPEN_ID, HIGH, true);
						
					}
					else					 //�ر�����״̬-> ��ͣ״̬ -> ȡ����ͣ               -> �ر�����״̬
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


