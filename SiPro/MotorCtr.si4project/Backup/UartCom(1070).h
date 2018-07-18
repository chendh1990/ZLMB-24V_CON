#ifndef __UART_COM_H__
#define __UART_COM_H__
#include "Typedef.h"
#include "QMsg.h"

#ifndef __UART_COM_C__
	#define UART_COM_EXTERN	extern
#else
	#define UART_COM_EXTERN
#endif

/***********************�궨��****************************/
#define P_HEAD_0		0x55
#define P_HEAD_1		0xaa
#define P_VER			0x00

#define SUM_HEAD_VER	(P_HEAD_0+P_HEAD_1+P_VER)

#define UART_RX_MAX  10
#define UART_TX_MAX  10


//=============================================================================
//����֡����
//=============================================================================
#define         HEAT_BEAT_CMD                   0                               //������
#define         PRODUCT_INFO_CMD                1                               //��Ʒ��Ϣ
#define         WORK_MODE_CMD                   2                               //��ѯMCU �趨��ģ�鹤��ģʽ	
#define         WIFI_STATE_CMD                  3                               //wifi����״̬	
#define         WIFI_RESET_CMD                  4                               //����wifi
#define         WIFI_MODE_CMD                   5                               //ѡ��smartconfig/APģʽ	
#define         DATA_QUERT_CMD                  6                               //�����·�
#define         STATE_UPLOAD_CMD                7                               //״̬�ϱ�	 
#define         STATE_QUERY_CMD                 8                               //״̬��ѯ   
#define         UPDATE_QUERY_CMD                9                               //������ѯ
#define         UPDATE_START_CMD                0x0a                            //������ʼ
#define         UPDATE_TRANS_CMD                0x0b                            //��������
#define         GET_ONLINE_TIME_CMD             0x0c                            //��ȡϵͳʱ��(��������ʱ��)
#define         FACTORY_MODE_CMD                0x0d                            //�������ģʽ    
#define         WIFI_TEST_CMD                   0x0e                            //wifi���ܲ���
#define         GET_LOCAL_TIME_CMD              0x1c                            //��ȡ����ʱ��                         




/***********************���ݽṹ����************************/
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


/********************ȫ�ֱ��������붨��***********************/
UART_COM_EXTERN UartISRCfg_t XDATA isrCfg;


/***************************��������***************************/
UART_COM_EXTERN void UartPacketInit(void);

UART_COM_EXTERN void UartPacketParse(void);

#endif
