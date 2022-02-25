/*!
******************************************************************************
\file ServerUtils.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "ServerUtils.h"
#include <WinSock2.h>
#include <stdio.h>
#include "../Utilities/Definitions.h"
#include "../Utilities/SocketTools.h"

/************************************
*      definitions                 *
************************************/

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

/************************************
*      variables                    *
************************************/
static ServerArguments ServerArgs_s;
static ServerParams ServerParams_s;

/************************************
*      static functions             *
************************************/
static void ServerUtils_PrintOutput();

/************************************
*       API implementation          *
************************************/
/*!
******************************************************************************
\brief
Initialize func
\details
Must be called only once
\param
 [in] counter_val - reset counter value
 [out] out_val    -
\return none
*****************************************************************************/
void ServerUtils_ServerInit(char* argv[])
{
	memset(&ServerArgs_s, 0, sizeof(ServerArguments));
	memset(&ServerParams_s, 0, sizeof(ServerParams));

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

void ServerUtils_ServerTearDown()
{
	//send_msg_to_client(sock, &channel_addr);
	closesocket(ServerParams_s.socket);
	fclose(ServerParams_s.file);
	ServerUtils_PrintOutput();
}

int ServerUtils_WaitForMessage()
{
	return 1;
}


/************************************
* static implementation             *
************************************/
static void ServerUtils_PrintOutput()
{
	// TODO: implement it.
}