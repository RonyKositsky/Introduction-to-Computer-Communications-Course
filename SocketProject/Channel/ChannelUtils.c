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

/************************************
*      defines                      *
************************************/
#define ERROR_PROB_CONSTANT (2^16)

typedef struct
{
    int prob;
    int seed;
    int cycle_length;
}ChannelArguments;

typedef struct
{
    int flipped_bits;
}ChannelOutput;

static ChannelOutput ChOutput_s;
static ChannelArguments ChArgs_s;
ChannelParams ChParams_s;

static int Channelutils_AddDeterministicNoise();
static int Channelutils_AddRandomNoise();

/*!
******************************************************************************
\brief
Reading user input to our variables.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/
void ChannelUtils_ReadInput(int argc, char* argv[])
{
    int optind;

    for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++) 
    {
        switch (argv[optind][-1])
        {
            case '-d':  ChParams_s.noise_type = DETERMINISTIC; break;
            case '-r':  ChParams_s.noise_type = RANDOM; break;
        }
    }

    if (ChParams_s.noise_type == DETERMINISTIC)
    {
        ChArgs_s.cycle_length = atoi(argv[4]);
    }

    else if (ChParams_s.noise_type == RANDOM) 
    {
        ChArgs_s.prob = atoi(argv[4]);
        ChArgs_s.seed = atoi(argv[5]);
        srand(ChArgs_s.seed);
        ChArgs_s.prob = ChArgs_s.prob / ERROR_PROB_CONSTANT;
    }

    ChParams_s.sender_ip = "127.0.0.1";
    ChParams_s.sender_port = 6342;

    ChannelUtils_InitSession();
}

/*!
******************************************************************************
\brief
Initialize the channel.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/

void ChannelUtils_ChannelInit(int argc, char* argv[])
{
    memset(&ChParams_s, 0, sizeof(ChannelParams));
    memset(&ChArgs_s, 0, sizeof(ChannelArguments));
    memset(&ChOutput_s, 0, sizeof(ChannelArguments));

    ChannelUtils_ReadInput(argc, argv);
}

/*!
******************************************************************************
\brief
Tearing down the channel.
\return none
*****************************************************************************/
void ChannelUtils_ChannelTearDown()
{
    closesocket(ChParams_s.server_sock);
    closesocket(ChParams_s.sender_sock);

    //TODO: Implement printing below.
     
    //char* sender_ip_str = inet_ntoa((ChParams.sender_addr)->sin_addr);
    //print_channel_output(ChArgs.server_ip, sender_ip_str);
}

/*!
******************************************************************************
\brief
Initializing new session.
\return none
*****************************************************************************/
void ChannelUtils_InitSession()
{
    // TODO: handle errors.
    // 
    //channel as server (recieves messages from the sender)
    ChParams_s.sender_sock = SocketTools_CreateSocket(ChParams_s.sender_ip, ChParams_s.sender_port, SERVER);
    ChParams_s.accepted_sock = accept(ChParams_s.sender_sock, NULL, NULL);

    // channel as sender (sends messages to the server)
    //ChParams_s.server_sock = SocketTools_CreateSocket();
}

/*!
******************************************************************************
\brief
Adding noise to recieved message.
\return none
*****************************************************************************/
void ChannelUtils_AddNoiseToMessage()
{
    uint32_t noise = ChParams_s.noise_type == RANDOM ? Channelutils_AddRandomNoise() : Channelutils_AddDeterministicNoise();
    ChParams_s.message ^= noise;
}

static int Channelutils_AddRandomNoise()
{
    uint32_t noise = 0;
    bool flip = false;
    for (int bit = 0; bit < HAMM_MSG_SIZE - 1; bit++)
    {
        flip = rand() % 100 < (ChArgs_s.prob * 100);
        if (!flip) continue;
        BIT_FLIP(BIT_FLIP, bit);
        ChOutput_s.flipped_bits++;
    };
}

static int Channelutils_AddDeterministicNoise()
{
    static int counter = 0;
    uint32_t noise = 0;
    for (int bit = 0; bit < HAMM_MSG_SIZE - 1; bit++) 
    {
        if (counter % ChArgs_s.cycle_length == 0) 
        {
            BIT_FLIP(BIT_FLIP, bit);
            ChOutput_s.flipped_bits++;

        };

        counter++;
    };

    return noise;
};