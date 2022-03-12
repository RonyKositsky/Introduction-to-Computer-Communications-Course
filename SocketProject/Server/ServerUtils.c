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

typedef struct
{
	int BytesRecieved;
	int BytesWritten;
	int FramesFixed;
}ServerOutputParams;

/************************************
*      variables                    *
************************************/
static ServerArguments ServerArgs_s;
static ServerOutputParams ServerOutParams_s;
ServerParams ServerParams_s;

/************************************
*      static functions             *
************************************/
void SenderUtils_OpenFile();
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
	memset(&ServerOutParams_s, 0, sizeof(ServerOutputParams));

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
		int j = 0;
		for (int i = 0; i < HAMM_MSG_SIZE; i++)
		{
			if (HammingPairingBitsIndexes[j] == i)
			{
				j++;
				continue;
			}
			bit = BitTools_GetNBit(recovered_message, i);
			fputs(&bit, ServerParams_s.file);
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
	fprintf(stderr, "Received: %d bytes\n", ServerOutParams_s.BytesRecieved);
	fprintf(stderr, "Wrote: %d bytes\n", ServerOutParams_s.BytesWritten);
	fprintf(stderr, "Detected and corrected %d errors\n", ServerOutParams_s.FramesFixed);
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
	uint32_t paitiy = BitTools_BitwiseXOR(msg);
	if (!paitiy) // No error.
		return msg;

	uint32_t ret = msg;

	// TODO: If we have 2+ errors.
	
	// We will use the 3blue1brown method to find the errored index.
	// We will go from the highest index, and if there is difference in the pairty we will add 2^i.
	int i = 0;
	int index = 0;
	for (int j = HAMM_PAIRITY_BITS - 1; j >= 0; j--)
	{
		uint32_t masked = msg & HammingMasksCehcks[j];
		if (BitTools_BitwiseXOR(masked) != 0)
		{
			index += (int)pow(2,i) - 1;
		}
		i++;
	}

	ret = BIT_FLIP(ret, index);
	return ret;
}

void SenderUtils_OpenFile()
{
	//TODO: Handle errors, quit.
	printf("File name:");
	//scanf("%s", SenderArgs_s.filename);
	char filename[FILE_NAME_BUFFER];
	strcpy(filename, "C:\\GitUni\\Introduction-to-Computer-Communications-Course\\res.txt");
	ServerParams_s.file = fopen(filename, "wb");
}

void ServerUtils_SessionInit()
{
	SenderUtils_OpenFile();
	if (ServerParams_s.quit) return;
	ServerParams_s.socket = SocketTools_CreateSocket(ServerArgs_s.ip, ServerArgs_s.port, SERVER);
	ServerParams_s.accepted_socket = accept(ServerParams_s.socket, NULL, NULL);
}
