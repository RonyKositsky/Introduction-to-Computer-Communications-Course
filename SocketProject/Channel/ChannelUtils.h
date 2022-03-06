/*!
******************************************************************************
\file ChannelUtils.h
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

#ifndef __CHANNEL_UTILS_H__
#define __CHANNEL_UTILS_H__

/************************************
*      include                      *
************************************/
#include <winsock2.h>
#include "../Utilities/SocketTools.h"

/************************************
*       types                       *
************************************/
typedef struct
{
	int prob;
	int seed;
}ChannelArguments;

typedef struct
{
	int msg_size_from_sender;
	SOCKET server_sock;
	SOCKET sender_sock;
	char* sender_ip;
	char* server_ip;
	int sender_port;
	int server_port;
	MessageVars readMsg;
	MessageVars writeMsg;
	char channel_recieve_buffer[MAX_BUFFER];
}ChannelParams;

/************************************
*      variables                    *
************************************/
static ChannelParams ChParams_s;
static ChannelArguments ChArgs_s;

/************************************
*       API                         *
************************************/

/*!
******************************************************************************
\brief
Initialize the channel.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/
void ChannelUtils_ChannelInit(char* argv[]);

/*!
******************************************************************************
\brief
Preparing channel read massage that was sent.
\return none
*****************************************************************************/
void ChannelUtils_ReadMsg();

/*!
******************************************************************************
\brief
Preparing channel to writing massage.
\return none
*****************************************************************************/
void ChannelUtils_PrepareWriteMsg();

/*!
******************************************************************************
\brief
Tearing down the channel.
\return none
*****************************************************************************/
void ChannelUtils_ChannelTearDown();

void SenderUtils_GetFileName();

#endif //__CHANNEL_UTILS_H__