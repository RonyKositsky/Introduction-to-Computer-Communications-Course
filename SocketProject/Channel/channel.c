/*!
******************************************************************************
\file Channel.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "ChannelUtils.h"
#include "../Utilities/Definitions.h"
#include "../Utilities/SocketTools.h"
#include <stdio.h>
#include <winsock2.h>

/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[])
{
    ChannelUtils_ChannelInit(argc, argv);
    uint32_t ack;
    bool firstIteration = true;

    while (!ChParams_s.quit)
    {
        ChannelUtils_InitSession(firstIteration);

        // Getting message size.
        SocketTools_ReadMessageSize(ChParams_s.sender_accepted_sock, &ChParams_s.message_size);
        SocketTools_SendMessageSize(ChParams_s.sender_accepted_sock, ACK);

        SocketTools_SendMessageSize(ChParams_s.server_accepted_sock, ChParams_s.message_size);
        SocketTools_ReadMessageSize(ChParams_s.server_accepted_sock, &ack);

        // Reading message.
        ChParams_s.message = (char*)malloc(ChParams_s.message_size * sizeof(char));
        ChParams_s.message_sent = (char*)malloc(ChParams_s.message_size * sizeof(char));
        SocketTools_ReadMessage(ChParams_s.sender_accepted_sock, ChParams_s.message, ChParams_s.message_size);

        ChannelUtils_AddNoiseToMessage();

        // Sending noisy message.
        SocketTools_SendMessage(ChParams_s.server_accepted_sock, ChParams_s.message_sent, ChParams_s.message_size);
        
        // Closing current procedure.
        closesocket(ChParams_s.sender_accepted_sock);
        closesocket(ChParams_s.server_accepted_sock);
        free(ChParams_s.message);

        ChannelUtils_PrintStatistics();
        ChannelUtils_AskToContinue();
        firstIteration = false;
    }
    
    return 0;
}