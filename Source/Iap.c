#define __IAP_C__
#include "Iap.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"

static bit BIT_TMP;
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
void IAP_PageWrite(uint16 pageAddr, uint8 dat[], uint8 length)
{
	uint8 i;
	if(length < PAGE_SIZE)
	{
		set_IAPEN;
		set_APUEN;

		IAPAL = LOBYTE(pageAddr);
		IAPAH = HIBYTE(pageAddr);
		IAPCN = BYTE_WRITE_AP;
		for(i; i < length; i++)
		{
			IAPFD = dat[i];
			set_IAPGO;
		
	        IAPAL++;
	        if(IAPAL == 0)
	        {
	            IAPAH++;
	        }
		}
		clr_APUEN;
		clr_IAPEN;
	}
}
void IAP_PageRead(uint16 pageAddr, uint8 dat[], uint8 length)
{
	uint8 i;
	if(length < PAGE_SIZE)
	{
		set_IAPEN;

		IAPAL = LOBYTE(pageAddr);
		IAPAH = HIBYTE(pageAddr);
		IAPCN = BYTE_WRITE_AP;
		for(i; i < length; i++)
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
}
void IAP_PageErase(uint16 pageAddr)
{
	set_IAPEN;
	set_APUEN;
	IAPAL = LOBYTE(pageAddr);
	IAPAH = HIBYTE(pageAddr);
	IAPCN = PAGE_ERASE_AP;
	set_IAPGO;
	clr_APUEN;
	clr_IAPEN;
}