#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>

#define EPOLL_MAX_CONN     2
#define EPOLL_RUN_TIMEOUT -1

int main(int argc, char *argv[])
{
		int epfd = epoll_create(EPOLL_MAX_CONN);
		int fd   = open("./fifo_epoll", O_RDWR | O_NONBLOCK);

		if (fd <  0) {
				perror("open error");
				abort();
		}
		if (epfd < 0) {
				perror("epoll_create error");
				abort();
		}

		const int BUFF_SIZE = 50;
		char buff[BUFF_SIZE];
		memset(buff, 0, BUFF_SIZE);

		struct epoll_event ev;
		struct epoll_event events;

		ev.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP; /* this is level-triggered mode */
		ev.data.fd = fd;

		int res = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
		if (res < 0) 
				printf("Error epoll_ctl: %i\n", errno);

		while (1) {
				int n = epoll_wait(epfd, &events, EPOLL_MAX_CONN, EPOLL_RUN_TIMEOUT);

				printf("Epoll Unblocked ...\n");
				if (n < 0) {
						perror("Epoll failed");
				} else if (n == 0) {
						printf("TIMEOUT\n");
				} else {
						int size = read(fd, buff, BUFF_SIZE);
						if (size < 0) {
								printf("Error Reading the device: %s\n", strerror(errno));
						} else if (size == 0) {
								printf("No input\n");
						} else { 
								buff[size] = '\0';
								printf("Input content: %s\n", buff);
						}
				}

				if (errno == EAGAIN)
						printf("ERRNO: EAGAIN\n");
		}
}
/* Reference: http://rico-studio.com/linux/epoll-2/ */
