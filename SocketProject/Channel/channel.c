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


/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[])
{
    ChannelUtils_ChannelInit(argv);

    while (1) 
    {
        
        //sender is ready
        ChannelUtils_ReadMsg();
        //ChParams_s.msg_size_from_sender = SocketTools_ReadMessage(&ChParams_s.readMsg); // Read message from client

        // TODO: Add noise.
        //randomly_flip_msg_bits(CHANNEL_REC_BUF, seed, prob); 

        ChannelUtils_PrepareWriteMsg();
        SocketTools_SendMessage(&ChParams_s.writeMsg);

        //channel_output_params[1] += msg_size_from_sender;
    }
    
    ChannelUtils_ChannelTearDown();
    return 0;
}