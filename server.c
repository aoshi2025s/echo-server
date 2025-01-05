#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9000
#define BUFFER_SIZE 1024

int main() {
    int server_sd = socket(PF_INET6, SOCK_STREAM, 0);
    if (server_sd == -1) {
        perror("failure: socket()");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in6 server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = PF_INET6;
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons(PORT);

    if (bind(server_sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("failure: bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sd, 5) == -1) {
        perror("failure: listen()");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
	struct sockaddr_in6 client_addr;
	socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == -1) {
            perror("failure: accept()");
            continue;
        }

        printf("Connection established\n");

        while (1) {
    	    char buffer[BUFFER_SIZE];
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
	    // TODO: -1の時と0の時でどうするか考える
            if (bytes_received <= 0) {
                perror("failure: recv()");
                break;
            }

            printf("From Client: %s", buffer);

	    // TODO: sendのエラーハンドリングどうするか
            send(client_fd, buffer, bytes_received, 0);
        }
    }

    close(server_sd);
    return EXIT_SUCCESS;
}
