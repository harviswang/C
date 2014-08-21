#include <stdio.h>
#include <string.h> /* memset strerror */
#include <unistd.h>
#include <arpa/inet.h> /* struct sockaddr_in */
#include <sys/select.h> /* select() */
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>

int main()
{
	int sockfd;
	int port = 6666;
	struct sockaddr_in addr;
	char buf[BUFSIZ];
	fd_set readset;
	int result;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "socket error: [Errno %d] %s\n", errno, strerror(errno));
		return -1;
	}
	
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		fprintf(stderr, "connect error: [Errno %d] %s\n", errno, strerror(errno));
		return -1;	
	}
	
	printf("send message to server, please input:");
	fgets(buf, BUFSIZ, stdin);
	if (send(sockfd, buf, strlen(buf), 0) == -1) {
		fprintf(stderr, "send error: [Errno %d] %s\n", errno, strerror(errno));
		return -1;
	}
	
	/* Call select()  */
	do {
		FD_ZERO(&readset);
		FD_SET(sockfd, &readset);
		result = select(sockfd + 1, &readset, NULL, NULL, NULL);	
	} while (result == -1 && errno == EINTR);

	if (result > 0) {
		if (FD_ISSET(sockfd, &readset)) {
			memset(buf, 0, BUFSIZ);
			if ((result = recv(sockfd, buf, BUFSIZ, 0)) == -1) {
				fprintf(stderr, "recv error: [Errno %d] %s\n", errno, strerror(errno));
				return -1;
			} else if (result == 0) {/* This means the other side closed the socket*/
				close(sockfd); 
			} 
		}
	}
	printf("receive message from server:%s\n", buf);
	close(sockfd);
}
