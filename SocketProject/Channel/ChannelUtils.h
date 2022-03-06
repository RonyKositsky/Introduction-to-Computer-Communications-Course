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
	uint32_t message;
	SOCKET server_sock;
	SOCKET sender_sock;
	char* sender_ip;
	char* server_ip;
	int sender_port;
	int server_port;
}ChannelParams;

/************************************
*      variables                    *
************************************/
extern ChannelParams ChParams_s;

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
void ChannelUtils_ReadMsgFromSender();

/*!
******************************************************************************
\brief
Preparing channel to writing massage.
\return none
*****************************************************************************/
void ChannelUtils_SendMsgToServer();

/*!
******************************************************************************
\brief
Tearing down the channel.
\return none
*****************************************************************************/
void ChannelUtils_ChannelTearDown();

void SenderUtils_OpenFile();

#endif //__CHANNEL_UTILS_H__