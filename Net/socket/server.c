#include <stdio.h>  /* BUFSIZ(maybe 8196), stderr */
#include <string.h> /* memset strerror */
#include <errno.h> /*  */
#include <unistd.h> /* close */
#include <arpa/inet.h> /* htonl, htons, ntohl, ntohs */
#include <sys/types.h> 
#include <sys/socket.h> /* socket, bind, listen, accept */

int main(int argc, char **argv)
{
	int sockfd, new_sockfd;
	struct sockaddr_in addr;
	int port = 6666;
	int backlog = 5;
	char buf[BUFSIZ];
	int res;
	socklen_t len;
	int on = 1;
/*	fd_set readset; */

	sockfd = socket(AF_INET, SOCK_STREAM, 0);/* AF_INET for ip v4, AF_INET6 for ip v6 */
	if (sockfd == -1) {
		fprintf(stderr, "socket() error: [Errno %d] %s\n", errno, strerror(errno));
		return -1;
	}

	/* Set socket descriptor to be reuseable  */	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
		fprintf(stderr, "setsockopt() error: [Errno %d] %s\n", errno, strerror(errno));
		close(sockfd);
		return -1;
	}	
	
	/* Bind the socket */
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr, "bind error: [Errno %d] %s\n", errno, strerror(errno));
		return -1;
	}

	if (listen(sockfd, backlog) == -1) {
		fprintf(stderr, "listen error: [Errno %d] %s\n", errno, strerror(errno));
		return -1;
	}
	
	for (;;) {
		if ((new_sockfd = accept(sockfd, (struct sockaddr *)&addr, &len)) == -1) {
			fprintf(stderr, "accept error: [Errno %d] %s\n", errno, strerror(errno));
			continue;
		}
		if((res = read(new_sockfd, buf, BUFSIZ)) > 0) {				
			buf[res] = '\0';
/*			printf("read from client:%s\n", buf); */
/*
			memset(buf, 0, BUFSIZ);
			printf("send message to client:");
			fgets(buf, BUFSIZ, stdin);
*/	
			/* echo whatever get to client */
			write(new_sockfd, buf, strlen(buf));
		}
		close(new_sockfd);
	}
	close(sockfd);
}
