#include <stdio.h>
#include <string.h> /* memset strerror */
#include <unistd.h>
#include <arpa/inet.h> /* struct sockaddr_in */
#include <sys/select.h> /* select() */
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

int main()
{
	int sockfd;
	int port = 6666;
	struct sockaddr_in addr;
	char buf[BUFSIZ];
	fd_set readset;
	int result;
	int N = 2;
	int myfds[N], maxfd, i;
	struct timeval tv;

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
	myfds[0] = sockfd;	
	myfds[1] = 0; /* stdin file descriptor */
	tv.tv_sec  = 2;
	tv.tv_usec = 0;
	do {
		maxfd =  0;
		FD_ZERO(&readset);
		
		for (i = 0; i < N; i++) {
			FD_SET(myfds[i], &readset);
			maxfd = myfds[i] > maxfd? myfds[i]: maxfd;
		}
		result = select(maxfd + 1, &readset, NULL, NULL, &tv);	
	} while (result == -1 && errno == EINTR);

	if (result > 0) {
		printf("select() = %d\n", result);
		for (i = 0; i < N; i++) {
			if (FD_ISSET(myfds[i], &readset)) {
				memset(buf, 0, BUFSIZ);
				if ((result = recv(myfds[i], buf, BUFSIZ, 0)) == -1) {
					fprintf(stderr, "recv error: [Errno %d] %s\n", errno, strerror(errno));
					return -1;
				} else if (result == 0) {/* This means the other side closed the socket*/
					close(sockfd); 
				} else
					printf("receive message from server:%s\n", buf);
				FD_CLR(myfds[i], &readset);
			}
		}
	}
	close(sockfd);
}
