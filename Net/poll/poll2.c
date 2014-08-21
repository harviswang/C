/*
 * First make two fifo pipe such as "myfifo" and "myfifo2" 
 * Run ./poll2
 * cat > myfifo << EOF
 * cat myfifo
 */
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
/* #include <sys/stropts.h> */

#define NPOLL 2 /* number of file descriptors to poll */
int main()
{
		struct pollfd pollfds[NPOLL];
		char buf[1024];
		int count, i;
		int times = 1;

		if ((pollfds[0].fd = open("./myfifo", O_RDWR | O_NONBLOCK)) < 0 ) {
				perror("open failed for /dev/stdin");
				exit(1);
		}

		if ((pollfds[1].fd = open("./myfifo2", O_RDWR | O_NONBLOCK)) < 0) {
				perror("open failed for /dev/tty1");
				exit(2);
		}

		/* set events to poll for incoming data */
		pollfds[0].events = POLLIN;  
		pollfds[1].events = POLLIN;  

		while (1) {
				/* poll and -1 timeout(infinite) */
				if (poll(pollfds, NPOLL, -1) < 0) {
						perror("poll failed");
						exit(3);
				}

				for (i = 0; i < NPOLL; i++) {
						switch (pollfds[i].revents) {
								default:
										fprintf(stderr, "event error");
										exit(4);
								case 0: 
										break;
								case POLLIN:
										printf("This is [%d] times POLLIN\n", times++);
										/* Echo incoming data on "other" stream */
										while ((count = read(pollfds[i].fd, buf, 1024)) > 0) 
												/*
												 * write loses data if flow control 
												 * prevents the transmit at this time
												 */
												if (write(pollfds[(i + 1) % NPOLL].fd, buf, count) != count)
														fprintf(stderr, "writer lost data");

										break;
						}
						pollfds[i].revents = 0;
				}
		}
}

