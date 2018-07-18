#define __PERIPHERAL_C__

#include "N76E003.h"
#include "Function_define.h"
#include "Typedef.h"
#include "Key.h"
#include "Peripheral.h"




void KeyInit(void)

{

	P03_Input_Mode;

	P04_Input_Mode;

	P05_Input_Mode;

}
static void LedInit()
{
	;
}
void PeriphralInit(void)
{
	KeyInit();
}
