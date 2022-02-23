#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

//  TODO: migrate 2 auxilary functions to different modules

/* ******************************************** */
void send_file(FILE *fp, int sockfd_s)
{
    int n;
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd_s, data, sizeof(data), 0) == -1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}
/* ******************************************** */
void write_file(int sockfd_s)
{
    int n;
    FILE *fp;
    char *filename = "recv.txt";
    char buffer[SIZE];

    fp = fopen(filename, "w");
    while (1)
    {
        n = recv(sockfd_s, buffer, SIZE, 0);
        if (n <= 0)
        {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}
/* ******************************************** */

int main()
{
    char *ip = "127.0.0.1";
    int port_s = 8080;
    int port_r = 8081;
    int e;

    int sockfd_s, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    /* ************************************************************ */

    /* 1st socket - handling file received from sender */
    /* based on server implemtation */
    sockfd_s = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_s < 0)
    {
        perror("[-]Error in sender socket");
        exit(1);
    }
    printf("[+]Sender socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port_s;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd_s, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e < 0)
    {
        perror("[-]Error in bind with sender");
        exit(1);
    }
    printf("[+]Binding (with sender) successfull.\n");

    if (listen(sockfd_s, 10) == 0)
    {
        printf("[+]Channel is Listening to sender....\n");
    }
    else
    {
        perror("[-]Error in listening");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd_s, (struct sockaddr *)&new_addr, &addr_size);

    // TODO: add noise to the file received from sender

    write_file(new_sock);
    printf("[+]Data written in the file successfully.\n");

    /* ************************************************************ */

    /* 2nd socket - send encoded file to receiver */
    /* based on client implementation */
    int sockfd_r;
    struct sockaddr_in server_addr_r;

    /*
    FILE *fp;
    char filename[10];
    printf("Enter file name: ");
    scanf("%s", filename);
    printf("[+]file's name: %s\n", filename);
    */

    sockfd_r = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_r < 0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Receiver socket created successfully.\n");

    server_addr_r.sin_family = AF_INET;
    server_addr_r.sin_port = port_r;
    server_addr_r.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd_r, (struct sockaddr *)&server_addr_r, sizeof(server_addr_r));
    if (e == -1)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Connected to Receiver.\n");

/*
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("[-]Error in reading file.");
        exit(1);
    }

    send_file(fp, sockfd_r);
    printf("[+]File data sent successfully.\n");
*/

//  TODO: log statistics to console

    printf("[+]Closing the connection.\n");
    close(sockfd_r);
    return 0;
}