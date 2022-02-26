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
static void ServerUtils_ParseNexrtBytesGroup();
static void ServerUtils_StripHammingCode(char msg[2], char result[2]);

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

	// TODO: Implement bind.
	//bind_socket(sock, &server_addr);
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

	ServerParams_s.BufferFilledBytes += bytesRecived; //add the number of recieved bytes to the relevant field
	ServerUtils_ParseMassage();
	// TODO: Make sure if we need to write it to file?
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
		ServerUtils_ParseNexrtBytesGroup();
		ServerOutParams_s.BytesRecieved += HAMM_MSG_SIZE;
	}
}


void ServerUtils_ParseNexrtBytesGroup()
{
	int i, orig_mod, orig_index, res_mod, res_index;
	char cur_hammed[2];
	char cur[2];
	for (i = 0; i < 8; i++)
	{
		orig_mod = 15 * i % 8;
		orig_index = (int)floor((15 * i) / 8);
		res_mod = 11 * i % 8;
		res_index = (int)floor(11 * i / 8);
		BitTools_GetNextNBists(15, orig_index, orig_mod, bytes, cur_hammed);
		ServerUtils_StripHammingCode(cur_hammed, cur);
		BitTools_ConcatenationMassage(MSG_SIZE, res_index, res_mod, result, cur);
	}
}

// takes the MSB 15 bits in msg and strips them into 11 bits of unhammed result.
// verifies no errors happened
void ServerUtils_StripHammingCode(char msg[2], char result[2]) {
	char bits[HAMMED_MSG_SIZE];
	unsigned char first, second;
	char temp[2];
	char eleven_bits[11];
	char checkbits[4];
	char position[4];
	int ind;
	get_msg_bits(msg, bits, HAMMED_MSG_SIZE);
	first = (bits[2] << 7) | (bits[4] << 6) | (bits[5] << 5) | (bits[6] << 4) | (bits[8] << 3) |
		(bits[9] << 2) | (bits[10] << 1) | bits[11];
	second = (bits[12] << 7) | (bits[13] << 6) | (bits[14] << 5);
	temp[0] = first; temp[1] = second;
	get_msg_bits(temp, eleven_bits, 11);
	calc_checkbits(eleven_bits, checkbits);
	position[0] = checkbits[3] ^ bits[7]; position[1] = checkbits[2] ^ bits[3];
	position[2] = checkbits[1] ^ bits[1]; position[3] = checkbits[0] ^ bits[0];
	ind = position_array_to_index(position);
	// if an error was detected:
	if (ind != 0) {
		output_params[2]++;
		bits[ind - 1] = 1 - bits[ind - 1];
		if (ind > 8) {
			second = (bits[12] << 7) | (bits[13] << 6) | (bits[14] << 5);
		}
		else {
			first = (bits[2] << 7) | (bits[4] << 6) | (bits[5] << 5) | (bits[6] << 4) | (bits[8] << 3) |
				(bits[9] << 2) | (bits[10] << 1) | bits[11];
		}
	}
	result[0] = first;
	result[1] = second;
}