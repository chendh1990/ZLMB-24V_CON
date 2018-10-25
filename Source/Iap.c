#define __IAP_C__
#include "Iap.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
void IAP_ByteWrite(uint16 byteAddr, uint8 dat)
{
	set_IAPEN;
	set_APUEN;
	IAPAL = LOBYTE(byteAddr);
	IAPAH = HIBYTE(byteAddr);
	IAPCN = BYTE_WRITE_AP;
	IAPFD = dat;
	set_IAPGO;
	clr_APUEN;
	clr_IAPEN;
}
uint8 IAP_ByteRead(uint16 byteAddr)
{
	uint8 dat;
	set_IAPEN;
	IAPAL = LOBYTE(byteAddr);
	IAPAH = HIBYTE(byteAddr);
	IAPCN = BYTER_READ_AP;
	set_IAPGO;
	dat = IAPFD;
	clr_IAPEN;
	return dat;
} 

void IAP_PageWrite(uint16 pageAddr, const uint8 dat[], uint16 length)
{
	uint16 i;
	set_IAPEN;
	set_APUEN;

	IAPAL = LOBYTE(pageAddr);
	IAPAH = HIBYTE(pageAddr);
	IAPCN = BYTE_WRITE_AP;
	for(i = 0; i < length; i++)
	{
		IAPFD = dat[i];
		set_IAPGO;
		while((CHPCON&SET_BIT6)==SET_BIT6);

		IAPAL++;
		if(IAPAL == 0)
		{
			IAPAH++;
		}
	}
	clr_APUEN;
	clr_IAPEN;
}

void IAP_PageRead(uint16 pageAddr, uint8 dat[], uint16 length)
{
	uint16 i;
	set_IAPEN;

	IAPAL = LOBYTE(pageAddr);
	IAPAH = HIBYTE(pageAddr);
	IAPCN = BYTE_WRITE_AP;
	for(i = 0; i < length; i++)
	{
		set_IAPGO;
		
		dat[i] = IAPFD;
		IAPAL++;
		if(IAPAL == 0)
		{
			IAPAH++;
		}
	}
	clr_IAPEN;
}

void IAP_PageErase(uint16 pageAddr)
{
	set_IAPEN;
	set_APUEN;
	IAPAL = LOBYTE(pageAddr);
	IAPAH = HIBYTE(pageAddr);
	IAPFD = 0xff;
	IAPCN = PAGE_ERASE_AP;
	set_IAPGO;
	while((CHPCON&SET_BIT6)==SET_BIT6);
	clr_APUEN;
	clr_IAPEN;
}
void IAP_PageEraseAllAppRom(void)
{
	uint8 i;
	for(i = 0; i < (14*1024)/PAGE_SIZE; i++)
	{
		IAP_PageErase(i*PAGE_SIZE);
	}
}
void ProgramCONFIG(uint8 addr, uint8 value)
{
	uint8 cf[5];
#define     CFG_READ            0xc0
#define     CFG_ERASE           0xe2
#define     CFG_BYTE_PROGRAM    0xe1
	if(addr > 4)
	{
			return ;
	}
	set_IAPEN;
	IAPAL = 0x00;
	IAPAH = 0x00;
	IAPCN = CFG_READ;
	set_IAPGO;                                  //Storage CONFIG0 data
	cf[0] = IAPFD;
	IAPAL = 0x01;
	set_IAPGO;                                  //Storage CONFIG1 data
	cf[1] = IAPFD;
	IAPAL = 0x02;
	set_IAPGO;                                  //Storage CONFIG2 data
	cf[2] = IAPFD;
	IAPAL = 0x03;
	set_IAPGO;                                  //Storage CONFIG3 data
	cf[3] = IAPFD;
	IAPAL = 0x04;
	set_IAPGO;                                  //Storage CONFIG4 data
	cf[4] = IAPFD;

	cf[addr] = value;
	
	set_CFUEN;	
	IAPCN = CFG_ERASE;													//Erase CONFIG all
	IAPAH = 0x00;
	IAPAL = 0x00;
	IAPFD = 0xFF;
	set_IAPGO;

	IAPCN = CFG_BYTE_PROGRAM;										//Write CONFIG
	IAPFD = cf[0];
	set_IAPGO;
	IAPAL = 0x01;
	IAPFD = cf[1];
	set_IAPGO;
	IAPAL = 0x02;
	IAPFD = cf[2];
	set_IAPGO;
	IAPAL = 0x03;
	IAPFD = cf[3];
	set_IAPGO;
	IAPAL = 0x04;
	IAPFD = cf[4];
	set_IAPGO;

	clr_CFUEN;
	clr_IAPEN;
}
void SoftResetLdRomStart(void)
{
	ProgramCONFIG(0x00, 0x7d);		
	set_BS;
	set_SWRST;	//soft reset
}
