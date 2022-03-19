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

SOCKET SocketTools_CreateSocket(char* ip, int port, SocketType sockType, bool printData, ClientType clientType);
void SocketTools_ReadMessageSize(SOCKET socket, uint32_t *message_p);
void SocketTools_SendMessageSize(SOCKET socket, uint32_t message_size);
void SocketTools_ReadMessage(SOCKET socket, char* message, int size);
void SocketTools_SendMessage(SOCKET socket, char* message_size, int size);

#endif //__SOCKET_TOOLS_H__