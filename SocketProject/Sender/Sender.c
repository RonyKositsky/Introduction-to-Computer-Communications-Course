/*!
******************************************************************************
\file Sender.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
ALL RIGHTS RESERVED
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "SenderUtils.h"
#include "../Utilities/Definitions.h"

/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[])
{
	SenderUtils_SenderInit(argv);
	
	// as long as we keep reading 11 bits from file (file is not empty and read is successful)
	while (SenderUtils_ReadBytesFromFile() == MSG_SIZE)
	{ //problem with size of sender buffer
		SenderUtils_AddHammCode();
		SenderUtils_AppendToBuffer();
	}

	SenderUtils_SenderTearDown();
	return 0;
}





