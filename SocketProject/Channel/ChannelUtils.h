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
	int port;
	char* server_ip;
	int server_port;
	int prob;
	int seed;
}ChannelArguments;

typedef struct
{
	int msg_size_from_sender;
	SOCKET server_sock;
	SOCKET sender_sock;
	struct sockaddr_in my_addr;
	struct sockaddr_in *sender_addr;
	struct sockaddr_in *server_addr;
	MessageVars readMsg;
	MessageVars writeMsg;
}ChannelParams;

/************************************
*      variables                    *
************************************/
static ChannelParams ChParams_s;
static ChannelArguments ChArgs_s;
static char* CHANNEL_REC_BUF;

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
void ChannelUtils_PrepareReadMsg();

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