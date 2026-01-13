//**************************** TCP Server Client timeout Application ******************************
//  Copyright (c) 2021 Trenser
//  All Rights Reserved
//*****************************************************************************
//
// File       :client_timeout.c
// Summary    :Application code for TCP client
// Note       :server code will be in server_timeout.c
// Author     :Mimi C.S
// Date       :13/01/2026
//
//*****************************************************************************

//******************************* Include Files *******************************
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define PORT 8080
int make_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
//******************************.main.******************************
//Purpose :Application code for client with timeout
//Inputs  :Receive data from server
//Outputs :Sent data to server
//Return  :
//Notes   :server application code will be in server_timeout.c
//**********************************************************************************
int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    make_non_blocking(sock);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    fd_set writefds;
    struct timeval timeout;
    FD_ZERO(&writefds);
    FD_SET(sock, &writefds);
    timeout.tv_sec = 5; // connect timeout
    timeout.tv_usec = 0;
    int activity = select(sock + 1, NULL, &writefds, NULL, &timeout);
    if (activity == 0)
    {
        printf("Connection timeout\n");
        return -1;
    }
    send(sock, "Hello from client", 18, 0);
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    activity = select(sock + 1, &readfds, NULL, NULL, &timeout);
    if (activity == 0)
    {
        printf("Receive timeout\n");
        return -1;
    }
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    buffer[bytes] = '\0';
    printf("Server says: %s\n", buffer);
    close(sock);
    return 0;
}
