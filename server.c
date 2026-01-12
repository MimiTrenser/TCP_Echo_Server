#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[BUF_SIZE];

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket failed");
        return 1;
    }

    // 2. Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept from any IP
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 3. Listen
    listen(server_fd, 5);
    printf("Echo server listening on port %d...\n", PORT);

    addr_len = sizeof(client_addr);

    // 4. Accept client
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    printf("Client connected\n");

    // 5. Echo loop
    while (1)
    {
        int bytes = recv(client_fd, buffer, BUF_SIZE, 0);
        if (bytes <= 0)
            break;

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
