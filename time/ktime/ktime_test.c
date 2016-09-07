#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /* struct timespec; */
/*
 struct timespec {
	__kernel_time_t tv_sec;
	long            tv_nsec;
 };
 struct timeval {
	__kernel_time_t      tv_sec;
	__kernel_suseconds_t tv_usec;
 };
*/

typedef long long s64;
typedef int s32;
typedef unsigned int u32;
#define NSEC_PER_SEC	1000000000L
union ktime {
	s64	tv64;
	struct {
		s32	nsec, sec;
	} tv;
};
typedef union ktime ktime_t;		/* Kill this */

static ktime_t ktime_set(const u32 sec, const u32 nsec);
static ktime_t ktime_sub(const ktime_t lhs, const ktime_t rhs);
static ktime_t ktime_add(const ktime_t add1, const ktime_t add2);
static struct timespec ktime_to_timespec(ktime_t ktime);
static ktime_t timespec_to_ktime(struct timespec ts);
static struct timeval ktime_to_timeval(ktime_t ktime);
static void ktime_sub_test();
static void ktime_add_test();
static void ktime_to_timespec_test();
static void timespec_to_ktime_test();
static void ktime_to_timeval_test();
int main(int argc, char *argv[])
{
	ktime_sub_test();
	ktime_add_test();
	ktime_to_timespec_test();
	timespec_to_ktime_test();
	ktime_to_timeval_test();
    return 0;
}

static ktime_t ktime_set(const u32 sec, const u32 nsec)
{
	return (ktime_t){.tv.sec = sec, .tv.nsec = nsec};
}

/*
 * lhs; left hand side
 * rhs: right hand side
 * in a sub expression '1-4', 1 is lhs, 2 is rhs
 */
static ktime_t ktime_sub(const ktime_t lhs, const ktime_t rhs)
{
	ktime_t res;
	
	res.tv64 = lhs.tv64 - rhs.tv64;
	/*
	 * 如果res.tv.nsec为负数, 说明res.tv.sec已经产生了借位
	 */
	if (res.tv.nsec < 0) {
		res.tv.nsec += NSEC_PER_SEC;
	}
	return res;
}

static void ktime_sub_test()
{
	ktime_t today;
	ktime_t yestoday;

	ktime_t res;

	today.tv64    = 0x32b9ac000LL;
	yestoday.tv64 = 0x23b9ac000LL;
/*
	res.tv64 = today.tv64 - yestoday.tv64;
	printf("NSEC_PER_SEC = %d\n", NSEC_PER_SEC);
	printf("res.tv.nsec = %d\n", res.tv.nsec);
	printf("res.tv.sec = %d\n", res.tv.sec);
	if (res.tv.nsec < 0) {
		res.tv.nsec += NSEC_PER_SEC;
		printf("line: %d\n", __LINE__);
	}
*/
	res = ktime_sub(today, yestoday);
	
	printf("NSEC_PER_SEC = 0x%lx\n", NSEC_PER_SEC);
	printf("NSEC_PER_SEC = %ld\n", NSEC_PER_SEC);
	printf(" res.tv.nsec = %d\n", res.tv.nsec);
	printf("  res.tv.sec = %d\n", res.tv.sec);
	printf("0x1000000 = %d\n", 0x10000000);
}

static ktime_t ktime_add(const ktime_t add1, const ktime_t add2)
{
	ktime_t res;
	
	res.tv64 = add1.tv64 + add2.tv64;
	/*
	 * performace trick: the (u32) -NSEC_PER_SEC gives 0x00000000Cxxxxxxx
	 * so we subtract NSEC_PER_SEC and add 1 to the upper 32 bit
	 * 
	 * it's equivalent to:
	 *   tv.nsec -= NSEC_PER_SEC;
	 *   tv.sec ++;
	 */
	if (res.tv.nsec >= NSEC_PER_SEC) {
		res.tv64 += (u32)-NSEC_PER_SEC;
	}
	return res;
}

static void ktime_add_test()
{
	ktime_t yestoday;
	ktime_t today;
	ktime_t res;
	
	yestoday.tv64 = 0x13b9ac900;
	today.tv64    = 0x000000100;
	res = ktime_add(yestoday, today);
	printf("(u32)-NSEC_PER_SEC = 0x%x\n", (u32)-NSEC_PER_SEC);
	printf("NSEC_PER_SEC = %ld\n", NSEC_PER_SEC);
	printf(" res.tv.nsec = %d\n", res.tv.nsec);
	printf("  res.tv.sec = %d\n", res.tv.sec);
}

static struct timespec ktime_to_timespec(ktime_t ktime)
{	
	return (struct timespec){
		.tv_sec  = (long)ktime.tv.sec, 
		.tv_nsec = (long)ktime.tv.nsec};
}

static void ktime_to_timespec_test()
{
	ktime_t yestoday;
	yestoday = ktime_set(1, 1000);
	struct timespec sometime = ktime_to_timespec(yestoday);
	
	printf(" tv_sec = %ld \n", sometime.tv_sec);
	printf("tv_nsec = %ld\n", sometime.tv_nsec);
}

static ktime_t timespec_to_ktime(struct timespec ts)
{
	return (ktime_t){.tv.sec = ts.tv_sec, .tv.nsec = ts.tv_nsec};
}

static void timespec_to_ktime_test()
{
	struct timespec now = {123, 456};
	ktime_t kt = timespec_to_ktime(now);
	printf(" tv.sec = %d\n", kt.tv.sec);
	printf("tv.nsec = %d\n", kt.tv.nsec);
}

static struct timeval ktime_to_timeval(ktime_t ktime)
{
	return (struct timeval){
		.tv_sec  = (long)ktime.tv.sec,
		.tv_usec = (long)ktime.tv.nsec / 1000 };
}

static void ktime_to_timeval_test()
{
	ktime_t before = ktime_set(12, 9999);
	struct timeval tv = ktime_to_timeval(before);
	printf(" tv_sec = %ld\n", tv.tv_sec);
	printf("tv_usec = %ld\n", tv.tv_usec);
}
