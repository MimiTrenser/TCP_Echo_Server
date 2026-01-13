//**************************** TCP Echo Server Application ******************************
//  Copyright (c) 2021 Trenser
//  All Rights Reserved
//*****************************************************************************
//
// File   : Server.c
// Summary    :Application code for TCP server
// Note    :Client code will be in client.c
// Author  :Mimi C.S
// Date   :13/01/2026
//
//*****************************************************************************

//******************************* Include Files *******************************
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024
//******************************.FUNCTION_HEADER.****************************** 
//Purpose :Application code for server
//Inputs  :Receive data from client as part of echo transfer
//Outputs :Sent data to client
//Return  :
//Notes   :Client application code will be in client.c
//********************************************************************************** 
int main()
{
    int server_fd, client_fd, return_r;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len
    char buffer[BUF_SIZE];

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket failed");
        return -1;
    }
    printf("Socket created successfully\n");
    // 2. Bind IP and Port to socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    return_r = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(return_r < 0)
    {
       printf("bind failed\n");
       close(server_fd);
       return -1;
    }
    printf("bind successful\n");
    // 3. Listening for the client connection request
    return_r = listen(server_fd, 5);
    if(return_r < 0)
    {
	printf("Listen failed");
	close(server_fd);
	return -1;
    }
    printf("Echo server listening on port %d...\n", PORT);

    addr_len = sizeof(client_addr);

    // 4. Accepting a client request and creating a session socket for accepted client
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if(client_fd < 0)
    {
       printf("accept failed\n");
       close(server_fd);
       return -1;
    }
    printf("Client connected\n");

    // 5. Echo loop
    while (1)
    {
        // Accept data from client
        int bytes = recv(client_fd, buffer, BUF_SIZE, 0);
        if (bytes <= 0)
        {
            break;
        }
        buffer[bytes] = '\0';
        printf("Received: %s\n", buffer);

        // 6. Echo back
        send(client_fd, buffer, bytes, 0);
    }

    // 7. Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
