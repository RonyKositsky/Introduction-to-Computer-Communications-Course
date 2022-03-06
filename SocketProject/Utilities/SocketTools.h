/*!
******************************************************************************
\file SocketTools.h
\date 24 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/
#ifndef __SOCKET_TOOLS_H__
#define __SOCKET_TOOLS_H__

/************************************
*      include                      *
************************************/
#include <stdio.h>
#include <winsock2.h>
#include "Definitions.h"

/************************************
*       types                       *
************************************/
typedef struct
{
	SOCKET sock;
	struct sockaddr_in *addr;
	char* buf;
	int buf_size;
}MessageVars;

/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Initializing new socket.
\return SOCKET.
*****************************************************************************/
SOCKET SocketTools_CreateSocket();

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
void SocketTools_CreateAddress(SOCKET* socket, char* ip,  int port, SocketType type);

/*!
******************************************************************************
\brief
Reading message via socket.
\param
 [in] msgVars - The message arguments struct.
\return the number of bits recieved.
*****************************************************************************/
int SocketTools_ReadMessage(MessageVars* msgVars);

/*!
******************************************************************************
\brief
Sending message via socket.
\param
 [in] msgVars - The message arguments struct.
\return the number of bits sent.
*****************************************************************************/
int SocketTools_SendMessage(MessageVars* msgVars);

#endif //__SOCKET_TOOLS_H__