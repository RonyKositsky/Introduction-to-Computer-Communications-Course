/*!
******************************************************************************
\file ChannelUtils.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include <winsock2.h>
#include "ChannelUtils.h"
#include "../Utilities/Definitions.h"
#include <stdint.h>

typedef struct
{
    int prob;
    int seed;
}ChannelArguments;

static ChannelArguments ChArgs_s;
ChannelParams ChParams_s;

/*!
******************************************************************************
\brief
Reading user input to our variables.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/
void ChannelUtils_ReadInput(char* argv[])
{
   /* 
    ;
    ChArgs_s.prob = atoi(argv[4]);
    ChArgs_s.seed = atoi(argv[5]);*/

    ChParams_s.sender_ip = "127.0.0.1";
    ChParams_s.sender_port = 6342;

    //ChParams_s.sender_ip = "127.0.0.1";
    //ChParams_s.sender_port = 6343;
}

/*!
******************************************************************************
\brief
Initialize the channel.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/

void ChannelUtils_ChannelInit(char* argv[])
{
    memset(&ChParams_s, 0, sizeof(ChannelParams));
    memset(&ChArgs_s, 0, sizeof(ChannelArguments));

    ChannelUtils_ReadInput(argv);

    //channel as server (recieves messages from the sender)
    ChParams_s.sender_sock = SocketTools_CreateSocket(ChParams_s.sender_ip, ChParams_s.sender_port, SERVER);

    // channel as sender (sends messages to the server)
    //ChParams_s.server_sock = SocketTools_CreateSocket();
    //SocketTools_CreateAddress(&ChParams_s.server_sock, ChParams_s.server_ip, ChParams_s.server_port, CLIENT);
}

/*!
******************************************************************************
\brief
Preparing channel read massage that was sent.
\return none
*****************************************************************************/
void ChannelUtils_ReadMsgFromSender()
{
    uint32_t rm;
    SOCKET s = accept(ChParams_s.sender_sock, NULL, NULL);
    int status = recv(s, &rm, sizeof(uint32_t), 0);
    ChParams_s.message = ntohl(rm);
}

/*!
******************************************************************************
\brief
Preparing channel to writing massage.
\return none
*****************************************************************************/
void ChannelUtils_SendMsgToServer()
{
   
}

/*!
******************************************************************************
\brief
Tearing down the channel.
\return none
*****************************************************************************/
void ChannelUtils_ChannelTearDown()
{
    ChannelUtils_ReadMsgFromSender();
    closesocket(ChParams_s.server_sock);
    closesocket(ChParams_s.sender_sock);

    //TODO: Implement printing below.
     
    //char* sender_ip_str = inet_ntoa((ChParams.sender_addr)->sin_addr);
    //print_channel_output(ChArgs.server_ip, sender_ip_str);
}