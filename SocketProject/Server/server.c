
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Utilities/Definitions.h"
#include <WinSock2.h>

char SERVER_REC_BUF[MAX_BUFFER];
int filled_bytes_in_REC_BUF[1];

int main(int argc, char* argv[]){
	int port = atoi(argv[1]);
	char* filename = argv[2];
	SOCKET sock = create_socket();
	struct sockaddr_in server_addr, channel_addr;
	char parsed_msg[MAX_BUFFER];
	int status = 0;
	FILE* file = fopen(filename, "rb");
	memset(&server_addr, 0, sizeof(server_addr));
	memset(&channel_addr, 0, sizeof(channel_addr));
	create_address(&server_addr, port, INADDR_ANY);
	bind_socket(sock, &server_addr);
	while (1)
	{
		status = wait_for_message_server(sock, SERVER_REC_BUF, MAX_BUFFER, &channel_addr);
		if (status == -1) 
		{
			//user inserted "End"
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
	send_msg_to_client(sock, &channel_addr);
	closesocket(sock);
	fclose(file);
	print_server_output();
	return 0;
}