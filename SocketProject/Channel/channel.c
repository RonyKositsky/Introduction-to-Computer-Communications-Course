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

    while (!ChParams_s.quit)
    {
        // Getting message size.
        SocketTools_ReadMessageSize(ChParams_s.accepted_sock, &ChParams_s.message_size);
        SocketTools_SendMessageSize(ChParams_s.server_sock, ChParams_s.message_size);

        // Reading message.
        ChParams_s.message = (char*)malloc(ChParams_s.message_size * sizeof(char));
        SocketTools_ReadMessage(ChParams_s.accepted_sock, ChParams_s.message, ChParams_s.message_size);
        ChannelUtils_AddNoiseToMessage();

        // Sending noisy message.
        //SocketTools_SendMessage(ChParams_s.server_sock, ChParams_s.message, ChParams_s.message_size);
        
        // Closing current procedure.
        SocketTools_ReadMessageSize(ChParams_s.accepted_sock, &ChParams_s.message_size);
        closesocket(ChParams_s.accepted_sock);
        free(ChParams_s.message);

        // Print
        ChannelUtils_AskToContinue();
    }
    
    return 0;
}