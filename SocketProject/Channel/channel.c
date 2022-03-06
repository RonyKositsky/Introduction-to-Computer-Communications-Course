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
    ChannelUtils_ChannelInit(argv);
    printf("YO\n");
    SOCKET s = accept(ChParams_s.sender_sock, NULL, NULL);

    fd_set active_fs;
    FD_ZERO(&active_fs);
    int ready;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;


    while (1) 
    {
        FD_ZERO(&active_fs);
        FD_SET(s, &active_fs);
        ready = select(3, &active_fs, NULL, NULL, NULL);
        if (ready != 0) 
        {
            if (FD_ISSET(s, &active_fs))
            {
                SocketTools_ReadMessage(s, &ChParams_s.message);
                printf("%lu\n", ChParams_s.message);
                if (ChParams_s.message == TERMINATION_MESSAGE)
                {
                    printf("YO\n");
                    closesocket(s);
                    break;
                }
            }
        }
        

        printf("%lu", ChParams_s.message);
        
        // TODO: Add noise.
        //randomly_flip_msg_bits(CHANNEL_REC_BUF, seed, prob); 

        //SocketTools_SendMessage(ChParams_s.server_sock, ChParams_s.message);
    }
    
    ChannelUtils_ChannelTearDown();
    return 0;
}