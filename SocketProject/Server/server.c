
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

/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[]){
	
	int bytesRecived;
	ServerUtils_ServerInit(argv);
	while (1)
	{
		//bytesRecived = ServerUtils_WaitForMessage();
		//if (bytesRecived == QUIT)
		//{
		//	// TODO: Implement quit.
		//	break;
		//} 
		//else 
		//{
		//	ServerUtils_HandleMessage(bytesRecived);
		//}
		ServerUtils_WaitForMessage();
	}

	ServerUtils_ServerTearDown();
	return 0;
}