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
#include <stdint.h>
#include <winsock2.h>
#include "Definitions.h"
#include <stdbool.h>

/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Initializing new socket.
\return SOCKET.
*****************************************************************************/
SOCKET SocketTools_CreateSocket(char* ip, int port, SocketType sockType, bool printData, ClientType clientType);

/*!
******************************************************************************
\brief
Reading message size via socket.
\param
 [in] socket    - The socket we are reading from.
 [in] message_p - Pointer to the message we will save the message to.
\return none.
*****************************************************************************/
void SocketTools_ReadMessageSize(SOCKET socket, uint32_t *message_p);

/*!
******************************************************************************
\brief
Sending message via socket.
\param
 [in] socket - the sending socket.
 [in] message_size - the size.
\return none.
*****************************************************************************/
void SocketTools_SendMessageSize(SOCKET socket, uint32_t message_size);

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
void SocketTools_ReadMessage(SOCKET socket, char* message, int size);

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
void SocketTools_SendMessage(SOCKET socket, char* message_size, int size);

#endif //__SOCKET_TOOLS_H__