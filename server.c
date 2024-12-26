#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT "8080"

void error(char * message) {
	printf("\x1b[31m%s\n", message);
	exit(1);
}

int main(int argc, char **argv) {

	struct addrinfo info;
	struct addrinfo *addr;
	struct sockaddr_storage from;
	char host[NI_MAXHOST];

	memset(&info, 0, sizeof(info));
	info.ai_family = PF_INET;
	info.ai_socktype = SOCK_STREAM;
	info.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, SERVER_PORT, &info, &addr) != 0) {
		error("getaddrinfo failed");
	}
	
	int socket_num = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (socket_num == -1) {
		error("create socket failed");
	}
	int bind_num = bind(socket_num, addr->ai_addr, addr->ai_addrlen);
	if (bind_num == -1) {
		error("bind fialed");
	}

	listen(socket_num, 5);
	printf("接続待機中・・・\n");
	socklen_t fromlen = sizeof(from);

	while (1) {

		int client_socket = accept(socket_num, (struct sockaddr *)&from, &fromlen);
		if (client_socket == -1) {
			error("accept failed");
		}

		getnameinfo((struct sockaddr *)&from, fromlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
		printf("accept from: %s\n", host);
		close(socket_num);
		char buff[1024];
		memset(buff, 0, sizeof(buff));
		int recv_num = recv(client_socket, buff, sizeof(buff), 0);
		if (recv_num  == -1) {
			error("recv failed");
		}
		printf("received from client: %s\n", buff);
		int send_num = send(client_socket, buff, sizeof(buff), 0);
		if (send_num == -1) {
			error("send failed");
		}
		// close(client_socket);
	}
	freeaddrinfo(addr);
}

