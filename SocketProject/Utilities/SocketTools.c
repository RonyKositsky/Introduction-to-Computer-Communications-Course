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
#include <winsock2.h>

/************************************
*       definitions		            *
************************************/
#define SUCCESS 0

/************************************
*       API implementation          *
************************************/

/*!
******************************************************************************
\brief
Initializing new socket.
\return SOCKET.
*****************************************************************************/
SOCKET SocketTools_CreateSocket(char* ip, int port, SocketType sockType, bool printData, ClientType clientType)
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	ASSERT( WSAStartup(wVersionRequested, &wsaData) == SUCCESS,  "WSAStartup failed");
	
	SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ASSERT(sockfd != INVALID_SOCKET, "Failed to create socket.");

	struct sockaddr_in sa;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(ip);
	int adrlen = sizeof(SOCKADDR);

	if (sockType == CLIENT)
	{
		ASSERT(connect(sockfd, (SOCKADDR*)&sa, sizeof(struct sockaddr)) == SUCCESS, "Error in connect() function.");
	}
	else
	{
		ASSERT(bind(sockfd, (SOCKADDR*)&sa, sizeof(struct sockaddr)) == SUCCESS, "Error in bind() function.");

		if (printData)
		{
			ASSERT(getsockname(sockfd, (SOCKADDR*)&sa, &adrlen) == SUCCESS, "Error in getsockname() function.");
			if (clientType == SENDER)
			{
				printf("sender socket : IP address = %s port = %d\n", inet_ntoa(sa.sin_addr), sa.sin_port);
			}
			else 
			{
				printf("receiver socket : IP address = %s port = %d\n", inet_ntoa(sa.sin_addr), sa.sin_port);
			}
		}

		ASSERT(listen(sockfd, SOMAXCONN) == SUCCESS, "Error in listen() function.");
	}

	return sockfd;
}

/*!
******************************************************************************
\brief
Reading message size via socket.
\param
 [in] socket    - The socket we are reading from.
 [in] message_p - Pointer to the message we will save the message to.
\return none.
*****************************************************************************/
void SocketTools_ReadMessageSize(SOCKET socket, uint32_t *message_size)
{
	uint32_t rm = 0;
	ASSERT(recv(socket, &rm, sizeof(uint32_t), 0) >= SUCCESS, "Error in recieving message size.");
	*message_size = ntohl(rm);
}

/*!
******************************************************************************
\brief
Sending message via socket.
\param
 [in] socket - the sending socket.
 [in] message_size - the size.
\return none.
*****************************************************************************/
void SocketTools_SendMessageSize(SOCKET socket, uint32_t message_size)
{
	uint32_t un = htonl(message_size);
	ASSERT(send(socket, &un, sizeof(uint32_t), 0) >= SUCCESS, "Error in sending message size.");
}

/*!
******************************************************************************
\brief
Sending message via socket.
\param
 [in] socket   - the socket we are sending to.
 [in] message  - the message.
 [in] size  - message size.
\return none.
*****************************************************************************/
void SocketTools_SendMessage(SOCKET socket, char *message, int size)
{
	ASSERT(send(socket, message, size, 0) >= SUCCESS, "Error in sending message.");
}

/*!
******************************************************************************
\brief
Reading size via socket.
\param
 [in] socket   - the socket we are reading from.
 [in] message  - message buffer.
 [in] size  - message size.
\return none.
*****************************************************************************/
void SocketTools_ReadMessage(SOCKET socket, char* message, int size)
{
	ASSERT(recv(socket, message, size, 0) >= SUCCESS, "Error in reading message.");
}






