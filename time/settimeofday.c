#include <stdio.h>
#include <sys/time.h>

#if 0
struct timeval {
	time_t      tv_sec;   /* seconds */
	suseconds_t tv_usec;  /* microseconds */
};

struct timezone {
	int tz_minuteswest;  /* minutes west of Greenwich */
	int tz_dsttime;      /* type of DST(夏令时) correction */
};
#endif

int test_gettimeofday(void)
{
	struct timeval tv = {0, 0};
	struct timezone tz = {0, 0};
	int err = -1;
	
	err = gettimeofday(&tv, &tz); /* &tv can't be NULL */
	if (err) {
		perror("gettimeofday() failed");
	}
	
	/* timeval info */
	printf("tv.tv_sec = %ld\n", tv.tv_sec);
	printf("tv.tv_usec = %ld\n", tv.tv_usec);
	
	/* timezone info */
	printf("tz.tz_minuteswest = %d\n", tz.tz_minuteswest);
	printf("tz.tz_dsttime = %d\n", tz.tz_dsttime);
	
	return err;
}

int test_settimeofday(void)
{
	struct timeval tv = {1459495915, 181688};
	struct timezone tz = {-480, 0};
	int err = -1;
	
	err = settimeofday(&tv, &tz); /* &tv can't be NULL */
	if (err) {
		perror("settimeofday() failed");
	}
	
	return err;
}

int main(int argc, char *argv[])
{
	int err;
	
	err = test_settimeofday();
	printf("err = %d\n", err);
	
	err = test_gettimeofday();
	printf("err = %d\n", err);

	return 0;
}
