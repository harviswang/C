#include <stdio.h> /* fprintf */
#include <time.h> /* time */
#include <string.h> /* memset */
#include <errno.h>
#include <unistd.h> /* close */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define MAXLINE 32

int main(int argc, char *argv[])
{
	int listen_fd, conn_fd;
	struct sockaddr_in serv_addr;
	char buff[MAXLINE];
	time_t ticks;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0) {
		fprintf(stderr, "socket failed:%s\n", strerror(errno));
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9999);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "bind failed:%s\n", strerror(errno));
		return -1;
	}

	if (listen(listen_fd, -1) < 0) {
		fprintf(stderr, "listen failed:%s\n", strerror(errno));
		return -1;
	}

	for (;;) {
		conn_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);
		if (conn_fd < 0) {
			fprintf(stderr, "accept failed:%s\n", strerror(errno));				return -1;
		}
		
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		if (write(conn_fd, buff, strlen(buff)) < 0) {
			fprintf(stderr, "write failed:%s\n", strerror(errno));
			return -1;
		}

		close(conn_fd);
	}
}
