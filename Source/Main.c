#include "Sys.h"

void main(void)
{
	SystemVarInit();

	while(true)
	{
		SystemStatusMachine(g_SystemVar.SystemLastStatus);
	}
}
