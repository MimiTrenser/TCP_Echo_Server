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
int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr;

    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    make_non_blocking(server_fd);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);

    printf("Server running with timeout...\n");

    while (1)
    {
        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        timeout.tv_sec = 5; // 5 seconds timeout
        timeout.tv_usec = 0;
        int activity = select(server_fd + 1, &readfds, NULL, NULL, &timeout);
        if (activity == 0)
        {
            printf("No client connection (timeout)\n");
            continue;
        }
        if (FD_ISSET(server_fd, &readfds))
        {
            client_fd = accept(server_fd, NULL, NULL);
            make_non_blocking(client_fd);
            printf("Client connected\n");
            FD_ZERO(&readfds);
            FD_SET(client_fd, &readfds);
            timeout.tv_sec = 5; // wait for client data
            timeout.tv_usec = 0;
            activity = select(client_fd + 1, &readfds, NULL, NULL, &timeout);
            if (activity == 0)
            {
                printf("Client timeout – no data\n");
                close(client_fd);
                continue;
            }
            if (FD_ISSET(client_fd, &readfds))
            {
                int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
                buffer[bytes] = '\0';
                printf("Received: %s\n", buffer);
                send(client_fd, "Hello from server", 18, 0);
                close(client_fd);
            }
        }
    }
}
