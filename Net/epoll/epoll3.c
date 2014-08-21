#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define NPOLL 2 /* number of file descriptors to poll */
#define EPOLL_MAX 5
int main()
{
		struct epoll_event ev, ev2, events;
		int epollfd;
		char buf[1024];
		int count, i;
		int times = 1;
		int fd, fd2, fds[2];
		int res;

		if ((fd = open("./fifo_epoll", O_RDWR | O_NONBLOCK)) < 0 ) {
				perror("open failed for ./fifo_epoll");
				exit(1);
		}

		if ((fd2 = open("./fifo_epoll2", O_RDWR | O_NONBLOCK)) < 0) {
				perror("open failed for ./fifo_epoll2");
				exit(2);
		}

		fds[0] = fd;
		fds[1] = fd2;

		/* set events to poll for incoming data */
		ev.events = EPOLLIN;
		ev.data.fd = fd;

		ev2.events = EPOLLIN;  
		ev2.data.fd = fd;

		epollfd = epoll_create(EPOLL_MAX);
		if (epollfd == -1) {
				perror("epoll_create");
				abort();
		}
		res = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
		if (res == -1) {
				perror("epoll_ctl");
				abort();
		}
		res = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd2, &ev2);
		if (res == -1) {
				perror("epoll_ctl");
				abort();
		}
		while (1) {
				res = epoll_wait(epollfd, &events, 5, -1);
				if (res == -1) {
						perror("epoll_wait");
						abort();
				} else if (res == 0) {
						perror("epoll_wait timeout");
						continue;
				} else {
						for (i = 0; i < NPOLL; i++) {
								/* printf("This is [%d] times POLLIN\n", times++); */
								/* Echo incoming data on "other" stream */
								while ((count = read(fds[i], buf, 1024)) > 0) { 
										/*
										 * write loses data if flow control 
										 * prevents the transmit at this time
										 */
										if (write(fds[(i + 1) % NPOLL], buf, count) != count)
												fprintf(stderr, "writer lost data");										
								}

						}
				}
		}
}


