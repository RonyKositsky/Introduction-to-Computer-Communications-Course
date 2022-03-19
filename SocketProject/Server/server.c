
/*!
******************************************************************************
\file Server.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "ServerUtils.h"
#include "../Utilities/Definitions.h"
#include "../Utilities/SocketTools.h"

/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[])
{
	ServerUtils_ServerInit(argv);
	while (!ServerParams_s.quit)
	{
		// Get message size.
		SocketTools_ReadMessageSize(ServerParams_s.socket, &ServerParams_s.message_size);
		SocketTools_SendMessageSize(ServerParams_s.socket, ACK);

		// Read message.
		ServerParams_s.message = (char*)malloc(ServerParams_s.message_size * sizeof(char));
		SocketTools_ReadMessage(ServerParams_s.socket, ServerParams_s.message ,ServerParams_s.message_size);

		// Write to file.
		ServerUtils_WriteToFile();

		// Closing procedure.
		closesocket(ServerParams_s.socket);
		
		ServerUtils_PrintOutput();
		ServerUtils_SessionInit();
	}

	return 0;
}