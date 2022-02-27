/*!
******************************************************************************
\file ChannelUtils.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include <winsock2.h>
#include "ChannelUtils.h"
#include "../Utilities/Definitions.h"

/*!
******************************************************************************
\brief
Reading user input to our variables.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/

void ChannelUtils_ParseArguments(int argc, char* argv[])
{
	int optind;

	for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++) {
		switch (argv[optind][-1]) {
		case '-d':  ChProps_s.channel_noise_type = DETERMINISTIC; break;
		case '-r':  ChProps_s.channel_noise_type = RANDOM; break;
		}
	}

	ChProps_s.port			= atoi(argv[1])	;
	ChProps_s.server_ip		= argv[2]		;
	ChProps_s.server_port	= atoi(argv[3])	;
	
	if (ChProps_s.channel_noise_type == DETERMINISTIC) {
		ChProps_s.cycle_length = atoi(argv[4]);
	}

	else if (ChProps_s.channel_noise_type == RANDOM) {
		ChProps_s.prob = atoi(argv[4]);
		ChProps_s.seed = atoi(argv[5]);
	}
}

/*!
******************************************************************************
\brief
Initialize the channel.
\param
 [in] argv - arguments from the user.
\return none
*****************************************************************************/

void ChannelUtils_ChannelInit(int argc, char* argv[])
{
	memset(&ChParams_s, 0, sizeof(ChannelParams));
	memset(&ChProps_s, 0, sizeof(ChProps_s));
	ChannelUtils_ReadInput(argv);
	ChannelUtils_ParseArguments(argc, argv);
	{

    //channel as server (recieves messages from the sender)
    SOCKET sender_sock = SocketTools_CreateSocket();
    SocketTools_CreateAddress(&ChParams_s.my_addr, ChProps_s.port, NULL);
    SocketTools_BindSocket(sender_sock, &ChParams_s.my_addr);

		// channel as sender (sends messages to the server)
		ChParams_s.server_sock = SocketTools_CreateSocket();
		SocketTools_CreateAddress(ChParams_s.server_addr, ChProps_s.server_port, ChProps_s.server_ip);
	}
}
/*!
******************************************************************************
\brief
Adding noise to a given message. 
\return none
*****************************************************************************/
void ChannelUtils_AddNoise(char* msg, int seed, int	prob, int cycle_length) {
	if (ChProps_s.channel_noise_type == DETERMINISTIC) {
		Channelutils_AddDeterministicNoise(CHANNEL_REC_BUF, cycle_length);
	}
	else if (ChProps_s.channel_noise_type == RANDOM) {
		Channelutils_AddDeterministicNoise(CHANNEL_REC_BUF, seed, prob);
	}
}
/*!
******************************************************************************
\brief
Preparing channel read massage that was sent.
\return none
*****************************************************************************/
void ChannelUtils_PrepareReadMsg()
{
	ChParams_s.readMsg.sock			= ChParams_s.server_sock	;
	ChParams_s.readMsg.addr			= ChParams_s.server_addr	;
	ChParams_s.readMsg.buf			= CHANNEL_REC_BUF			;
	ChParams_s.readMsg.buf_size		= MAX_BUFFER				;
}

/*!
******************************************************************************
\brief
Preparing channel to writing massage.
\return none
*****************************************************************************/
void ChannelUtils_PrepareWriteMsg()
{
	ChParams_s.readMsg.sock		= ChParams_s.sender_sock			;
	ChParams_s.readMsg.addr		= ChParams_s.sender_addr			;
	ChParams_s.readMsg.buf		= CHANNEL_REC_BUF					;
	ChParams_s.readMsg.buf_size = ChParams_s.msg_size_from_sender	;
}

/*!
******************************************************************************
\brief
Tearing down the channel.
\return none
*****************************************************************************/
void ChannelUtils_ChannelTearDown()
{
	ChannelUtils_PrepareReadMsg();
	ChannelUtils_PrepareWriteMsg();
	SocketTools_ReadMessage(&ChParams_s.readMsg); //read the message from the server 
	SocketTools_SendMessage(&ChParams_s.writeMsg);
	closesocket(ChParams_s.server_sock);
	closesocket(ChParams_s.sender_sock);

	//TODO: Implement printing below.

	//char* sender_ip_str = inet_ntoa((ChParams.sender_addr)->sin_addr);
	//print_channel_output(ChArgs.server_ip, sender_ip_str);
}

void Channelutils_FlipBit(int ind, char* msg) {
	
	if (msg[ind] == '1') {
		msg[ind] = '0';
	}

	else {
		msg[ind] = '1';
	};
};

void Channelutils_AddDeterministicNoise(char* msg, int cycle_length, int bits_flipped_cnt){
	for (int c = 0; msg[c] != '\0'; c++) {
		if (c % cycle_length == 0) {
			Channelutils_FlipBit(msg, c);
			bits_flipped_cnt++;

		};
	};
	};


/*****************************************************************************/
void Channelutils_AddRandomNoise(char* msg, int seed, int prob) {

};
/*****************************************************************************/
//void Channelutils_AddDeterministicNoise(char* msg, int cycle_length) {
//
//};
