
/*!
******************************************************************************
\file Server.c
\date 25 February 2022
\author Jonathan Matetzky & Rony Kosistky
ALL RIGHTS RESERVED
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include "ServerUtils.h"

/************************************
*			Main	                *
************************************/
int main(int argc, char* argv[]){
	
	int status;
	ServerUtils_ServerInit(argv);
	while (1)
	{
		status = ServerUtils_WaitForMessage();
		if (status == -1) 
		{
			// TODO: Implement quit.
			break;
		} 
		else 
		{
			if (file == NULL) {
				file = fopen(filename, "wb");
			}
			filled_bytes_in_REC_BUF[0] += status; //add the number of recieved bytes to the relevant field
			parse_msg(parsed_msg, filled_bytes_in_REC_BUF[0]); //shouldn't be just the last recieved bytes? 
			write_msg_to_file(file, parsed_msg);
		}
	}

	ServerUtils_ServerTearDown();
	return 0;
}