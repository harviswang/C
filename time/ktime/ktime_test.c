#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static ktime_t ktime_sub(const ktime_t lhs, const ktime_t rhs);
static void ktime_sub_test();
static ktime_t ktime_add(const ktime_t add1, const ktime_t add2);
static void ktime_add_test();
int main(int argc, char *argv[])
{
	ktime_sub_test();
	ktime_add_test();
    return 0;
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
