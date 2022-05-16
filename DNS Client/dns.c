/*!
******************************************************************************
\file dns.c
\date 15 May 2022
\author Jonathan Matetzky & Rony Kosistky
*****************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/************************************
*      include                      *
************************************/
#include "dns.h"
#include <stdbool.h>

/************************************
*      definitions                 *
************************************/
#define BUFFER_SIZE 65536
#define PORT 53
#define OFFSET_HEX (0xC000) // 11000000 00000000

/************************************
*      static functions             *
************************************/
static void InitDnsStruct(DNS_HEADER* dns);
static void ParseAnswer(SOCKET s, char* buf, SOCKADDR_IN dest, char* name, char* host_name);
static unsigned char* ReadName(unsigned char* reader, unsigned char* buffer, int* count);
static void GetFormat(unsigned char* dns, unsigned char* host);

/************************************
*       API implementation          *
************************************/
void dnsQuery(unsigned char* host, char* ip) 
{
    unsigned char buf[BUFFER_SIZE];
    QUESTION* info = NULL;

    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Configure the sockaddress structure with information of DNS server
    SOCKADDR_IN dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);
    dest.sin_addr.s_addr = inet_addr(ip);

    // Set the DNS structure to standard queries
    DNS_HEADER* dns = (struct DNS_HEADER*)&buf;
    InitDnsStruct(dns);
    
    unsigned char * name = (unsigned char*)&buf[sizeof(struct DNS_HEADER)];
    GetFormat(name, host);

    // Sdnging query.
    info = (struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen((const char*)name) + 1)];
    info->qtype = htons(1);
    info->qclass = htons(1);

    if (sendto(s, (char*)buf, sizeof(struct DNS_HEADER) + (strlen((const char*)name) + 1) + sizeof(struct QUESTION),
        0, (struct sockaddr*)&dest, sizeof(dest)) == SOCKET_ERROR)
    {
        perror(">ERROR: Failed sending query.\n");
    }

    ParseAnswer(s, buf, dest, name, host);
    
    closesocket(s);
}

/************************************
* static implementation             *
************************************/

/*!
******************************************************************************
\brief
 Initializing the struct of the header.
\return name.
*****************************************************************************/
static void InitDnsStruct(DNS_HEADER* dns) 
{
    dns->id = (unsigned short)(htons(GetCurrentProcessId()));
    dns->qr = 0;      // This is a query,
    dns->opcode = 0;  // This is a standard query.
    dns->aa = 0;      // Not Authoritative.
    dns->tc = 0;      // This message is not truncated.
    dns->rd = 1;      // Recursion Desired.
    dns->ra = 0;      // Recursion not available.
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1);  // we have only 1 question
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;
}

/*!
******************************************************************************
\brief
 Parsing the answer.
\return name.
*****************************************************************************/
static void ParseAnswer(SOCKET s, char* buf, SOCKADDR_IN dest, char* name, char* host_name) 
{
    int size = sizeof(dest);
    if (recvfrom(s, (char*)buf, 65536, 0, (struct sockaddr*)&dest, &size) == SOCKET_ERROR) 
    {
        perror(">ERROR: Failed recieving query.\n");
    }

    // reading answers
    char*  reader = &buf[sizeof(struct DNS_HEADER) + (strlen((const char*)name) + 1) + sizeof(struct QUESTION)];
    RES_RECORD answer;
    int stop = 0;

    answer.name = ReadName(reader, buf, &stop);
    reader = reader + stop;
    answer.resource = (struct R_DATA*)(reader);
    reader = reader + sizeof(struct R_DATA);

    SOCKADDR_IN a;
    host_name[strlen(host_name) - 1] = '\0';

    if (ntohs(answer.resource->type) == 1)
    {
        // Has IPv4 Address
        answer.rdata = (unsigned char*)malloc(ntohs(answer.resource->data_len));
        for (int i = 0; i < ntohs(answer.resource->data_len); i++) 
        {
            answer.rdata[i] = reader[i];
        }

        answer.rdata[ntohs(answer.resource->data_len)] = '\0';
        reader = reader + ntohs(answer.resource->data_len);
        long* p = (long*)answer.rdata;
        a.sin_addr.s_addr = (*p);
        printf("%s\n", inet_ntoa(a.sin_addr));
    }
    else 
    {
        answer.rdata = ReadName(reader, buf, &stop);
        reader = reader + stop;
        printf(">ERROR: NONEXISTENT\n");
    }
}

/*!
******************************************************************************
\brief
 parsing the name.
\return name.
*****************************************************************************/
static unsigned char* ReadName(unsigned char* reader, unsigned char* buffer, int* count) 
{
    unsigned int p = 0, offset;
    int i = 0;
    bool jumped = false;

    *count = 1;
    unsigned char*  name = (unsigned char*)malloc(256);
    name[0] = '\0';

    // read the names in 3www6*3com format
    while (*reader != 0) 
    {
        if (*reader >= 192) 
        {
            offset = (*reader) * 256 + *(reader + 1) - OFFSET_HEX; 
            reader = buffer + offset - 1;
            jumped = true;  // we have jumped to another location so counting wont go up!
        }
        else 
        {
            name[p++] = *reader;
        }

        reader = reader + 1;

        if (!jumped)
        {
            *count = *count + 1;  // if we havent jumped to another location then we can count up
        }
    }

    name[p] = '\0';  // string complete
    if (jumped) 
    {
        *count = *count + 1;  // number of steps we actually moved forward in the packet
    }

    // Convert to regular URL.
    for (int i = 0; i < (int)strlen((const char*)name); i++) 
    {
        p = name[i];
        for (int j = 0; j < (int)p; j++) 
        {
            name[i] = name[i + 1];
            i = i + 1;
        }
        name[i] = '.';
    }

    name[i - 1] = '\0';  // remove the last dot

    return name;
}

/*!
******************************************************************************
\brief
 This will convert www.google.com to 3www6google3com.
\param
 [in] host - the host.
 [in] dns - the server.
\return none
*****************************************************************************/
static void GetFormat(unsigned char* dns, unsigned char* host) 
{
    int lock = 0;

    strcat((char*)host, ".");

    for (int i = 0; i < (int)strlen((char*)host); i++) 
    {
        if (host[i] == '.') 
        {
            *dns++ = i - lock;
            for (; lock < i; lock++) 
            {
                *dns++ = host[lock];
            }
            lock++;  
        }
    }
    *dns++ = '\0';
}

