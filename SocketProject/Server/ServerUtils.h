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
#include "../Utilities/Definitions.h"

/************************************
*       types                       *
************************************/
typedef struct
{
	SOCKET socket;
	FILE* file;
	uint32_t message_size;
	uint32_t messageHamming;
	char filename[FILE_NAME_BUFFER];
	bool quit;
	char* message;
	int bytes_fixed;
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
 Initializing new server session.
\return none
*****************************************************************************/
void ServerUtils_SessionInit();

/*!
******************************************************************************
\brief
Printing relevant data and statistics.
\return none
*****************************************************************************/
void ServerUtils_PrintOutput();

/*!
******************************************************************************
\brief
Printing relevant data and statistics.
\return none
*****************************************************************************/
void ServerUtils_WriteToFile();

#endif //__SERVER_UTILSw_H__