#ifndef __WIFI_H__
#define __WIFI_H__

#include "Typedef.h"
#include "QMsg.h"

#ifndef __WIFI_C__
	#define WIFI_EXTERN	extern
#else
	#define WIFI_EXTERN
#endif


/***********************�궨��****************************/
#define P_HEAD_0		0x55
#define P_HEAD_1		0xaa
#define P_VER			0x00

#define SUM_HEAD_VER	(P_HEAD_0+P_HEAD_1+P_VER)

#define UART_PACKET_DAT_MAX  	24


#define PRODUCT_KEY			"8ahVHV0wZHvUFVPF"    //����ƽ̨������Ʒ�����ɵ�16λ�ַ���ƷΨһ��ʶ
 
#define MCU_VER 			"1.0.0"                   //�û�������汾,����MCU�̼�����,MCU�����汾���޸�

#define PRODUCT_INFO		PRODUCT_KEY##""##MCU_VER


//����(���·����ϱ�)
//��ע:
#define DPID_SWITCH_1 		1
//��ͣ(���·����ϱ�)
//��ע:
#define DPID_SWITCH_2 		2

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

//=============================================================================
//dp���ݵ�����
//=============================================================================
#define         DP_TYPE_RAW                     0x00				//RAW��
#define         DP_TYPE_BOOL                    0x01	           	//������
#define         DP_TYPE_VALUE                   0x02	         	//��ֵ��
#define         DP_TYPE_STRING                  0x03				//�ַ�����
#define         DP_TYPE_ENUM                    0x04				//ö����
#define         DP_TYPE_FAULT                   0x05				//������

//=============================================================================
//WIFI����״̬
//=============================================================================
#define         SMART_CONFIG_STATE              0x00
#define         AP_STATE                        0x01
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define         WIFI_SATE_UNKNOW                0xff
//=============================================================================
//wifi�����ķ�ʽ
//=============================================================================
#define         SMART_CONFIG                    0x0  
#define         AP_CONFIG                       0x1   

//=============================================================================
//wifi��λ״̬
//=============================================================================
#define         RESET_WIFI_ERROR                0
#define         RESET_WIFI_SUCCESS              1

//=============================================================================
//wifi��λ״̬
//=============================================================================
#define         SET_WIFICONFIG_ERROR            0
#define         SET_WIFICONFIG_SUCCESS          1



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
	uint8 dat[UART_PACKET_DAT_MAX];
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
} WifiPacketCfg_t;

typedef struct
{
	uint8 CurrState;
	uint8 BackState;
	uint8 resetFlag;
	uint8 setModeFlag;
}Wifi_t;


typedef struct {
  uint8 dp_id;                        //dp���
  uint8 dp_type;                      //dp����
}DownloadCmd_t;


typedef enum{
	WIFI_RESET = 0,
	WIFI_SMART_CONFIG,
	WIFI_AP,
	WIFI_UPLOAD
}Wifi_Cmd_t;

/********************ȫ�ֱ��������붨��***********************/


/***************************��������***************************/
WIFI_EXTERN void WifiInit(void);

WIFI_EXTERN void WifiPacketParse(void);

WIFI_EXTERN void WifiHandle(const MSG_t *const pMsg);

WIFI_EXTERN void WifiStatusShow(void);

#endif
