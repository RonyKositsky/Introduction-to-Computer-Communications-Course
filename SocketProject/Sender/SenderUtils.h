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
	uint32_t message;
	uint32_t messageHamming;
	char msg_buffer[MSG_SIZE];
	bool quit;
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
Adding hamming code to the message we have read.
\return none
*****************************************************************************/
void SenderUtils_AddHammCode();

/*!
******************************************************************************
\brief
Reading from the input file the next 26 bits.
\return Number of bits read.
*****************************************************************************/
int SenderUtils_ReadBytesFromFile();

/*!
******************************************************************************
\brief
Tear down our sender.
\return none
*****************************************************************************/
void SenderUtils_SenderTearDown();

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

#endif //__SENDER_UTILS_H__