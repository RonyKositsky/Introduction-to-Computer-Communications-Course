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
#include "../Utilities/Definitions.h"
#include "../Utilities/SocketTools.h"
#include "../Utilities/BitTools.h"

/************************************
*       types                       *
************************************/
typedef struct
{
	int port;
	char* filename;
}ServerArguments;

typedef struct
{
	SOCKET socket;
	struct sockaddr_in server_addr;
	struct sockaddr_in channel_addr;
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
static void ServerUtils_ParseMassage();
static void ServerUtils_ParseNextBytesGroup();
static uint32_t ServerUtils_StripHammingCode();

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
	ServerArgs_s.port = atoi(argv[1]);
	ServerArgs_s.filename = argv[2];

	// Init params.
	ServerParams_s.socket = SocketTools_CreateSocket();
	ServerParams_s.file = fopen(ServerArgs_s.filename, "rb");
	SocketTools_CreateAddress(&ServerParams_s.server_addr, ServerArgs_s.port, INADDR_ANY);
	SocketTools_BindSocket(ServerParams_s.socket, &ServerParams_s.server_addr);
}

/*!
******************************************************************************
\brief
Teardown the server.
\return none
*****************************************************************************/
void ServerUtils_ServerTearDown()
{
	//send_msg_to_client(sock, &channel_addr);
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
int ServerUtils_WaitForMessage()
{
	return 1;
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
		ServerParams_s.file = fopen(ServerArgs_s.filename, "wb");
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

void ServerUtils_ParseMassage() {
	int total_iterations = ServerParams_s.BufferFilledBytes / HAMM_MSG_SIZE;
	for (int i = 0; i < total_iterations; i++) 
	{
		ServerUtils_ParseNextBytesGroup();
		ServerOutParams_s.BytesRecieved += HAMM_MSG_SIZE;
	}
}


void ServerUtils_ParseNextBytesGroup()
{
	//GET NEXT NUMBER.
	ServerUtils_StripHammingCode();
	//WRITE TO FILE.
}

/*!
******************************************************************************
\brief
Returns the message without the hamming code. Making corrections if possible.
\return Message to be written.
*****************************************************************************/
uint32_t ServerUtils_StripHammingCode()
{
	//TODO: Implement and do error calculation.
}