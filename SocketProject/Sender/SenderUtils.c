/*!
******************************************************************************
\file SenderUtils.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <stdint.h>
#include "SenderUtils.h"
#include "../Utilities/Definitions.h"
#include "../Utilities/SocketTools.h"
#include "../Utilities/BitTools.h"

/************************************
*       types                       *
************************************/
typedef struct
{
	char* ip;
	int port;
	char* filename;
}SenderArguments;

typedef struct
{
	SOCKET socket;
	FILE* file;
	struct sockaddr_in channel_addr;
	char msg_buffer[MSG_SIZE];
	char msg_hamming[HAMM_MSG_SIZE];
}SenderParams;

/************************************
*      variables                 *
************************************/
static char SEND_BUF[MAX_BUFFER];
static char REC_BUF[MAX_BUFFER];
static int send_buf_cur_ind;
static SenderArguments SenderArgs_s;
static SenderParams SenderParams_s;

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
	SenderArgs_s.ip = argv[1];
	SenderArgs_s.port = atoi(argv[2]);
	SenderArgs_s.filename = argv[3];

	// Init params.
	SenderParams_s.file = fopen(SenderArgs_s.filename, "rb");
	SenderParams_s.socket = SocketTools_CreateSocket();
	SocketTools_CreateAddress(&SenderParams_s.channel_addr, SenderArgs_s.port, SenderArgs_s.ip);
}


/*!
******************************************************************************
\brief
Append the massage with the hamming code to the writing buffer.
We are filling the buffer until it is full, and then sending it.
\return none
*****************************************************************************/
void SenderUtils_AppendToBuffer()
{
	for (int i = 0; i < HAMM_MSG_SIZE; i++) 
	{
		SEND_BUF[send_buf_cur_ind + i] = SenderParams_s.msg_hamming[i];
	}
	send_buf_cur_ind += HAMM_MSG_SIZE;
	if (send_buf_cur_ind > MAX_BUFFER - HAMM_MSG_SIZE)
	{
		Sleep(10);
		// TODO: Send the message.
		//send_message(sock, SEND_BUF, LARGE_NUM, addr);
		send_buf_cur_ind = 0;
	}
}

/*!
******************************************************************************
\brief
Reading from the input file the next 26 bits.
\return Number of bits read.
*****************************************************************************/
int SenderUtils_ReadBytesFromFile()
{
	if (SenderParams_s.file == NULL)
	{
		fprintf(stderr, "NULL file.\n");
		exit(-1);
	}
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
	//Sending the rest of the message.
	MessageVars sendMsg =
	{
		.sock = SenderParams_s.socket,
		.buf = SEND_BUF,
		.buf_size = send_buf_cur_ind,
		.addr = &SenderParams_s.channel_addr
	};
	SocketTools_SendMessage(&sendMsg);

	// Waiting for the reciever to get the message.
	MessageVars readMessage =
	{
		.sock = SenderParams_s.socket,
		.buf = REC_BUF,
		.buf_size = MAX_BUFFER,
		.addr = &SenderParams_s.channel_addr
	};
	SocketTools_ReadMessage(&readMessage);
	
	// Closing.
	SenderUtils_PrintOutput();
	closesocket(SenderParams_s.socket);
	fclose(SenderParams_s.file);
}

void SenderUtils_AddHammCode()
{
	uint32_t message = SenderUtils_ConvertMessageToUint(SenderParams_s.msg_buffer);
	uint32_t messageHamming = BitTools_GetMassageWithHamming(message);
	BitTools_ConvertUintToString(SenderParams_s.msg_hamming, HAMM_MSG_SIZE, messageHamming);
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