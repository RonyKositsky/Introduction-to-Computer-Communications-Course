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
    //fd_set active_fs;
    //ChannelUtils_ChannelInit(argv);
    char recieved_buffer[1200];
    WSADATA wsaData; 
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) 
        printf("Error at WSAStartup()\n");

    struct sockaddr_in my_addr; 
    struct sockaddr_in peer_addr;

    SOCKET tcp_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    my_addr.sin_port = htons(6342);

    int status = bind(tcp_s, (SOCKADDR*)&my_addr, sizeof(struct sockaddr));
    status = listen(tcp_s, SOMAXCONN);
    int len = sizeof(peer_addr);

    while (1)
    {
        printf("hi\n");
        printf("hi\n");
        SOCKET s = accept(tcp_s, NULL, NULL);
        printf("hi2\n");
        int received = recv(s, recieved_buffer, MSG_SIZE, 0); 
        if (received) printf("%s", recieved_buffer);
    }

    //FD_ZERO(&active_fs);
    //while (1) 
    //{
    //    FD_SET(ChParams_s.sender_sock, &active_fs);
    //    FD_SET(ChParams_s.server_sock, &active_fs);
    //    int ready = select(2, &active_fs, NULL, NULL, NULL);
    //    if (ready < 0) 
    //    {
    //        fprintf(stderr, "select failed in channel");
    //    }
    //    else if (FD_ISSET(ChParams_s.server_sock, &active_fs))
    //    { 
    //        break;
    //    }
    //    else 
    //    { 
    //        //sender is ready
    //        ChannelUtils_PrepareReadMsg();
            ChParams_s.msg_size_from_sender = SocketTools_ReadMessage(&ChParams_s.readMsg); // Read message from client

    //        // TODO: Add noise.
    //        //randomly_flip_msg_bits(CHANNEL_REC_BUF, seed, prob); 

    //        ChannelUtils_PrepareWriteMsg();
    //        SocketTools_SendMessage(&ChParams_s.writeMsg);

    //        //channel_output_params[1] += msg_size_from_sender;
    //    }
    //}
    
    ChannelUtils_ChannelTearDown();
    return 0;
}