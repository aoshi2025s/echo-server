#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080

void error(char *message) {
	printf("%s\n", message);
	exit(1);
}

int main(int argc, char **argv) {
	// socket作成
	int server_d = socket(PF_INET, SOCK_STREAM, 0);
	if (server_d == -1) {
		error("create socket failed");
	}

	struct sockaddr_in server_name;
	// 初期化
	memset(&server_name, 0, sizeof(server_name));

	server_name.sin_family = PF_INET,
	server_name.sin_port = htons(SERVER_PORT);
	server_name.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	connect(server_d, (struct sockaddr *)&server_name, sizeof(server_name));

	send(server_d, argv[1], strlen(argv[1]), 0);
	
	char buff[1024];
	recv(server_d, buff, 1024, 0);
	printf("received from server: %s\n", buff);

	close(server_d);
}


