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

/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Initializing new socket.
\return SOCKET.
*****************************************************************************/
SOCKET SocketTools_CreateSocket(char* ip, int port, SocketType type);

/*!
******************************************************************************
\brief
Reading message via socket.
\param
 [in] socket    - The socket we are reading from.
 [in] message_p - Pointer to the message we will save the message to.
\return the number of bits recieved.
*****************************************************************************/
int SocketTools_ReadMessage(SOCKET socket, uint32_t *message_p);

/*!
******************************************************************************
\brief
Sending message via socket.
\param
 [in] msgVars - The message arguments struct.
\return the number of bits sent.
*****************************************************************************/
int SocketTools_SendMessage(SOCKET socket, uint32_t message);

/*!
******************************************************************************
\brief
Terminating the program.
\param
 [in] socket - The socket we will send the data.
\return the number of bits sent.
*****************************************************************************/
int SocketTools_SendQuit(SOCKET socket);

/*!
******************************************************************************
\brief
Sending continue session for another file.
\return the number of bits sent.
*****************************************************************************/
int SocketTools_SendContinue(SOCKET socket);

#endif //__SOCKET_TOOLS_H__