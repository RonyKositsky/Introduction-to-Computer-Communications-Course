/*!
******************************************************************************
\file ChannelUtils.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/************************************
*      include                      *
************************************/
#include <winsock2.h>
#include "ChannelUtils.h"
#include "../Utilities/Definitions.h"
#include "../Utilities/BitTools.h"
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

static uint32_t Channelutils_AddDeterministicNoise();
static uint32_t Channelutils_AddRandomNoise();

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

    //for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++) 
    //{
    //    switch (argv[optind][-1])
    //    {
    //        case '-d':  ChParams_s.noise_type = DETERMINISTIC; break;
    //        case '-r':  ChParams_s.noise_type = RANDOM; break;
    //    }
    //}

    //if (ChParams_s.noise_type == DETERMINISTIC)
    //{
    //    ChArgs_s.cycle_length = atoi(argv[4]);
    //}

    //else if (ChParams_s.noise_type == RANDOM) 
    //{
    //    ChArgs_s.prob = atoi(argv[4]);
    //    ChArgs_s.seed = atoi(argv[5]);
    //    srand(ChArgs_s.seed);
    //    ChArgs_s.prob = ChArgs_s.prob / ERROR_PROB_CONSTANT;
    //}


    ChParams_s.noise_type == DETERMINISTIC;
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
    //memset(&ChParams_s, 0, sizeof(ChannelParams));
    //memset(&ChArgs_s, 0, sizeof(ChannelArguments));
    //memset(&ChOutput_s, 0, sizeof(ChannelArguments));

    ChannelUtils_ReadInput(argc, argv);
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
    
    // channel as server (recieves messages from the sender)
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
    int index = 0;
    uint32_t msg, noise, noisy_msg;
    char buf[HAMM_MSG_SIZE];
    while (index < ChParams_s.message_size)
    {
        // Get message as uint.
        for (size_t i = 0; i < HAMM_MSG_SIZE; i++)
        {
            buf[i] = ChParams_s.message[index];
            index++;
        }

        // Add noise.
        msg =  BitTools_ConvertStringToUint(buf, false);
        noise = ChParams_s.noise_type == RANDOM ? Channelutils_AddRandomNoise() : Channelutils_AddDeterministicNoise();
        noisy_msg = msg ^ noise;

        // Add to sent buffer.
        for (int i = 0; i < HAMM_MSG_SIZE; i++)
        {
            int sent_off = index - HAMM_MSG_SIZE;
            ChParams_s.message_sent[sent_off + i] = BitTools_GetNBit(noisy_msg, i);
        }
    }
}

/*!
******************************************************************************
\brief
Adding noise to recieved message.
\return none
*****************************************************************************/
void ChannelUtils_AskToContinue()
{
    char response[10];
    printf("continue? (yes/no)");
    if (!scanf("%s", response))
    {
        printf("Error in scanning answer.");
        exit(-1);
    }
    ChParams_s.quit = strcmp(response, "yes") == 0;
}

void ChannelUtils_PrintStatistics()
{
    // TODO: Implement
}


static uint32_t Channelutils_AddRandomNoise()
{
    uint32_t noise = 0;
    bool flip = false;
    for (int bit = 0; bit < HAMM_MSG_SIZE - 1; bit++)
    {
        flip = rand() % 100 < (ChArgs_s.prob * 100);
        if (!flip) continue;
        BIT_FLIP(noise, bit);
        ChOutput_s.flipped_bits++;
    };

    return noise;
}

static int Channelutils_AddDeterministicNoise()
{
    static int counter = 0;
    uint32_t noise = 0;
    for (int bit = 0; bit < HAMM_MSG_SIZE - 1; bit++) 
    {
        if (counter % ChArgs_s.cycle_length == 0) 
        {
            BIT_FLIP(noise, bit);
            ChOutput_s.flipped_bits++;
        };

        counter++;
    };

    return noise;
};