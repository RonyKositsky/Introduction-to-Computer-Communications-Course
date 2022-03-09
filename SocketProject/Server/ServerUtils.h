/*!
******************************************************************************
\file ServerUtils.h
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

#ifndef __SERVER_UTILS_H__
#define __SERVER_UTILS_H__

/************************************
*      include                      *
************************************/
#include <WinSock2.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/************************************
*      definitions                 *
************************************/

/************************************
*       types                       *
************************************/
typedef struct
{
	SOCKET socket;
	SOCKET accepted_socket;
	FILE* file;
	uint32_t message;
	uint32_t messageHamming;
	bool quit;
}ServerParams;

extern ServerParams ServerParams_s;

/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Initialize the server.
\param
 [in] argv - User input.
\return none
*****************************************************************************/
void ServerUtils_ServerInit(char* argv[]);

/*!
******************************************************************************
\brief
Teardown the server.
\return none
*****************************************************************************/
void ServerUtils_ServerTearDown();

/*!
******************************************************************************
\brief
 Handeling new received message.
\return none
*****************************************************************************/
void ServerUtils_HandleMessage(int bytesRecived);

/*!
******************************************************************************
\brief
 Initializing new server session.
\return none
*****************************************************************************/
void ServerUtils_SessionInit();

#endif //__SERVER_UTILSw_H__