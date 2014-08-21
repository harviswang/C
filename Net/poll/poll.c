#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_FILES 32

int main(int argc, char **argv)
{
	int i;
	struct pollfd fds[MAX_FILES];
	char c;

	/* Use only fifos and not normal files! */
	for (i = 1; i < argc; i++) {
		if ((fds[i].fd = open(argv[i], O_RDONLY)) == -1) {
			perror(argv[i]);
			exit(0);
		} else 
			fds[i].events = POLLIN;
	}		
	/* Add stdin to make it more interesting */
	fds[0].fd = 0; /* 0 is stdin */
	fds[0].events = POLLIN;

	while (1) {
		/* Wait a second for data to read from files */
		if (poll(fds, (unsigned long)argc, 1000) < 0)
			perror("poll");

		for (i = 0; i < argc; i++) {
			if (fds[i].revents) {
				if (read(fds[i].fd, &c, 1) != 1)
					perror("reading");
				else
					printf("read '%c' from %s\n", c , i == 0? "stdin" : argv[i]);

				fds[i].revents = 0;
			}
		}
	}
}
/* Reference: http://www.experts-exchange.com/Programming/System/Q_10008555.html */
