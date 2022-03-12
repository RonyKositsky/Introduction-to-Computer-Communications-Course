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
		SenderUtils_ReadingFile();

		SocketTools_SendMessageSize(SenderParams_s.socket, SenderParams_s.message_size);
		SocketTools_ReadMessageSize(SenderParams_s.socket, &SenderParams_s.ack);

		SocketTools_SendMessage(SenderParams_s.socket, SenderParams_s.sent_message, SenderParams_s.message_size);

		closesocket(SenderParams_s.socket);
		free(SenderParams_s.sent_message);

		SenderUtils_PrintOutput();
		SenderUtils_InitSession();
		SenderParams_s.quit = true;
	}

	return 0;
}





