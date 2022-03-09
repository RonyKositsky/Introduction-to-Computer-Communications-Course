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
#include <stdbool.h>
#include <winsock2.h>
#include "../Utilities/SocketTools.h"

/************************************
*       types                       *
************************************/
typedef enum
{
	RANDOM,
	DETERMINISTIC
}Noise;

typedef struct
{
	uint32_t message;
	SOCKET server_sock;
	SOCKET sender_sock;
	SOCKET accepted_sock;
	Noise noise_type;
	char* sender_ip;
	char* server_ip;
	int sender_port;
	int server_port;
	bool quit;
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
void ChannelUtils_ChannelInit(int argc, char* argv[]);


/*!
******************************************************************************
\brief
Initializing new session.
\return none
*****************************************************************************/
void ChannelUtils_InitSession();

/*!
******************************************************************************
\brief
Tearing down the channel.
\return none
*****************************************************************************/
void ChannelUtils_ChannelTearDown();

/*!
******************************************************************************
\brief
Adding noise to recieved message.
\return none
*****************************************************************************/
void ChannelUtils_AddNoiseToMessage();

#endif //__CHANNEL_UTILS_H__