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
static void SenderUtils_HammingErrorCorrectionCode(char msg[2], char hammed_msg[2]);

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
Adding hamming code to the message we have read.
\return none
*****************************************************************************/
void SenderUtils_AddHammCode()
{
	//TODO: Refactor.
	int i;
	int orig_mod, orig_index, hammed_mod, hammed_index;
	char cur[2];
	char hammed_cur[2];
	for (i = 0; i < 8; i++)
	{
		orig_mod = 11 * i % 8;
		orig_index = (int)floor(11 * i / 8);
		hammed_mod = 15 * i % 8;
		hammed_index = (int)floor(15 * i / 8);
		BitTools_GetNextNBists(11, orig_index, orig_mod, SenderParams_s.msg_buffer, cur);
		SenderUtils_HammingErrorCorrectionCode(cur, hammed_cur);
		BitTools_ConcatenationMassage(HAMM_MSG_SIZE, hammed_index, hammed_mod, SenderParams_s.msg_hamming, hammed_cur);
	}
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
Creating the hamming code error correction code.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/
static void SenderUtils_HammingErrorCorrectionCode(char msg[2], char hammed_msg[2])
{

	//TODO : Refator funciton.
	unsigned char first = 0;
	unsigned char second = 0;
	unsigned char bits[11];
	char checkbits[4];

	// put all 11 msg bits separated in bits array
	BitTools_GetMessageBits(msg, bits, 11);

	// make first and second contain the data bits in the right places
	first = first | (bits[0] << 5) | (bits[1] << 3) | (bits[2] << 2) | (bits[3] << 1);
	second = second | (bits[4] << 6) | (bits[5] << 5) | (bits[6] << 4) | (bits[7] << 3) | (bits[8] << 2)
		| (bits[9] << 1) | bits[10];
	second = second << 1;
	// calculate check bits by XORing the right bits
	BitTools_CheckBits(bits, checkbits);
	// place checkbits in first and second
	first = first | (checkbits[0] << 7) | (checkbits[1] << 6) | (checkbits[2] << 4) | checkbits[3];
	hammed_msg[0] = first;
	hammed_msg[1] = second;
}