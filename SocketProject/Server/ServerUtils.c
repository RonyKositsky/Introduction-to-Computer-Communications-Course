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
	//ServerArgs_s.ip = argv[1];
	//ServerArgs_s.port = atoi(argv[2]);

	ServerArgs_s.ip = LOCAL_HOST_IP;
	ServerArgs_s.port = 6343;

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
	int char_index = 0;
	char byte = 0;
	uint32_t msg, recovered_message, prev_message;

	while (index < (int)ServerParams_s.message_size)
	{
		msg = 0;
		recovered_message = 0;

		// Get message as uint.
		for (int i = 0; i < HAMM_MSG_SIZE; i++)
		{
			if (ServerParams_s.message[index] == '1')
			{
				BIT_SET(msg, i);
			}
			index++;
		}

		// remove noise.
		recovered_message = ServerUtils_StripHammingCode(msg);

		// Add to file buffer.
		int hamming_index = 0;
		for (int i = 0; i < HAMM_MSG_SIZE; i++)
		{
			if (HammingPairingBitsIndexes[hamming_index] == i)
			{
				hamming_index++;
				continue;
			}
			if (BitTools_GetNBit(recovered_message, i))
			{
				byte += (char)((1 << char_index));
			}
			if (++char_index == BYTE_LENGTH)
			{
				fwrite(&byte, sizeof(char), 1, ServerParams_s.file);
				byte = 0;
				char_index = 0;
			}
		}
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
	printf("Received: %d bytes\n", ServerParams_s.message_size / 8);
	printf("Wrote: %d bytes\n", ServerParams_s.message_size / HAMM_MSG_SIZE * MSG_SIZE / 8);
	printf("Detected and corrected %d errors\n", ServerParams_s.bits_fixed);
}


/*!
******************************************************************************
\brief
 Initializing new server session.
\return none
*****************************************************************************/
void ServerUtils_SessionInit()
{
	SenderUtils_OpenFile();
	if (ServerParams_s.quit) return;
	ServerParams_s.socket = SocketTools_CreateSocket(ServerArgs_s.ip, ServerArgs_s.port, CLIENT, false, RECIEVER);
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
		ServerParams_s.bits_fixed++;
		ret = BIT_FLIP(ret, index);
	}

	return ret;
}

/*!
******************************************************************************
\brief
Opening the file. If "quit" is entered we finish the procedure.
\return none.
*****************************************************************************/
static void SenderUtils_OpenFile()
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

