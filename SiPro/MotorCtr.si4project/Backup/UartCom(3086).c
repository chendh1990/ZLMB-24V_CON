#define __UART_COM_C__
#include "UartCom.h"
#include "PacketQueue.h"
#include "Common.h"
#include "Uart.h"
#include "Log.h"

static void WifiPacketHandle(void);
static void HeatBeatCheck(void);
static void ProductInfoUpdate(void);
static void SetTxPacketData(uint8 const* const d, uint8 XDATA length);
static void SendTxPacketData(uint8 XDATA cmd);


void WifiPacketInit(void)
{
	s_WifiPacketCfg.rxCnt = 0;
	s_WifiPacketCfg.rxStatus = RX_IDDLE;
	s_WifiPacketCfg.rxPack.head_0 = P_HEAD_0;
	s_WifiPacketCfg.rxPack.head_1 = P_HEAD_1;
	s_WifiPacketCfg.rxPack.ver = P_VER;
	
	s_WifiPacketCfg.txCnt = 0;
	s_WifiPacketCfg.txStatus = RX_IDDLE;
	s_WifiPacketCfg.txPack.head_0 = P_HEAD_0;
	s_WifiPacketCfg.txPack.head_1 = P_HEAD_1;
	s_WifiPacketCfg.txPack.ver = P_VER;
	s_WifiPacketCfg.txLength = 0;
}
void WifiPacketParse(void)
{
  	uint8 XDATA dat;
	uint8 XDATA checkSum;
	if(QueuePend(&dat) != 0)
	{	
		return ;
	}
	putchar(getHex(HI_UINT8(dat)));
	putchar(getHex(LO_UINT8(dat)));
	putchar(' ');
	switch(s_WifiPacketCfg.rxStatus)
	{
		case RX_IDDLE:
		{
			if(dat == P_HEAD_0)
			{
				s_WifiPacketCfg.rxStatus = RX_HEAD_0;
			}
			break;
		}

		case RX_HEAD_0:
		{
			if(dat == P_HEAD_1)
			{
				s_WifiPacketCfg.rxStatus = RX_HEAD_1;
			}
			else
			{
				s_WifiPacketCfg.rxStatus = RX_IDDLE;
			}
			break;
		}	
		case RX_HEAD_1:
		{
			if(dat == P_VER)
			{
				s_WifiPacketCfg.rxStatus = RX_VER;
			}
			else
			{
				s_WifiPacketCfg.rxStatus = RX_IDDLE;
			}
			break;
		}
		case RX_VER:
		{
			s_WifiPacketCfg.rxPack.cmd = dat;	//CMD
			s_WifiPacketCfg.rxStatus = RX_CMD;
			break;
		}
		case RX_CMD:
		{
			s_WifiPacketCfg.rxPack.length[0] = dat;	//LENGTH0
			s_WifiPacketCfg.rxStatus = RX_LEN_0;

			break;
		}
		case RX_LEN_0:
		{
			s_WifiPacketCfg.rxPack.length[1] = dat;	//LENGTH1
			s_WifiPacketCfg.rxLength = BUILD_UINT16(s_WifiPacketCfg.rxPack.length[1], s_WifiPacketCfg.rxPack.length[0]);
			if(s_WifiPacketCfg.rxLength <= (UART_PACKET_DAT_MAX))
			{
				if(s_WifiPacketCfg.rxLength != 0)
				{
					s_WifiPacketCfg.rxStatus = RX_BUSY;
				}
				else
				{
					s_WifiPacketCfg.rxStatus = RX_VERIFY;
				}
			}
			else
			{
				s_WifiPacketCfg.rxStatus = RX_IDDLE;	
				s_WifiPacketCfg.rxCnt = 0;
				s_WifiPacketCfg.rxLength = 0;
			}
			break;
		}
		case RX_BUSY:
		{
		//	putchar('7');
		//	putchar('\r');
		//	putchar('\n');
			s_WifiPacketCfg.rxPack.dat[s_WifiPacketCfg.rxCnt++] = dat;
			if(s_WifiPacketCfg.rxCnt >= (s_WifiPacketCfg.rxLength))
			{
				s_WifiPacketCfg.rxStatus = RX_VERIFY;
			};
			break;
		}
		case RX_VERIFY:
		{
		//	putchar('8');
		//	putchar('\r');
		//	putchar('\n');
		//	putchar(getHex(HI_UINT8(dat)));
		//	putchar(getHex(LO_UINT8(dat)));
		//	putchar('\r');
		//	putchar('\n');

			checkSum = getCheckSum((uint8  *)(&(s_WifiPacketCfg.rxPack.head_0)), s_WifiPacketCfg.rxCnt+6);
		//	checkSum = SUM_HEAD_VER;
		
				UartSendHexBuffer(UART_WIFI, "rx sum:", (uint8  *)(&(dat)), 1, ' ', true);
			if(checkSum == dat)
			{
				uint8 XDATA s[] =  "+verify ok!\r\n";
				//	UartSendHexBuffer(UART_WIFI, "+results:", (uint8  *)(&(checkSum)), 1, ' ', true);
				UartSendString(UART_WIFI, s);
				WifiPacketHandle();
			//	UartSendHexBuffer(UART_WIFI, "+raw:", (uint8  *)(&(s_WifiPacketCfg.rxPack.cmd)), s_WifiPacketCfg.rxCnt+3, ' ', true);
			
			}
			else
			{
				uint8 XDATA s[] =  "-verify fail!\r\n";
				UartSendHexBuffer(UART_WIFI, "-results:", (uint8  *)(&(checkSum)), 1, ' ', true);
				checkSum = s_WifiPacketCfg.rxPack.dat[0] + s_WifiPacketCfg.rxPack.dat[1] + s_WifiPacketCfg.rxPack.dat[2];
				UartSendHexBuffer(UART_WIFI, "--results:", (uint8  *)(&(checkSum)), 1, ' ', true);
				UartSendString(UART_WIFI, s);
				UartSendHexBuffer(UART_WIFI, "-raw:", (uint8  *)(&(s_WifiPacketCfg.rxPack.cmd)), s_WifiPacketCfg.rxCnt+3, ' ', true);
			}
			s_WifiPacketCfg.rxStatus = RX_IDDLE;
			s_WifiPacketCfg.rxCnt = 0;
			break;
		}
		default:
		{
			s_WifiPacketCfg.rxStatus = RX_IDDLE;
			s_WifiPacketCfg.rxCnt = 0;
			break;
		}
	} 
}

/*****************************************************************************
函数名称 : data_handle
功能描述 : 数据帧处理
输入参数 : offset:数据起始位
返回参数 : 无
*****************************************************************************/
void WifiPacketHandle(void)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
  unsigned char *firmware_addr;
  static unsigned long firm_length;                                             //MCU升级文件长度
  static unsigned char firm_update_flag;                                        //MCU升级标志
  unsigned long dp_len;
#else
//  unsigned short dp_len;
#endif
  
  //unsigned char ret;
  //unsigned short i,total_len;
#ifdef WIFI_TEST_ENABLE
  //unsigned char result;
  //unsigned char rssi;
#endif
  
  switch(s_WifiPacketCfg.rxPack.cmd)
  {
  case HEAT_BEAT_CMD:                                   //心跳包
	HeatBeatCheck();
	SendTxPacketData(HEAT_BEAT_CMD);
    break;
 
  case PRODUCT_INFO_CMD:                                //产品信息
    ProductInfoUpdate();
  	SendTxPacketData(PRODUCT_INFO_CMD);
    break;
 
  case WORK_MODE_CMD:                                   //查询MCU设定的模块工作模式
    SendTxPacketData(WORK_MODE_CMD);
    break;
 
  case WIFI_STATE_CMD:                                  //wifi工作状态	
    s_Wifi.CurrState = s_WifiPacketCfg.rxPack.dat[0];
    wifi_uart_write_frame(WIFI_STATE_CMD,0);
    break;

  case WIFI_RESET_CMD:                                  //重置wifi(wifi返回成功)
    reset_wifi_flag = RESET_WIFI_SUCCESS;
    break;
 #if 0   
  case WIFI_MODE_CMD:                                   //选择smartconfig/AP模式(wifi返回成功)	
    set_wifimode_flag = SET_WIFICONFIG_SUCCESS;
    break;

    
  case DATA_QUERT_CMD:                                  //命令下发
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
        //成功提示
      }
      else
      {
        //错误提示
      }
      
      i += (dp_len + 4);
    }
    
    break;
    
  case STATE_QUERY_CMD:                                 //状态查询
    all_data_update();                               
    break;
    
#ifdef SUPPORT_MCU_FIRM_UPDATE
  case UPDATE_START_CMD:                                //升级开始
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
    
  case UPDATE_TRANS_CMD:                                //升级传输
    if(firm_update_flag == UPDATE_START_CMD)
    {
      //停止一切数据上报
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
        //最后一包
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
      //恢复一切数据上报
      stop_update_flag = DISABLE;    
    }
    break;
#endif      
    
#ifdef SUPPORT_MCU_RTC_CHECK
  case GET_LOCAL_TIME_CMD:                             //获取本地时间
      mcu_write_rtctime(wifi_uart_rx_buf + offset + DATA_START);
    break;
#endif
 
#ifdef WIFI_TEST_ENABLE
  case WIFI_TEST_CMD:                                   //wifi功能测试
    result = wifi_uart_rx_buf[offset + DATA_START];
    rssi = wifi_uart_rx_buf[offset + DATA_START + 1];
    wifi_test_result(result, rssi);
    break;
#endif
#endif 
  default:
    break;
 }
}
static void HeatBeatCheck(void)
{
	uint8 XDATA length = 1;
	static uint8 XDATA resetState = FALSE;
  
	if(FALSE == resetState)
	{
		SetTxPacketData(&resetState, length);
		resetState = TRUE;
	}
	else
	{
		SetTxPacketData(&resetState, length);
	}
}
/*
static void product_info_update(void)
{
  unsigned char length = 0;
  
  length = set_wifi_uart_buffer(length,(unsigned char *)PRODUCT_KEY,my_strlen((unsigned char *)PRODUCT_KEY));
  length = set_wifi_uart_buffer(length,(unsigned char *)MCU_VER,my_strlen((unsigned char *)MCU_VER));
  
  wifi_uart_write_frame(PRODUCT_INFO_CMD, length);
}*/
static void ProductInfoUpdate(void)
{
	SetTxPacketData((uint8 const* const)(PRODUCT_INFO), my_strlen((uint8 *)PRODUCT_INFO));
}
static void get_mcu_wifi_mode(void)
{
  unsigned char length = 0;
  
#ifdef WIFI_CONTROL_SELF_MODE                                   //模块自处理
  length = set_wifi_uart_byte(length, WF_STATE_KEY);
  length = set_wifi_uart_byte(length, WF_RESERT_KEY);
#else                                                           
  //无需处理数据
#endif
  
  wifi_uart_write_frame(WORK_MODE_CMD, length);
}

static void SetTxPacketData(uint8 const* const d, uint8 XDATA length)
{
	my_memcpy(&s_WifiPacketCfg.txPack.dat, d, length);
	s_WifiPacketCfg.txLength = length;
}
static void SendTxPacketData(uint8 XDATA cmd)
{
	uint8 XDATA txLength = s_WifiPacketCfg.txLength+6;
	s_WifiPacketCfg.txPack.cmd = cmd;
	s_WifiPacketCfg.txPack.length[0] = HI_UINT16(s_WifiPacketCfg.txLength);
	s_WifiPacketCfg.txPack.length[1] = LO_UINT16(s_WifiPacketCfg.txLength);
	s_WifiPacketCfg.txPack.dat[s_WifiPacketCfg.txLength] = getCheckSum((uint8 const *)&s_WifiPacketCfg.txPack.head_0, txLength++);
//	UartSendBuffer(UART_WIFI, (const uint8 *const)&s_WifiPacketCfg.txPack.head_0, txLength);
	UartSendHexBuffer(UART_WIFI, "res:", (const uint8 *const)&s_WifiPacketCfg.txPack.head_0, txLength, ' ', true);

	s_WifiPacketCfg.txLength = 0;
}
