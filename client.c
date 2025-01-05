#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_ADDRESS "::1"
#define PORT 9000
#define BUFFER_SIZE 1024

int main() {
    int client_sd = socket(PF_INET6, SOCK_STREAM, 0);
    if (client_sd == -1) {
        perror("failure: socket()");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in6 server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = PF_INET6;
    inet_pton(PF_INET6, SERVER_ADDRESS, &server_addr.sin6_addr);
    server_addr.sin6_port = htons(PORT);

    if (connect(client_sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("failure: connect()");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    while (1) {
        printf("Enter message: ");
    	char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);

		// TODO: sendでエラー起きたらどうするべき？
        send(client_sd, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_sd, buffer, BUFFER_SIZE - 1, 0);
		// TODO: recvの返り値0と1で処理変える？エラーの時は-1で、0の時は↓
		// For TCP sockets, the return value 0 means the peer has closed its half side of the connection.
        if (bytes_received <= 0) {
            perror("failure: recv()");
            break;
        }

        printf("From Server: %s", buffer);
    }

    close(client_sd);
	return EXIT_SUCCESS;
}
