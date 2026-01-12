#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    // 1. Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("Socket failed");
        return 1;
    }

    // 2. Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 3. Connect to server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connect failed");
        return 1;
    }

    printf("Connected to server\n");

    // 4. Send & receive loop
    while (1)
    {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);

        send(sock_fd, buffer, strlen(buffer), 0);

        int bytes = recv(sock_fd, buffer, BUF_SIZE, 0);
        buffer[bytes] = '\0';

        printf("Echo from server: %s\n", buffer);
    }

    // 5. Close socket
    close(sock_fd);
    return 0;
}
