/*!
******************************************************************************
\file SenderUtils.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/
#define _WINSOCK_DEPRECATED_NO_WARNINGS

/************************************
*      include                      *
************************************/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SenderUtils.h"
#include "../Utilities/SocketTools.h"
#include "../Utilities/BitTools.h"
/************************************
*      definitions                 *
************************************/
#define MAX_LIMIT 1000
#define NUMBER_OF_WORDS 8

/************************************
*       types                       *
************************************/
typedef struct
{
	char* ip;
	int port;
}SenderArguments;

/************************************
*      variables                 *
************************************/
static SenderArguments SenderArgs_s;
SenderParams SenderParams_s;

/************************************
*      static functions             *
************************************/
void SenderUtils_GetMessageSize();
static void SenderUtils_AddHammCode();
static void SenderUtils_AddHammCode();
static void SenderUtils_AddMessageToBuffer(uint32_t message);

static int index;

/************************************
*       API implementation          *
************************************/

/*!
******************************************************************************
\brief
Initialize the sender.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/
void SenderUtils_SenderInit(char* argv[])
{
	memset(&SenderArgs_s, 0, sizeof(SenderArguments));
	memset(&SenderParams_s, 0, sizeof(SenderParams));

	// Reading user input.
	SenderArgs_s.ip = argv[1];
	SenderArgs_s.port = atoi(argv[2]);

	SenderUtils_InitSession();
}

/*!
******************************************************************************
\brief
Reading from the input file the next 26 bits.
\return Number of bits read.
*****************************************************************************/
void SenderUtils_ReadingFile()
{
	SenderUtils_GetMessageSize();
	SenderUtils_AddHammCode();
}

void SenderUtils_OpenFile()
{
	printf("File name:\n");
	ASSERT(scanf("%s", SenderParams_s.filename) == 1, "Error in scanning file");
	if (strcmp(SenderParams_s.filename, "quit")) // returns 1 if they are not equal.
	{
		fopen_s(&SenderParams_s.file, SenderParams_s.filename, "r");
		ASSERT(SenderParams_s.file != NULL, "Error in open file in sender.");
		return;
	}
	SenderParams_s.quit = true;
}

/*!
******************************************************************************
\brief
Initialize sender new session.
\return none
*****************************************************************************/
void SenderUtils_InitSession()
{
	SenderParams_s.message_size = 0;
	index = 0;
	SenderUtils_OpenFile();
	if (SenderParams_s.quit) return;
	SenderParams_s.socket = SocketTools_CreateSocket(SenderArgs_s.ip, SenderArgs_s.port, CLIENT, SENDER);
}


/*!
******************************************************************************
\brief
Printing statistics and relevant data.
\return none
*****************************************************************************/
void SenderUtils_PrintOutput()
{
	printf("file length : %d bytes\n", SenderParams_s.message_size / (8 * 31) * 26 );
	printf("bytes sent : %d bytes\n", SenderParams_s.message_size / 8);
	 
}


/************************************
* static implementation             *
************************************/

/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
uint32_t BitTools_GetMassageWithHamming(uint32_t msg)
{
	uint32_t masked = 0;
	uint32_t pairity = 0;
	uint32_t hammingMessage = msg;

	for (int i = 0; i < HAMM_PAIRITY_BITS; i++)
	{
		masked = msg & HammingMasks[i];
		pairity = BitTools_BitwiseXOR(masked);
		
		// Set to 1 if pairity is 1, otherwise the default is 0 so we don't need to change it.
		if (!pairity) continue;
		BIT_SET(hammingMessage, HammingPairingBitsIndexes[i]);
	}
	return hammingMessage;
}

/*!
******************************************************************************
\brief
Calculating the message size.
\return none.
*****************************************************************************/
void SenderUtils_GetMessageSize()
{
	size_t err, message_chunks = 0;
	// As said, we can assume that we will get blocks of MSG_SIZE.
	while (err = fread(SenderParams_s.msg_buffer, 1, MSG_SIZE, SenderParams_s.file))
	{
		ASSERT(err >= 0, fprintf(stderr, "Read from file failed.\n"));
		message_chunks++;
	}

	rewind(SenderParams_s.file);
	SenderParams_s.message_size = (int)message_chunks * HAMM_MSG_SIZE * 8;
	SenderParams_s.sent_message = (char*)malloc(SenderParams_s.message_size * sizeof(char));
}

/*!
******************************************************************************
\brief
Preparing the sent message to be sent.
\return none.
*****************************************************************************/
static void SenderUtils_AddHammCode()
{
	size_t err;
	uint32_t msg;

	// Reading 26 bytes, meaning 26*8 = 208 bits.
	while (err = fread(SenderParams_s.msg_buffer, 1, MSG_SIZE, SenderParams_s.file))
	{
		ASSERT(err >= 0, "Read from file failed.\n");
		int	curr_word = 0;
		int char_index = 0;

		for (int word = 0; word < NUMBER_OF_WORDS; word++)
		{
			uint32_t msg = 0;
			int hamming_index = 0;

			for (int i = 0; i < HAMM_MSG_SIZE; i++)
			{
				if (i == HammingPairingBitsIndexes[hamming_index])
				{
					hamming_index++;
					continue;
				}

				if (SenderParams_s.msg_buffer[curr_word] >> char_index & 0x1) // Getting the bit.
				{
					BIT_SET(msg, i);
				}

				if (++char_index == BYTE_LENGTH)
				{
					char_index = 0;
					curr_word++;
				}
			}

			SenderUtils_AddMessageToBuffer(msg);
		}
	}	
}

/*!
******************************************************************************
\brief
Adding hamming pairty bits and adding them to buffer.
\return none.
*****************************************************************************/
static void SenderUtils_AddMessageToBuffer(uint32_t message)
{
	uint32_t messageHamming = BitTools_GetMassageWithHamming(message);

	for (int i = 0; i < HAMM_MSG_SIZE; i++)
	{
		SenderParams_s.sent_message[index] = BitTools_GetNBit(messageHamming, i) == 0 ? '0' : '1';
		index++;
	}
}