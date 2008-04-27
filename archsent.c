//Archives files after they are sent
//Part of a calculator transport program

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MIN_AMS 200           // Compile for AMS 2.00 or higher

#include <tigcclib.h>         // Include All Header Files

void _main(void)
{
	unsigned int numVars; //Number of vars to recieve

	ST_helpMsg("Waiting for initialization...");
	if (LIO_RecvData(&numVars, sizeof(int), 100) != 0)
	{
		DlgMessage("Error", "Link request timed out", BT_OK, BT_NONE);
		return;
	}

	LIO_CTX* file;
	unsigned int i;
	for (i = 0; i < numVars; i++)
	{
		if (LIO_Receive(file, 0x98, 0) != 0)
		{
			LIO_SendData("Erro", 4);
			continue;
		}
		if (EM_moveSymToExtMem(SYMSTR(file->VarName), HS_NULL) == FALSE)
			LIO_SendData("Erro", 4); //Send error reply
		else
			LIO_SendData("Data", 4); //Send success reply
	}
}
