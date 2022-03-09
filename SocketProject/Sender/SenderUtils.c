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
static void SenderUtils_PrintOutput();
static uint32_t SenderUtils_ConvertMessageToUint(char* massage);

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
}


/*!
******************************************************************************
\brief
Reading from the input file the next 26 bits.
\return Number of bits read.
*****************************************************************************/
int SenderUtils_ReadBytesFromFile()
{
	int err = fread(SenderParams_s.msg_buffer, 1, MSG_SIZE, SenderParams_s.file);
	if (err < 0)
	{
		fprintf(stderr, "Read from file failed.\n");
		exit(-1);
	}
	return err;
}

/*!
******************************************************************************
\brief
Tear down our sender.
\return none
*****************************************************************************/
void SenderUtils_SenderTearDown()
{	
	// Closing.
	SenderUtils_PrintOutput();
	closesocket(SenderParams_s.socket);
	fclose(SenderParams_s.file);
}

void SenderUtils_AddHammCode()
{
	uint32_t message = SenderUtils_ConvertMessageToUint(SenderParams_s.msg_buffer);
	SenderParams_s.messageHamming = BitTools_GetMassageWithHamming(message);
}

void SenderUtils_OpenFile()
{
	//TODO: Handle errors, quit.
	printf("File name:");
	//scanf("%s", SenderArgs_s.filename);
	strcpy(SenderParams_s.filename, "C:\\GitUni\\Introduction-to-Computer-Communications-Course\\file.txt");
	SenderParams_s.file = fopen(SenderParams_s.filename, "rb");
	
	//TODO: IF "quit" quit = 1;
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

/************************************
* static implementation             *
************************************/

/*!
******************************************************************************
\brief
Printing statistics and relevant data.
\return none
*****************************************************************************/
static void SenderUtils_PrintOutput()
{
	//TODO
}


/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
uint32_t SenderUtils_ConvertMessageToUint(char* massage)
{
	uint32_t val = 0;
	int i = 0;
	if (massage == NULL)
		return 0;

	while (massage[i] == '0' || massage[i] == '1')
	{
		val <<= 1;
		val += massage[i] - '0';
		i++;
	}

	return val;
}

/*!
******************************************************************************
\brief
Adding to recieved message 5 pairity bits.
\return Message with uninitialized hamming code.
*****************************************************************************/
uint32_t BitTools_GetMassageWithHamming(uint32_t message)
{
	uint32_t hammeingMessage = message;
	for (int i = 0; i < HAMM_PAIRITY_BITS; i++)
	{
		uint32_t masked = message & HammingMasks[i];
		uint32_t pairity = BitTools_BitwiseXOR(masked);
		
		// Set to 1 if pairity is 1, otherwise the default is 0 so we don't need to change it.
		if (!pairity) continue;
		BIT_SET(message, HammingMasks[i]);
	}
	return hammeingMessage;
}