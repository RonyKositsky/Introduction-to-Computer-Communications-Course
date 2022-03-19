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
void SenderUtils_SenderInit(char* argv[]);
void  SenderUtils_ReadingFile();
void SenderUtils_OpenFile();
void SenderUtils_InitSession();
void SenderUtils_PrintOutput();

#endif //__SENDER_UTILS_H__