/*!
******************************************************************************
\main.c
\date 15 May 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

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
*      variables                    *
************************************/
bool run;
WSADATA wsdata;
char input[MAX_LEN];
char* ip;

/************************************
*      static functions             *
************************************/
static void GetUserArgument();
static int Init();
static bool LegalInput();

/************************************
*       main			          *
************************************/
int main(int argc, char* argv[])
{
	if (Init() || argc != 2)
	{
		return FAIL;
	}
	
	ip = argv[1];
	GetUserArgument();
	
	while (run)
	{
		if (LegalInput) 
		{
			dnsQuery(input, ip);
		}

		GetUserArgument();
	}

	printf("Finised.");
	WSACleanup();
	return SUCCESS;
}

/************************************
* static implementation             *
************************************/
static int Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != SUCCESS)
	{
		perror(">ERROR: Failed initialising Winsock.\n");
		return FAIL;
	}

	run = true;
	return SUCCESS;
}

static void GetUserArgument()
{
	printf("nsclient>");
	scanf("%s", input);
	run = strncmp(input, "quit", 5);
}

static bool LegalInput()
{
	return true;
	perror(">ERROR: BAD NAME");
}


