/*
******************************************************************************
\file Sender.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "SenderUtils.h"
#include "../Utilities/Definitions.h"
#include <WinSock2.h>

/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[])
{
	SenderUtils_SenderInit(argv);
	SenderUtils_GetFileName();

	// As said, we can assume that we will get blocks of MSG_SIZE.
	while (SenderUtils_ReadBytesFromFile() == MSG_SIZE)
	{ 
		SenderUtils_AddHammCode();
		SenderUtils_AppendToBuffer();
	}

	SenderUtils_SenderTearDown();
	return 0;
}





