#include <stdio.h> /* strerror fprintf */
#include <unistd.h> /* read */
#include <string.h> /* memset */
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 32

int main(int argc, char *argv[])
{
	int sock_fd, n;
	char recv_line[MAXLINE + 1];
	struct sockaddr_in serv_addr;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
		return -1;
	}

	if ( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "socket error:%s\n", strerror(errno));
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9999);
	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		fprintf(stderr, "inet_pton error:%s\n", strerror(errno));
		return -1;
	}

	if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "connect error:%s\n", strerror(errno));
		return -1;
	}

	while ( (n = read(sock_fd, recv_line, MAXLINE)) > 0) {
		recv_line[n] = 0;
		if (fputs(recv_line, stdout) == EOF) {
			fprintf(stderr, "fputs error:%s\n", strerror(errno));
			return -1;
		}
	}

	if (n < 0) {
		fprintf(stderr, "read error:%s\n", strerror(errno));
		return -1;
	}

	return 0;
}

