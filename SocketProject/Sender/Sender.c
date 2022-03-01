/*!
******************************************************************************
\file Sender.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
ALL RIGHTS RESERVED
*****************************************************************************/
#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
	WSADATA wsaData; 
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); 
	if (iResult != NO_ERROR) 
		printf("Error at WSAStartup()\n");

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in remote_addr; 

	
	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote_addr.sin_port = htons(6342);

	int status = connect(s, (SOCKADDR*)&remote_addr, sizeof(struct sockaddr));

	char buf[5];
	strcpy(buf, "yo");


	send(s, buf, (int)strlen(buf), 0);
	int sent = send(s, buf, MSG_SIZE, 0);

	SenderUtils_SenderInit(argv);
	//SenderUtils_GetFileName();

	//// As said, we can assume that we will get blocks of MSG_SIZE.
	//while (SenderUtils_ReadBytesFromFile() == MSG_SIZE)
	//{ 
	//	SenderUtils_AddHammCode();
	//	SenderUtils_AppendToBuffer();
	//}

	//SenderUtils_SenderTearDown();
	return 0;
}





