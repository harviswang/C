#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <event.h>


void onTime(int sock, short event, void *arg)
{
	struct timeval tv = {1, 0};
	printf("Hello world\n");

	event_add((struct event*)arg, &tv);
}

int main()
{
	struct event ev_time;
	struct timeval tv = {1, 0};

	event_init();

	evtimer_set(&ev_time, onTime, &ev_time);

	event_add(&ev_time, &tv);

	event_dispatch();

	return 0;
}

