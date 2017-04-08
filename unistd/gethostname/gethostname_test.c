#include <stdio.h>
#include <unistd.h> /* gethostname */
#include <limits.h>

#ifndef HOST_NAME_MAX
#ifdef MAXHOSTNAMELEN
#define HOST_NAME_MAX MAXHOSTNAMELEN
#else
#define HOST_NAME_MAX sysconf(_SC_HOST_NAME_MAX)
#endif
#endif

int main()
{
	char host[HOST_NAME_MAX + 1];

	int err = gethostname(host, HOST_NAME_MAX);
	if (err) {
		printf("gethostname failed\n");
	}

	printf("hostname %s\n", host);
	return 0;
}
