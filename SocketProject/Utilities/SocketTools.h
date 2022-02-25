/*!
******************************************************************************
\file SocketTools.h
\date 24 February 2022
\author Jonathan Matetzky & Rony Kosistky
ALL RIGHTS RESERVED
*****************************************************************************/

#ifndef __SOCKET_TOOLS_H__
#define __SOCKET_TOOLS_H__

/************************************
*      include                      *
************************************/
#include <stdio.h>
#include <winsock2.h>

/************************************
*       types                       *
************************************/
typedef struct
{
	SOCKET sock;
	char* buf;
	int buf_size;
	struct sockaddr_in* addr;
}MessgeVars;

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
void SocketTools_CreateAddress(struct sockaddr_in* sa, int port, char* ip);

/*!
******************************************************************************
\brief
Reading message via socket.
\param
 [in] msgVars - The message arguments struct.
\return the number of bits recieved.
*****************************************************************************/
int SocketTools_ReadMessage(MessgeVars* msgVars);

/*!
******************************************************************************
\brief
Sending message via socket.
\param
 [in] msgVars - The message arguments struct.
\return the number of bits sent.
*****************************************************************************/
int SocketTools_SendMessage(MessgeVars* msgVars);

#endif //__SOCKET_TOOLS_H__