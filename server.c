#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080

// bind(int socket, const struct sockaddr *address, socklen_t address_len);

void error(char * message) {
	printf("\x1b[31m%s\n", message);
	exit(1);
}

int main(int argc, char **argv) {

	// PF_INET -> ipv4
	// PF_INET6 -> ipv6
	// SOCK_STREAM -> tcp
	// SOCK_DGRAM -> udp
	int socket_num = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_num == -1) {
		error("create socket failed");
	}

	// socket name
	struct sockaddr_in socket_name;
	socket_name.sin_family = PF_INET;
	// socket_name.sin_port = (in_port_t)htons(5000);
	socket_name.sin_port = htons(SERVER_PORT);
	socket_name.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	int bind_num = bind(socket_num, (struct sockaddr *)&socket_name, sizeof(socket_name));
	if (bind_num == -1) {
		error("bind failed");
	}

	// listen(socket_discriptor, max_len_of_pending_queue);
	listen(socket_num, 1);
	printf("接続待機中\n");

	while (1) {
		struct sockaddr_storage client_addr;
		unsigned int address_size = sizeof(client_addr);

		int client_socket = accept(socket_num, (struct sockaddr *)&client_addr, (socklen_t *)&address_size);
		if (client_socket == -1) {
			error("accept failed");
		}
		printf("connect success\n");


		// クライアントからの受信
		// recv(int socket, void *buffer, size_t length, int flags);
		char buff[1024];
		memset(buff, 0, sizeof(buff));
		int recv_byte = recv(client_socket, buff, sizeof(buff), 0);
		// TODO: recv_byte == 0だったらどうする？
		if (recv_byte == -1) {
			error("recieve failed");
		}
		// "quit"または"exit"が送られてきたら接続終了する
		if (strncmp(buff, "quit\n", recv_byte) == 0 || strncmp(buff, "exit\n", recv_byte) == 0) {
			// TODO
			// 接続終了処理を書く
			printf("TODO: 接続終了処理をかく\n");
		}
		printf("received from client: %s\n", buff);

		int send_byte = send(client_socket, buff, sizeof(buff), 0);
		if (send_byte == -1) {
			error("send failed");
		}
		close(client_socket);
	}

	printf("socket_num: %d\n", socket_num);
}

