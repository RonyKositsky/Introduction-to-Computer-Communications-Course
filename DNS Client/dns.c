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

/************************************
*      static functions             *
************************************/
static void SetDnsRequest(DNS_HEADER* dns);
static void SendDnsQuery(SOCKET s, char* buf, char* name, SOCKADDR_IN dest, QUESTION* info);
static void GetAnswer(SOCKET s, char* buf, SOCKADDR_IN dest, char* name, char* host_name);
static unsigned char* ReadName(unsigned char* reader, unsigned char* buffer, int* count);
static void DnsFormat(unsigned char* dns, unsigned char* host);

/************************************
*       API implementation          *
************************************/
void dnsQuery(unsigned char* host, char* ip) 
{
    unsigned char buf[BUFFER_SIZE], *name;
    SOCKET s;
    QUESTION* info = NULL;

    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Configure the sockaddress structure with information of DNS server
    SOCKADDR_IN dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);
    dest.sin_addr.s_addr = inet_addr(ip);

    // Set the DNS structure to standard queries
    DNS_HEADER* dns = (struct DNS_HEADER*)&buf;

    SetDnsRequest(dns);
    name = (unsigned char*)&buf[sizeof(struct DNS_HEADER)];

    DnsFormat(name, host);
    SendDnsQuery(s, buf, name, dest, info);
    GetAnswer(s, buf, dest, name, host);
    dns = (struct DNS_HEADER*)buf;
}

/************************************
* static implementation             *
************************************/
static void SetDnsRequest(DNS_HEADER* dns) 
{
    dns->id = (unsigned short)(htons(GetCurrentProcessId()));
    dns->qr = 0;      // This is a query
    dns->opcode = 0;  // This is a standard query
    dns->aa = 0;      // Not Authoritative
    dns->tc = 0;      // This message is not truncated
    dns->rd = 1;      // Recursion Desired
    dns->ra = 0;      // Recursion not available! hey we dont have it (lol)
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1);  // we have only 1 question
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;
}

static void SendDnsQuery(SOCKET s, char* buf, char* name, SOCKADDR_IN dest, QUESTION* info) 
{
    info = (struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen((const char*)name) + 1)];     // fill it
    info->qtype = htons(1);                                                                         // ipv4 address
    info->qclass = htons(1);                                                                        // its internet

    if (sendto(s, (char*)buf, sizeof(struct DNS_HEADER) + (strlen((const char*)name) + 1) + sizeof(struct QUESTION), 0, (struct sockaddr*)&dest, sizeof(dest)) == SOCKET_ERROR) 
    {
        perror("\nERROR: Failed sending query.");
    }
}

static void GetAnswer(SOCKET s, char* buf, SOCKADDR_IN dest, char* name, char* host_name) 
{
    int size = sizeof(dest);
    RES_RECORD answer;
    if (recvfrom(s, (char*)buf, 65536, 0, (struct sockaddr*)&dest, &size) == SOCKET_ERROR) 
    {
        perror("\nERROR: Failed recieving query.");
    }

    // reading answers
    char*  reader = &buf[sizeof(struct DNS_HEADER) + (strlen((const char*)name) + 1) + sizeof(struct QUESTION)];
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
        for (int j = 0; j < ntohs(answer.resource->data_len); j++) 
        {
            answer.rdata[j] = reader[j];
        }

        answer.rdata[ntohs(answer.resource->data_len)] = '\0';
        reader = reader + ntohs(answer.resource->data_len);
        long* p = (long*)answer.rdata;
        a.sin_addr.s_addr = (*p);
        printf("\nIPv4 = %s", inet_ntoa(a.sin_addr));
    }
    else 
    {
        answer.rdata = ReadName(reader, buf, &stop);
        reader = reader + stop;
        printf("\nCould not reach `%s`", host_name);
    }
}

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
            offset = (*reader) * 256 + *(reader + 1) - 49152;  // 49152 = 11000000 00000000 ;)
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

    // now convert 3www6*3com to www.*.com
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

static void DnsFormat(unsigned char* dns, unsigned char* host) 
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