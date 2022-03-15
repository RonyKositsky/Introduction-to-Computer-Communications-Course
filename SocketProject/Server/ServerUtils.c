/*!
******************************************************************************
\file ServerUtils.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE

/************************************
*      include                      *
************************************/
#include "ServerUtils.h"
#include <math.h>
#include "../Utilities/Definitions.h"
#include "../Utilities/SocketTools.h"
#include "../Utilities/BitTools.h"

/************************************
*       types                       *
************************************/
typedef struct
{
	int port;
	char* ip;
}ServerArguments;


/************************************
*      variables                    *
************************************/
static ServerArguments ServerArgs_s;
ServerParams ServerParams_s;

/************************************
*      static functions             *
************************************/
static void SenderUtils_OpenFile();
static uint32_t ServerUtils_StripHammingCode(uint32_t message_size);

/************************************
*       API implementation          *
************************************/

/*!
******************************************************************************
\brief
Initialize the server.
\param
 [in] argv - User input.
\return none
*****************************************************************************/
void ServerUtils_ServerInit(char* argv[])
{
	memset(&ServerArgs_s, 0, sizeof(ServerArguments));
	memset(&ServerParams_s, 0, sizeof(ServerParams));

	// Rading values from user.
	ServerArgs_s.ip = argv[1];
	ServerArgs_s.port = atoi(argv[2]);

	ServerUtils_SessionInit();
}

/*!
******************************************************************************
\brief
Printing relevant data and statistics.
\return none
*****************************************************************************/
void ServerUtils_WriteToFile()
{
	int index = 0;
	char bit;
	uint32_t msg, recovered_message;
	char buf[HAMM_MSG_SIZE] = {0};
	while (index < (int)ServerParams_s.message_size)
	{
		// Get message as uint.
		for (int i = 0; i < HAMM_MSG_SIZE; i++)
		{
			buf[i] = ServerParams_s.message[index];
			index++;
		}

		// remove noise.
		msg = BitTools_ConvertStringToUint(buf, false);
		recovered_message = ServerUtils_StripHammingCode(msg);

		// Add to sent buffer.
		int hamming_index = 0;
		int msg_index = 0;
		char stripped_message[MSG_SIZE] = { 0 };
		for (int i = 0; i < HAMM_MSG_SIZE; i++)
		{
			if (HammingPairingBitsIndexes[hamming_index] == i)
			{
				hamming_index++;
				continue;
			}
			stripped_message[msg_index] = BitTools_GetNBit(recovered_message, i) == 0 ? '0' : '1';
			msg_index++;
		}
		fwrite(stripped_message, sizeof(char), MSG_SIZE, ServerParams_s.file);
	}

	fclose(ServerParams_s.file);
}

/*!
******************************************************************************
\brief
Printing relevant data and statistics.
\return none
*****************************************************************************/
void ServerUtils_PrintOutput()
{
	printf("Received: %d bytes\n", ServerParams_s.message_size);
	printf("Wrote: %d bytes\n", ServerParams_s.message_size / HAMM_MSG_SIZE * MSG_SIZE);
	printf("Detected and corrected %d errors\n", ServerParams_s.bytes_fixed);
}

/************************************
* static implementation             *
************************************/

/*!
******************************************************************************
\brief
Returns the message without the hamming code. Making corrections if possible.
\return Message to be written.
*****************************************************************************/
uint32_t ServerUtils_StripHammingCode(uint32_t msg)
{
	bool error_detected = false;
	uint32_t ret = msg;

	// We will use the 3blue1brown method to find the errored index.
	// We will go from the highest index, and if there is difference in the pairty we will add 2^i.
	int i = 0;
	int index = -1;
	int index2 = 0;
	uint32_t masked;
	for (int j = HAMM_PAIRITY_BITS - 1; j >= 0; j--)
	{
		masked = msg & HammingMasks[j];
		if (BitTools_BitwiseXOR(masked))
		{
			error_detected = true;
			index += (int)pow(2,j);
		}
		i++;
	}

	if (error_detected)
	{
		ServerParams_s.bytes_fixed++;
		ret = BIT_FLIP(ret, index);
	}

	return ret;
}

void SenderUtils_OpenFile()
{
	printf("File name:");
	ASSERT(scanf("%s", ServerParams_s.filename) == 1, "Error in response in server");
	if (strcmp(ServerParams_s.filename, "quit"))
	{
		ServerParams_s.file = fopen(ServerParams_s.filename, "w");
		ASSERT(ServerParams_s.file != NULL, "Error in open file in server.");
		return;
	}
	ServerParams_s.quit = true;
}

void ServerUtils_SessionInit()
{
	SenderUtils_OpenFile();
	if (ServerParams_s.quit) return;
	ServerParams_s.socket = SocketTools_CreateSocket(ServerArgs_s.ip, ServerArgs_s.port, CLIENT);
}
