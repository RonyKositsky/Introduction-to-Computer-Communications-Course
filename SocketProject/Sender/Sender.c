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
#include "../Utilities/SocketTools.h"
#include "../Utilities/Definitions.h"
#include <WinSock2.h>

/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[])
{
	SenderUtils_SenderInit(argv);

	while (!SenderParams_s.quit)
	{
		// As said, we can assume that we will get blocks of MSG_SIZE.
		while (SenderUtils_ReadBytesFromFile() == MSG_SIZE)
		{
			SenderUtils_AddHammCode();
			SocketTools_SendMessage(SenderParams_s.socket, SenderParams_s.messageHamming);
		}

		SocketTools_SendMessage(SenderParams_s.socket, TERMINATION_MESSAGE);
		closesocket(SenderParams_s.socket);
		SenderUtils_InitSession();
		SenderParams_s.quit = true;
	}
	SocketTools_SendMessage(SenderParams_s.socket, QUIT);
	SenderUtils_SenderTearDown();
	return 0;
}





