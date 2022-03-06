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

    fd_set active_fs;
    FD_ZERO(&active_fs);
    SOCKET s = accept(ChParams_s.sender_sock, NULL, NULL);
    int ready;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;


    while (1) 
    {
        FD_SET(s, &active_fs);
        ready = select(1, &active_fs, NULL, NULL, &tv);
        if (ready < 0) {
            fprintf(stderr, "select failed in channel");
        }
        else if (ready)
        {  
            SocketTools_ReadMessage(s, &ChParams_s.message);
            printf("%lu\n", ChParams_s.message);
        }
        
        // TODO: Add noise.
        //randomly_flip_msg_bits(CHANNEL_REC_BUF, seed, prob); 

        //SocketTools_SendMessage(ChParams_s.server_sock, ChParams_s.message);
    }
    
    ChannelUtils_ChannelTearDown();
    return 0;
}