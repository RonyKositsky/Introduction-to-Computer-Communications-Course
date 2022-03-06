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
#include <WinSock2.h>
#include <stdio.h>
#include <stdint.h>
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
	SOCKET socket;
	uint32_t message;
	FILE* file;
	char ServerRecordBuffer[MAX_BUFFER];
	int BufferFilledBytes;
	char ParsedMassege[MAX_BUFFER];
}ServerParams;

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
static ServerParams ServerParams_s;
static ServerOutputParams ServerOutParams_s;

/************************************
*      static functions             *
************************************/
static void ServerUtils_PrintOutput();
void SenderUtils_OpenFile();
static uint32_t ServerUtils_StripHammingCode(uint32_t message);

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

	// Init params.
	ServerParams_s.socket = SocketTools_CreateSocket(ServerArgs_s.ip, ServerArgs_s.port, SERVER);
	//SenderUtils_OpenFile();
}

/*!
******************************************************************************
\brief
Teardown the server.
\return none
*****************************************************************************/
void ServerUtils_ServerTearDown()
{
	closesocket(ServerParams_s.socket);
	fclose(ServerParams_s.file);
	ServerUtils_PrintOutput();
}

/*!
******************************************************************************
\brief
 Waiting to recieve new message.
\return QUIT if we want to finish sequence. Else, new message.
*****************************************************************************/
void ServerUtils_WaitForMessage()
{
	uint32_t rm;
	SOCKET s = accept(ServerParams_s.socket, NULL, NULL);
	int status = recv(s, &rm, sizeof(uint32_t), 0);
	ServerParams_s.message = ntohl(rm);
}

/*!
******************************************************************************
\brief
 Handeling new received message.
\return none
*****************************************************************************/
void ServerUtils_HandleMessage(int bytesRecived)
{
	if (ServerParams_s.file == NULL)
	{
	//	ServerParams_s.file = fopen(ServerArgs_s.filename, "wb");
	}

	ServerParams_s.BufferFilledBytes += bytesRecived; 
	ServerUtils_ParseMassage();
	// TODO: Make sure if we need to write it to file.
	//write_msg_to_file(file, parsed_msg);
}

/************************************
* static implementation             *
************************************/

/*!
******************************************************************************
\brief
Printing relevant data and statistics.
\return none
*****************************************************************************/
static void ServerUtils_PrintOutput()
{
	fprintf(stderr, "Received: %d bytes\n", ServerOutParams_s.BytesRecieved);
	fprintf(stderr, "Wrote: %d bytes\n", ServerOutParams_s.BytesWritten);
	fprintf(stderr, "Detected and corrected %d errors\n", ServerOutParams_s.FramesFixed);
}

//void ServerUtils_ParseMassage() {
//	int total_iterations = ServerParams_s.BufferFilledBytes / HAMM_MSG_SIZE;
//	for (int i = 0; i < total_iterations; i++) 
//	{
//		ServerUtils_ParseNextBytesGroup();
//		ServerOutParams_s.BytesRecieved += HAMM_MSG_SIZE;
//	}
//}


void ServerUtils_ParseNextBytesGroup()
{
	//GET NEXT NUMBER.
	// uint32_t num = GetNextNum();
	//uint32_t num = 0;
	//uint32_t strippedNumber = ServerUtils_StripHammingCode(num);
	//WRITE TO FILE.
	// Write to file(strippedNumber);
}

/*!
******************************************************************************
\brief
Returns the message without the hamming code. Making corrections if possible.
\return Message to be written.
*****************************************************************************/
uint32_t ServerUtils_StripHammingCode(uint32_t message)
{
	uint32_t ret = message;
	uint32_t paitiy = BitTools_BitwiseXOR(message);
	if (!paitiy) // No error.
		return ret;

	// TODO: If we have 2+ errors.
	
	// We will use the 3blue1brown method to find the errored index.
	// We will go from the highest index, and if there is difference in the pairty we will add 2^i.
	int i = 0;
	int index = 0;
	for (int j = HAMM_PAIRITY_BITS - 1; j >= 0; j--)
	{
		uint32_t masked = message & HammingMasks[j];
		uint32_t newPairity = BitTools_BitwiseXOR(masked);
		if (newPairity != CHECK_BIT(message, HammingPairingBitsIndexes[j]))
		{
			index += (int)pow(2,i);
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
	char filename[1000];
	strcpy(filename, "C:\\GitUni\\Introduction-to-Computer-Communications-Course\\res.txt");
	ServerParams_s.file = fopen(filename, "wb");
}