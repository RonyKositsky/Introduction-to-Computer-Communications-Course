
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

/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[])
{
	ServerUtils_ServerInit(argv);
	while (!ServerParams_s.quit)
	{
		while (ServerParams_s.messageHamming != TERMINATION_MESSAGE)
		{

		}

		SocketTools_ReadMessage(ServerParams_s.accepted_socket, &ServerParams_s.messageHamming);
		closesocket(ServerParams_s.accepted_socket);
		closesocket(ServerParams_s.socket);

		if (ServerParams_s.messageHamming == QUIT)
		{
			ServerUtils_ServerTearDown();
			break;
		}

		ServerUtils_SessionInit();
	}

	return 0;
}