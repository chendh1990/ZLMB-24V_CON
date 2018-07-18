#ifndef __UART_COM_H__
#define __UART_COM_H__
#include "Typedef.h"
#include "QMsg.h"

#ifndef __UART_COM_C__
	#define UART_COM_EXTERN	extern
#else
	#define UART_COM_EXTERN
#endif

/***********************宏定义****************************/
#define P_HEAD_0		0x55
#define P_HEAD_1		0xaa
#define P_VER			0x00

#define SUM_HEAD_VER	(P_HEAD_0+P_HEAD_1+P_VER)

#define UART_RX_MAX  10
#define UART_TX_MAX  10


//=============================================================================
//数据帧类型
//=============================================================================
#define         HEAT_BEAT_CMD                   0                               //心跳包
#define         PRODUCT_INFO_CMD                1                               //产品信息
#define         WORK_MODE_CMD                   2                               //查询MCU 设定的模块工作模式	
#define         WIFI_STATE_CMD                  3                               //wifi工作状态	
#define         WIFI_RESET_CMD                  4                               //重置wifi
#define         WIFI_MODE_CMD                   5                               //选择smartconfig/AP模式	
#define         DATA_QUERT_CMD                  6                               //命令下发
#define         STATE_UPLOAD_CMD                7                               //状态上报	 
#define         STATE_QUERY_CMD                 8                               //状态查询   
#define         UPDATE_QUERY_CMD                9                               //升级查询
#define         UPDATE_START_CMD                0x0a                            //升级开始
#define         UPDATE_TRANS_CMD                0x0b                            //升级传输
#define         GET_ONLINE_TIME_CMD             0x0c                            //获取系统时间(格林威治时间)
#define         FACTORY_MODE_CMD                0x0d                            //进入产测模式    
#define         WIFI_TEST_CMD                   0x0e                            //wifi功能测试
#define         GET_LOCAL_TIME_CMD              0x1c                            //获取本地时间                         




/***********************数据结构定义************************/
typedef enum
{
	RX_IDDLE = 0,
	RX_HEAD_0,
	RX_HEAD_1,
	RX_VER,
	RX_CMD,
	RX_LEN_0,
	RX_LEN_1,
	RX_BUSY,
	RX_VERIFY
}UartRxState_t;

typedef struct{
	uint8 head_0;
	uint8 head_1;
	uint8 ver;
	uint8 cmd;
	uint8 length[2];
	uint8 dat[UART_TX_MAX];
}UartDataPacket_t;
typedef struct
{
	UartDataPacket_t rxPack;
	uint8 rxCnt;
	uint8 rxStatus;
	uint16 rxLength;

	UartDataPacket_t txPack;
	
	uint8 txCnt;
	uint8 txStatus;
	uint16 txLength;
} UartISRCfg_t;


/********************全局变量声明与定义***********************/
UART_COM_EXTERN UartISRCfg_t XDATA isrCfg;


/***************************函数声明***************************/
UART_COM_EXTERN void UartPacketInit(void);

UART_COM_EXTERN void UartPacketParse(void);

#endif
