/*!
******************************************************************************
\main.c
\date 15 May 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/

/************************************
*      include                      *
************************************/
#include <stdbool.h>
#include "dns.h"

/************************************
*      definitions                 *
************************************/
#define MAX_LEN 1024

/************************************
*       types                       *
************************************/

/************************************
*      variables                    *
************************************/
bool run = false;
WSADATA wsdata;

/************************************
*      static functions             *
************************************/
static void GetUserArgument();
static void Init();

/************************************
*       main			          *
************************************/
int main(int argc, char* argv[])
{
	//Init();
	char* ip = argv[1];
	char input[MAX_LEN] = "www.google.com";
	

	//while (run)
	//{
		//GetUserArgument();
		dnsQuery(input, ip);
	//}

	return 0;
}

/************************************
* static implementation             *
************************************/
static void Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != SUCCESS)
	{
		printf("\n\tERROR: Failed Initialising Winsock (%d)", WSAGetLastError());
		return 1;
	}
}

static void GetUserArgument()
{
	//sscanf("%s", input);
	//run = !strncmp(input, "quit", 5);
}


