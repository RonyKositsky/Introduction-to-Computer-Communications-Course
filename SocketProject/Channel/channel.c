/*!
******************************************************************************
\file Channel.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
ALL RIGHTS RESERVED
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "ChannelUtils.h"
#include "../Utilities/Definitions.h"
#include "../Utilities/SocketTools.h"


/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[])
{
    fd_set active_fs;
    ChannelUtils_ChannelInit(argv);

    FD_ZERO(&active_fs);
    while (1) 
    {
        FD_SET(ChParams_s.sender_sock, &active_fs);
        FD_SET(ChParams_s.server_sock, &active_fs);
        int ready = select(2, &active_fs, NULL, NULL, NULL);
        if (ready < 0) 
        {
            fprintf(stderr, "select failed in channel");
        }
        else if (FD_ISSET(ChParams_s.server_sock, &active_fs))
        { 
            break;
        }
        else 
        { 
            //sender is ready
            ChannelUtils_PrepareReadMsg();
            ChParams_s.msg_size_from_sender = SocketTools_ReadMessage(&ChParams_s.readMsg); // Read message from client

            // TODO: Add noise.
            //randomly_flip_msg_bits(CHANNEL_REC_BUF, seed, prob); 

            ChannelUtils_PrepareWriteMsg();
            SocketTools_SendMessage(&ChParams_s.writeMsg);

            //channel_output_params[1] += msg_size_from_sender;
        }
    }
    
    ChannelUtils_ChannelTearDown();
    return 0;
}