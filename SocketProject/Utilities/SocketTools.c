/*!
******************************************************************************
\file SocketTools.c
\date 24 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

/************************************
*      include                      *
************************************/
#include "SocketTools.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <winsock2.h>
#include <conio.h>
#include <ws2tcpip.h>

/************************************
*       API implementation          *
************************************/

/*!
******************************************************************************
\brief
Initializing new socket.
\return SOCKET.
*****************************************************************************/
SOCKET SocketTools_CreateSocket(char* ip, int port, SocketType type)
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData);
	if(err != 0)
	{
		fprintf(stderr, "WSAStartup failed with error: %d\n", err);
		exit(-1);
	}
	
	SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == INVALID_SOCKET)
	{
		fprintf(stderr, "Failed to create socket, error %d", WSAGetLastError());
		exit(-1);
	}

	struct sockaddr_in sa;
	int status;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(ip);

	if (type == CLIENT)
	{
		status = connect(sockfd, (SOCKADDR*)&sa, sizeof(struct sockaddr));
	}
	else
	{
		status = bind(sockfd, (SOCKADDR*)&sa, sizeof(struct sockaddr));
		status = listen(sockfd, SOMAXCONN);
	}

	return sockfd;
}



/*!
******************************************************************************
\brief
Reading message via socket.
\param
 [in] msgVars - The message arguments struct.
\return the number of bits recieved.
*****************************************************************************/
int SocketTools_ReadMessage(SOCKET socket, uint32_t *message)
{
	uint32_t rm = 0;
	int status = read(socket, &rm, sizeof(uint32_t), MSG_PEEK );
	*message = ntohl(rm);
}

/*!
******************************************************************************
\brief
Sending message via socket.
\param
 [in] msgVars - The message arguments struct.
\return the number of bits sent.
*****************************************************************************/
int SocketTools_SendMessage(SOCKET socket, uint32_t message)
{
	uint32_t un = htonl(message);
	int status = send(socket, &un, sizeof(uint32_t), 0);
}



