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

    ChParams_s.sender_ip = "127.0.0.1";
    ChParams_s.sender_port = 6343;
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
    ChParams_s.sender_sock = SocketTools_CreateSocket();
    SocketTools_CreateAddress(&ChParams_s.sender_sock, ChParams_s.sender_ip, ChArgs_s.server_port, SERVER);

    // channel as sender (sends messages to the server)
    ChParams_s.server_sock = SocketTools_CreateSocket();
    SocketTools_CreateAddress(&ChParams_s.server_sock, ChParams_s.server_ip, ChArgs_s.server_port, CLIENT);
}

/*!
******************************************************************************
\brief
Preparing channel read massage that was sent.
\return none
*****************************************************************************/
void ChannelUtils_ReadMsg()
{
    SOCKET s = accept(&ChParams_s.sender_sock, NULL, NULL);
    ChParams_s.msg_size_from_sender = recv(s, ChParams_s.channel_recieve_buffer, MAX_BUFFER, 0);
    /*ChParams_s.readMsg.sock = ChParams_s.server_sock;
    ChParams_s.readMsg.addr = ChParams_s.server_addr;
    ChParams_s.readMsg.buf = CHANNEL_REC_BUF;
    ChParams_s.readMsg.buf_size = MAX_BUFFER;*/
}

/*!
******************************************************************************
\brief
Preparing channel to writing massage.
\return none
*****************************************************************************/
void ChannelUtils_PrepareWriteMsg()
{
    ChParams_s.readMsg.sock = ChParams_s.sender_sock;
    ChParams_s.readMsg.addr = ChParams_s.sender_addr;
    ChParams_s.readMsg.buf = CHANNEL_REC_BUF;
    ChParams_s.readMsg.buf_size = ChParams_s.msg_size_from_sender;
}

/*!
******************************************************************************
\brief
Tearing down the channel.
\return none
*****************************************************************************/
void ChannelUtils_ChannelTearDown()
{
    ChannelUtils_ReadMsg();
    ChannelUtils_PrepareWriteMsg();
    SocketTools_ReadMessage(&ChParams_s.readMsg); //read the message from the server 
    SocketTools_SendMessage(&ChParams_s.writeMsg);
    closesocket(ChParams_s.server_sock);
    closesocket(ChParams_s.sender_sock);

    //TODO: Implement printing below.
     
    //char* sender_ip_str = inet_ntoa((ChParams.sender_addr)->sin_addr);
    //print_channel_output(ChArgs.server_ip, sender_ip_str);
}