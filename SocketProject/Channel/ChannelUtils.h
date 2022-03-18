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
	uint32_t message_size;
	char* message;
	char* message_sent;
	SOCKET server_sock;
	SOCKET sender_sock;
	SOCKET sender_accepted_sock;
	SOCKET server_accepted_sock;
	Noise noise_type;
	char* ip;
	int sender_port;
	int server_port;
	bool quit;
	int flipped_bits;
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
void ChannelUtils_InitSession(bool firstIteration);

/*!
******************************************************************************
\brief
Adding noise to recieved message.
\return none
*****************************************************************************/
void ChannelUtils_AddNoiseToMessage();

/*!
******************************************************************************
\brief
Adding noise to recieved message.
\return none
*****************************************************************************/
void ChannelUtils_AskToContinue();

/*!
******************************************************************************
\brief
Adding noise to recieved message.
\return none
*****************************************************************************/
void ChannelUtils_PrintStatistics();

#endif //__CHANNEL_UTILS_H__