//Send files then wait for a ready response
//Part of a calculator transport program

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
#define MIN_AMS 200           // Compile for AMS 2.00 or higher

#include <tigcclib.h>         // Include All Header Files

void _main(void)
{
	unsigned int numVars; //Number of vars to send
	const char** varNames; //Names of vars to send

	ESI argptr = top_estack;
	numVars = RemainingArgCnt(argptr);
	varNames = alloca(sizeof(char*) * numVars);
	unsigned int i;
	for (i = 0; RemainingArgCnt(argptr) > 0; i++)
	{
		if (GetArgType(argptr) == STRING_TAG)
			varNames[i] = GetStrnArg(argptr);
		else
		{
			SkipArg(argptr);
			numVars--;
		}
	}
	LIO_SendData(&numVars, sizeof(int));

	for (i = 0; i < numVars; i++)
	{
		LIO_CTX file;
		SYM_ENTRY* symPtr;
		char message[100];

		symPtr = SymFindPtr(SYMSTR(varNames[i]), 0);
		if (symPtr == NULL)
		{
			//Uh oh...
			sprintf(message, "The requested file \"%s\" was not found. Continue transfer?", varNames[i]);
			short result = DlgMessage("Error", message, BT_YES, BT_NO);
			if (result == KEY_ENTER)
				continue;
			else
				break;
		}

		CESI heapPtr = (CESI)HeapDeref(symPtr->handle);
		file.hVar = symPtr->handle;
		file.VarName = varNames[i];
		file.VarSize = (unsigned short)HeapSize(symPtr->handle);
		file.Index = 0;
		file.Unknown = 0;
		file.VarType = (unsigned char)GetDataType(heapPtr);
		file.VarCompat = (unsigned char)symPtr->compat;
		LIO_Send(&file, 0x98);
		LIO_RecvData(message, 4, 60); //Wait for reply
		message[4] = '\0'; //Make null terminated string
		if (strcmp(message, "Erro") == 0)
		{
			//We have an error
			sprintf(message, "There was an error sending the file \"%s\". Continue transfer?", varNames[i]);
			short result = DlgMessage("Error", message, BT_YES, BT_NO);
			if (result == KEY_ESC)
				break;
		}
	}
}
