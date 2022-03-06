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
SOCKET SocketTools_CreateSocket()
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

	return sockfd;
}


/*!
******************************************************************************
\brief
Creating socket address via reference.
\param
 [in] sa   - Socket address pointer.
 [in] port - Port number.
 [in] ip   - The ip as string.
\return none.
*****************************************************************************/
void SocketTools_CreateAddress(SOCKET *socket, char* ip, int port, SocketType type)
{
	struct sockaddr_in sa;
	int status;

	sa.sin_family	   = AF_INET;
	sa.sin_port		   = htons(port);
	sa.sin_addr.s_addr = inet_addr(ip);

	if (type == CLIENT)
	{
		status = connect(*socket, (SOCKADDR*)&sa, sizeof(struct sockaddr));
	}
	else
	{
		status = bind(*socket, (SOCKADDR*)&sa, sizeof(struct sockaddr));
		status = listen(*socket, SOMAXCONN);
	}
}

/*!
******************************************************************************
\brief
Reading message via socket.
\param
 [in] msgVars - The message arguments struct.
\return the number of bits recieved.
*****************************************************************************/
int SocketTools_ReadMessage(MessageVars* msgVars)
{
	int len = sizeof(msgVars->addr);
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		fprintf(stderr, "WSAStartup failed with error: %d\n", err);
		exit(-1);
	}

	int res = recvfrom(msgVars->sock, msgVars->buf, msgVars->buf_size, 0, (struct sockaddr*)msgVars->addr, &len);
	if (res < 0) 
	{
		fprintf(stderr, "read message failed\n");
		fprintf(stderr, "read message with error %d \n", WSAGetLastError());

		exit(-1);
	}
	return res;
}

/*!
******************************************************************************
\brief
Sending message via socket.
\param
 [in] msgVars - The message arguments struct.
\return the number of bits sent.
*****************************************************************************/
int SocketTools_SendMessage(MessageVars* msgVars)
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	
	int err =  WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		fprintf(stderr, "WSAStartup failed with error: %d\n", err);
		exit(-1);
	}

	char buf[5];
	strcpy(buf, "yo");

	msgVars->addr->sin_family = AF_INET;
	send(msgVars->sock, buf, 10, 0);
	//send(msgVars->sock, msgVars->buf, msgVars->buf_size, 0);
	int size = 0;
	/*if ((size = sendto(msgVars->sock, msgVars->buf, msgVars->buf_size, 0, (struct sockaddr*)msgVars->addr, sizeof(struct sockaddr))) < 0) 
	{
		fprintf(stderr, "send message faile with error: %d\n", WSAGetLastError());
		exit(-1);
	}*/
	return size;
}



