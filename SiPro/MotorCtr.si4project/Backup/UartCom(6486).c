#define __UART_COM_C__
#include "UartCom.h"
#include "PacketQueue.h"
#include "Common.h"
#include "Uart.h"
#include "Log.h"


void UartPacketParse(void)
{
  	uint8 dat;
	if(QueuePend(&dat) != 0)
	{	
		return ;
	}
	
	putchar(getHex(HI_UINT8(dat)));
	putchar(getHex(LO_UINT8(dat)));
	putchar(' ');
	switch(isrCfg.rxStatus)
	{
		case RX_IDDLE:
		{
			if(dat == P_HEAD_0)
			{
				isrCfg.rxStatus = RX_HEAD_0;
			}
			break;
		}

		case RX_HEAD_0:
		{
			if(dat == P_HEAD_1)
			{
				isrCfg.rxStatus = RX_HEAD_1;
			}
			else
			{
				isrCfg.rxStatus = RX_IDDLE;
			}
			break;
		}	
		case RX_HEAD_1:
		{
			if(dat == P_VER)
			{
				isrCfg.rxStatus = RX_VER;
			}
			else
			{
				isrCfg.rxStatus = RX_IDDLE;
			}
			break;
		}
		case RX_VER:
		{
			isrCfg.rxPack.cmd = dat;	//CMD
			isrCfg.rxStatus = RX_CMD;
			break;
		}
		case RX_CMD:
		{
			isrCfg.rxPack.length[0] = dat;	//LENGTH0
			isrCfg.rxStatus = RX_LEN_0;

			break;
		}
		case RX_LEN_0:
		{
			isrCfg.rxPack.length[1] = dat;	//LENGTH1
			isrCfg.rxLength = BUILD_UINT16(isrCfg.rxPack.length[1], isrCfg.rxPack.length[0]);
			if(isrCfg.rxLength <= (UART_ISR_RX_MAX - 4))
			{
				if(isrCfg.rxLength != 0)
				{
					isrCfg.rxStatus = RX_BUSY;
				}
				else
				{
					isrCfg.rxStatus = RX_VERIFY;
				}
			}
			else
			{
				isrCfg.rxStatus = RX_IDDLE;	
				isrCfg.rxCnt = 0;
				isrCfg.rxLength = 0;
			}
			break;
		}
		case RX_BUSY:
		{
			putchar('7');
			putchar('\r');
			putchar('\n');
			isrCfg.rxPack.dat[isrCfg.rxCnt++] = dat;
			if(isrCfg.rxCnt >= (isrCfg.rxLength))
			{
				isrCfg.rxStatus = RX_VERIFY;
			};
			break;
		}
		case RX_VERIFY:
		{uint8 dat1;
			putchar('8');
			putchar('\r');
			putchar('\n');
			putchar(getHex(HI_UINT8(dat)));
			putchar(getHex(LO_UINT8(dat)));
			putchar('\r');
			putchar('\n');

				UartSendHexBuffer(UART_WIFI, "rx sum:", (uint8  *)(&(dat)), 1, ' ', true);
			if((dat1 = getCheckSum((uint8  *)(&(isrCfg.rxPack)), isrCfg.rxCnt+3)+SUM_HEAD_VER) == dat)
			{
				uint8 XDATA s[] =  "+verify ok!\r\n";
					UartSendHexBuffer(UART_WIFI, "+results:", (uint8  *)(&(dat1)), 1, ' ', true);
				UartSendString(UART_WIFI, s);
				//UartPackethandle();
				UartSendHexBuffer(UART_WIFI, "+raw:", (uint8  *)(&(isrCfg.rxPack)), isrCfg.rxCnt+3, ' ', true);
			
			}
			else
			{
				uint8 XDATA s[] =  "-verify fail!\r\n";
				UartSendHexBuffer(UART_WIFI, "-results:", (uint8  *)(&(dat1)), 1, ' ', true);
				UartSendString(UART_WIFI, s);
				UartSendHexBuffer(UART_WIFI, "-raw:", (uint8  *)(&(isrCfg.rxPack)), isrCfg.rxCnt+3, ' ', true);
			}
			isrCfg.rxStatus = RX_IDDLE;
			isrCfg.rxCnt = 0;
			break;
		}
		default:
		{
			isrCfg.rxStatus = RX_IDDLE;
			isrCfg.rxCnt = 0;
			break;
		}
	}   
}
/*****************************************************************************
�������� : data_handle
�������� : ����֡����
������� : offset:������ʼλ
���ز��� : ��
*****************************************************************************/
/*
void UartPackethandle(void)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
  unsigned char *firmware_addr;
  static unsigned long firm_length;                                             //MCU�����ļ�����
  static unsigned char firm_update_flag;                                        //MCU������־
  unsigned long dp_len;
#else
  unsigned short dp_len;
#endif
  
  unsigned char ret;
  unsigned short i,total_len;
  unsigned char cmd_type = wifi_uart_rx_buf[offset + FRAME_TYPE];

#ifdef WIFI_TEST_ENABLE
  unsigned char result;
  unsigned char rssi;
#endif
  
  switch(cmd_type)
  {
  case HEAT_BEAT_CMD:                                   //������
    heat_beat_check();
    break;
    
  case PRODUCT_INFO_CMD:                                //��Ʒ��Ϣ
    product_info_update();
    break;
    
  case WORK_MODE_CMD:                                   //��ѯMCU�趨��ģ�鹤��ģʽ
    get_mcu_wifi_mode();
    break;
    
#ifndef WIFI_CONTROL_SELF_MODE
  case WIFI_STATE_CMD:                                  //wifi����״̬	
    wifi_work_state = wifi_uart_rx_buf[offset + DATA_START];
    wifi_uart_write_frame(WIFI_STATE_CMD,0);
    break;

  case WIFI_RESET_CMD:                                  //����wifi(wifi���سɹ�)
    reset_wifi_flag = RESET_WIFI_SUCCESS;
    break;
    
  case WIFI_MODE_CMD:                                   //ѡ��smartconfig/APģʽ(wifi���سɹ�)	
    set_wifimode_flag = SET_WIFICONFIG_SUCCESS;
    break;
#endif
    
  case DATA_QUERT_CMD:                                  //�����·�
    total_len = wifi_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
    total_len += wifi_uart_rx_buf[offset + LENGTH_LOW];
    
    for(i = 0;i < total_len;)
    {
      dp_len = wifi_uart_rx_buf[offset + DATA_START + i + 2] * 0x100;
      dp_len += wifi_uart_rx_buf[offset + DATA_START + i + 3];
      //
      ret = data_point_handle((unsigned char *)wifi_uart_rx_buf + offset + DATA_START + i);
      
      if(SUCCESS == ret)
      {
        //�ɹ���ʾ
      }
      else
      {
        //������ʾ
      }
      
      i += (dp_len + 4);
    }
    
    break;
    
  case STATE_QUERY_CMD:                                 //״̬��ѯ
    all_data_update();                               
    break;
    
#ifdef SUPPORT_MCU_FIRM_UPDATE
  case UPDATE_START_CMD:                                //������ʼ
    firm_length = wifi_uart_rx_buf[offset + DATA_START];
    firm_length <<= 8;
    firm_length |= wifi_uart_rx_buf[offset + DATA_START + 1];
    firm_length <<= 8;
    firm_length |= wifi_uart_rx_buf[offset + DATA_START + 2];
    firm_length <<= 8;
    firm_length |= wifi_uart_rx_buf[offset + DATA_START + 3];
    //
    wifi_uart_write_frame(UPDATE_START_CMD,0);
    firm_update_flag = UPDATE_START_CMD;
     break;
    
  case UPDATE_TRANS_CMD:                                //��������
    if(firm_update_flag == UPDATE_START_CMD)
    {
      //ֹͣһ�������ϱ�
      stop_update_flag = ENABLE;                                                 
      
      total_len = wifi_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
      total_len += wifi_uart_rx_buf[offset + LENGTH_LOW];
      
      dp_len = wifi_uart_rx_buf[offset + DATA_START];
      dp_len <<= 8;
      dp_len |= wifi_uart_rx_buf[offset + DATA_START + 1];
      dp_len <<= 8;
      dp_len |= wifi_uart_rx_buf[offset + DATA_START + 2];
      dp_len <<= 8;
      dp_len |= wifi_uart_rx_buf[offset + DATA_START + 3];
      
      firmware_addr = (unsigned char *)wifi_uart_rx_buf;
      firmware_addr += (offset + DATA_START + 4);
      if((total_len == 4) && (dp_len == firm_length))
      {
        //���һ��
        ret = mcu_firm_update_handle(firmware_addr,dp_len,0);
        
        firm_update_flag = 0;
      }
      else if((total_len - 4) <= FIRM_UPDATA_SIZE)
      {
        ret = mcu_firm_update_handle(firmware_addr,dp_len,total_len - 4);
      }
      else
      {
        firm_update_flag = 0;
        ret = ERROR;
      }
      
      if(ret == SUCCESS)
      {
        wifi_uart_write_frame(UPDATE_TRANS_CMD,0);
      }
      //�ָ�һ�������ϱ�
      stop_update_flag = DISABLE;    
    }
    break;
#endif      
    
#ifdef SUPPORT_MCU_RTC_CHECK
  case GET_LOCAL_TIME_CMD:                             //��ȡ����ʱ��
      mcu_write_rtctime(wifi_uart_rx_buf + offset + DATA_START);
    break;
#endif
 
#ifdef WIFI_TEST_ENABLE
  case WIFI_TEST_CMD:                                   //wifi���ܲ���
    result = wifi_uart_rx_buf[offset + DATA_START];
    rssi = wifi_uart_rx_buf[offset + DATA_START + 1];
    wifi_test_result(result, rssi);
    break;
#endif
    
  default:
    break;
  }
}*/

