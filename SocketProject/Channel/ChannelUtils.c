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
*      definitions                  *
************************************/
#define ERROR_PROB_CONSTANT (2^16)
#define IP_LISTEN_ALL "0.0.0.0"

/************************************
*       types                       *
************************************/
typedef struct
{
    int prob;
    int seed;
    int cycle_length;
}ChannelArguments;

/************************************
*      variables                 *
************************************/
static ChannelArguments ChArgs_s;
ChannelParams ChParams_s;

/************************************
*      static functions             *
************************************/
static uint32_t Channelutils_AddDeterministicNoise();
static uint32_t Channelutils_AddRandomNoise();

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

    ASSERT((argc <= 4) && (argc > 1), "Error in channel arguments");

    if (argc == 4)
    {
        ChParams_s.noise_type = RANDOM;
        ChArgs_s.prob = atoi(argv[2]);
        ChArgs_s.seed = atoi(argv[3]);
        srand(ChArgs_s.seed);
        ChArgs_s.prob = ChArgs_s.prob / ERROR_PROB_CONSTANT;
    }    
    else
    {
        ChArgs_s.cycle_length = atoi(argv[2]);
        ChParams_s.noise_type = DETERMINISTIC;
    }
        
    ChParams_s.ip = LOCAL_HOST_IP; 
    ChParams_s.sender_port = 0;
    ChParams_s.server_port = 0;


    ChParams_s.sender_sock = SocketTools_CreateSocket(ChParams_s.ip, ChParams_s.sender_port, SERVER,  SENDER);
    ChParams_s.server_sock = SocketTools_CreateSocket(ChParams_s.ip, ChParams_s.server_port, SERVER,  RECIEVER);
}

/*!
******************************************************************************
\brief
Initializing new session. Channel behave like 2 way server.
\return none
*****************************************************************************/
void ChannelUtils_InitSession()
{
    ChParams_s.message_size = 0;
    ChParams_s.flipped_bits = 0;

    ChParams_s.server_accepted_sock = accept(ChParams_s.server_sock, NULL, NULL);
    ChParams_s.sender_accepted_sock = accept(ChParams_s.sender_sock, NULL, NULL);
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
        msg =  BitTools_ConvertStringToUint(buf);
        noise = ChParams_s.noise_type == RANDOM ? Channelutils_AddRandomNoise() : Channelutils_AddDeterministicNoise();
        noisy_msg = msg ^ noise;

        // Add to sent buffer.
        for (int i = 0; i < HAMM_MSG_SIZE; i++)
        {
            int sent_off = index - HAMM_MSG_SIZE;
            ChParams_s.message_sent[sent_off + i] = BitTools_GetNBit(noisy_msg, i) == 0 ? '0' : '1';
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
    char response[10]; // Big enough buffer.
    printf("continue? (yes/no)\n");
    ASSERT(scanf("%s", response) >= 0, "Error in scanning answer.");
    ChParams_s.quit = strncmp(response, "yes", 10);
}

void ChannelUtils_PrintStatistics()
{
    printf("retransmitted %d bytes, flipped %d bits.\n", ChParams_s.message_size, ChParams_s.flipped_bits);
}

/*!
******************************************************************************
\brief
Adding noise to random bits based on user input.
\return none
*****************************************************************************/
static uint32_t Channelutils_AddRandomNoise()
{
    uint32_t noise = 0;
    bool flip = false;
    for (int bit = 0; bit < HAMM_MSG_SIZE - 1; bit++)
    {
        flip = rand() % 100 < (ChArgs_s.prob * 100);
        if (!flip) continue;
        BIT_FLIP(noise, bit);
        ChParams_s.flipped_bits++;
    };

    return noise;
}

/*!
******************************************************************************
\brief
Adding noise every nth bit based on user input.
\return none
*****************************************************************************/
static int Channelutils_AddDeterministicNoise()
{
    static int counter = 0;
    uint32_t noise = 0;
    for (int bit = 0; bit < HAMM_MSG_SIZE; bit++) 
    {
        if (counter % ChArgs_s.cycle_length == 0) 
        {
            BIT_FLIP(noise, bit);
            ChParams_s.flipped_bits++;
        };

        counter++;
    };

    return noise;
};