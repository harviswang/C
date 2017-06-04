#include <stdio.h>
#include <sys/utsname.h>

int main(int argc, char *argv[])
{
	struct utsname name = { };
	int err = uname(&name);
	if (err) {
		printf("unknown sysname/nodename/release/version/machine\n");
	} else {
		printf("%s\n", name.sysname);
		printf("%s\n", name.nodename);
		printf("%s\n", name.release);
		printf("%s\n", name.version);
		printf("%s\n", name.machine);
	}

	return 0;
}
