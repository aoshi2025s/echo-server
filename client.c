#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#define SERVER_ADDR_IPV4 "127.0.0.1"
#define SERVER_ADDR_IPV6 "::1"
#define SERVER_PORT "8080"

void error(char *message) {
	printf("%s\n", message);
	exit(1);
}

int main(int argc, char **argv) {

	char * send_message;
	if (argc == 3) {
		send_message = argv[2];
	} else {
		error("Please input ./client [server_ip_address] [message]");
	}

	struct addrinfo info;
	struct addrinfo *addr = NULL;
	memset(&info, 0, sizeof(info));
	// PF_INET -> ipv4, PF_INET6 -> ipv6, PF_UNSPECでどちらもいける？
	// info.ai_family = PF_INET6;
	info.ai_family = PF_UNSPEC;
	info.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(argv[1], SERVER_PORT, &info, &addr) != 0) {
	   error("getaddrinfo failed");
	}	   


	int server_d = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (server_d == -1) {
		error("create socket failed");
	}

	int connect_num = connect(server_d, addr->ai_addr, addr->ai_addrlen);
	if (connect_num == -1) {
		error("connect failed");
	}


	send(server_d, send_message, strlen(send_message), 0);
	char buff[1024];
	recv(server_d, buff, 1024, 0);
	printf("received from server: %s\n", buff);
	close(server_d);

	freeaddrinfo(addr);
	return 0;
}

