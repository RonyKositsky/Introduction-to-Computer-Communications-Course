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
        while (ChParams_s.message == TERMINATION_MESSAGE)
        {
            SocketTools_ReadMessage(ChParams_s.accepted_sock, &ChParams_s.message);
            ChannelUtils_AddNoiseToMessage();
            //SocketTools_SendMessage(ChParams_s.server_sock, ChParams_s.message);
        }
        
        SocketTools_ReadMessage(ChParams_s.accepted_sock, &ChParams_s.message);
        closesocket(ChParams_s.accepted_sock);

        if (ChParams_s.message == QUIT)
        {
            SocketTools_SendQuit(ChParams_s.server_sock);
            ChParams_s.quit = true;
        }      
    }
    
    ChannelUtils_ChannelTearDown();
    return 0;
}