/*!
******************************************************************************
\file SenderUtils.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

#ifndef __SENDER_UTILS_H__
#define __SENDER_UTILS_H__

#include "../Utilities/Definitions.h"
#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/************************************
*       types                       *
************************************/
typedef struct
{
	SOCKET socket;
	FILE* file;
	uint32_t ack;
	char msg_buffer[MSG_SIZE];
	char filename[FILE_NAME_BUFFER];
	bool quit;
	int message_size;
	char* sent_message;
}SenderParams;

extern SenderParams SenderParams_s;

/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Initialize the sender.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/
void SenderUtils_SenderInit(char* argv[]);

/*!
******************************************************************************
\brief
Reading from the input file the next 26 bits.
\return Number of bits read.
*****************************************************************************/
void  SenderUtils_ReadingFile();

/*!
******************************************************************************
\brief
Getting file name from the user.
\return none
*****************************************************************************/
void SenderUtils_OpenFile();

/*!
******************************************************************************
\brief
Initialize sender new session.
\return none
*****************************************************************************/
void SenderUtils_InitSession();

/*!
******************************************************************************
\brief
Printing statistics and relevant data.
\return none
*****************************************************************************/
void SenderUtils_PrintOutput();

#endif //__SENDER_UTILS_H__