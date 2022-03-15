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
static void SenderUtils_GetMessageSize();
static void SenderUtils_AddHammCode();

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
	/*SenderArgs_s.ip = argv[1];
	SenderArgs_s.port = atoi(argv[2]);*/

	SenderArgs_s.ip = "127.0.0.1";
	SenderArgs_s.port = 6342;

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
	fclose(SenderParams_s.file);
}

void SenderUtils_OpenFile()
{
	//TODO: Handle errors, quit.
	printf("File name:");
	scanf("%s", SenderParams_s.filename);
	if (strcmp(SenderParams_s.filename, "quit")) // returns 1 if they are not equal.
	{
		// TODO: Handle errors.
		SenderParams_s.file = fopen(SenderParams_s.filename, "rb");
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
	SenderUtils_OpenFile();
	if (SenderParams_s.quit) return;
	SenderParams_s.socket = SocketTools_CreateSocket(SenderArgs_s.ip, SenderArgs_s.port, CLIENT);
}


/*!
******************************************************************************
\brief
Printing statistics and relevant data.
\return none
*****************************************************************************/
void SenderUtils_PrintOutput()
{
	printf("file length : %d bytes\n", SenderParams_s.message_size / 31 * 26);
	printf("bytes sent  : %d bytes\n", SenderParams_s.message_size);
	 
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


static void SenderUtils_GetMessageSize()
{
	size_t err, message_chunks = 0;
	// As said, we can assume that we will get blocks of MSG_SIZE.
	while (err = fread(SenderParams_s.msg_buffer, 1, MSG_SIZE, SenderParams_s.file))
	{
		if (err < 0)
		{
			fprintf(stderr, "Read from file failed.\n");
			exit(-1);
		}

		message_chunks++;
	}

	rewind(SenderParams_s.file);
	SenderParams_s.message_size = (int)message_chunks * HAMM_MSG_SIZE;
	SenderParams_s.sent_message = (char*)malloc(SenderParams_s.message_size * sizeof(char));
}

static void SenderUtils_AddHammCode()
{
	size_t err, index = 0;
	uint32_t msg, messageHamming;
	while (err = fread(SenderParams_s.msg_buffer, 1, MSG_SIZE, SenderParams_s.file))
	{
		if (err < 0)
		{
			fprintf(stderr, "Read from file failed.\n");
			exit(-1);
		}

		msg   = BitTools_ConvertStringToUint(SenderParams_s.msg_buffer, true);
		messageHamming = BitTools_GetMassageWithHamming(msg);

		for (int i = 0; i < HAMM_MSG_SIZE; i++)
		{
			SenderParams_s.sent_message[index] = BitTools_GetNBit(messageHamming, i) == 0 ? '0' : '1';
			index ++;
		}
	}	
}