#include "hash.h"
#include <stdio.h>

#define dprintk(args...)  printf(args) 
static void crush_uniform_bucket_choose_test();

int main()
{
	__u32 keys[]  = {0x00, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0xFFFFFFFF};
	__u32 hash_v;
	int i;

	crush_uniform_bucket_choose_test();

	for (i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
		hash_v = crush_hash32(CRUSH_HASH_RJENKINS1, keys[i]);
		printf("crush_hash32(0x%x) = 0x%x\n", keys[i], hash_v);
		hash_v = crush_hash32_2(CRUSH_HASH_RJENKINS1, keys[i], 0);
		printf("crush_hash32_2(0x%x, 0) = 0x%x\n", keys[i], hash_v);
		hash_v = crush_hash32_3(CRUSH_HASH_RJENKINS1, keys[i], 0, 0);
		printf("crush_hash32_3(0x%x, 0, 0) = 0x%x\n", keys[i], hash_v);
		hash_v = crush_hash32_4(CRUSH_HASH_RJENKINS1, keys[i], 0, 0, 0);
		printf("crush_hash32_4(0x%x, 0, 0, 0) = 0x%x\n", keys[i], hash_v);
		hash_v = crush_hash32_5(CRUSH_HASH_RJENKINS1, keys[i], 0, 0, 0, 0);
		printf("crush_hash32_5(0x%x, 0, 0, 0, 0) = 0x%x\n", keys[i], hash_v);
	}

	return 0;
}

struct crush_work_bucket {
        __u32 perm_x; /* @x for which *perm is defined */
        __u32 perm_n; /* num elements of *perm that are permuted/defined */
        __u32 *perm;  /* Permutation of the bucket's items */
};

struct crush_work {
        struct crush_work_bucket **work; /* Per-bucket working store */
};

struct crush_bucket {
        __s32 id;        /*!< bucket identifier, < 0 and unique within a crush_map */
        __u16 type;      /*!< > 0 bucket type, defined by the caller */
        __u8 alg;        /*!< the item selection ::crush_algorithm */
        /*! @cond INTERNAL */
        __u8 hash;       /* which hash function to use, CRUSH_HASH_* */
        /*! @endcond */
        __u32 weight;    /*!< 16.16 fixed point cumulated children weight */
        __u32 size;      /*!< size of the __items__ array */
        __s32 *items;    /*!< array of children: < 0 are buckets, >= 0 items */
};

/*
 * Choose based on a random permutation of the bucket.
 *
 * We used to use some prime number arithmetic to do this, but it
 * wasn't very random, and had some other bad behaviors.  Instead, we
 * calculate an actual random permutation of the bucket members.
 * Since this is expensive, we optimize for the r=0 case, which
 * captures the vast majority of calls.
 */
static int bucket_perm_choose(const struct crush_bucket *bucket,
                              struct crush_work_bucket *work,
                              int x, int r)
{
        unsigned int pr = r % bucket->size;
        unsigned int i, s;

        /* start a new permutation if @x has changed */
        if (work->perm_x != (__u32)x || work->perm_n == 0) {
                dprintk("bucket %d new x=%d\n", bucket->id, x);
                work->perm_x = x;

                /* optimize common r=0 case */
                if (pr == 0) {
                        s = crush_hash32_3(bucket->hash, x, bucket->id, 0) %
                                bucket->size;
                        work->perm[0] = s;
                        work->perm_n = 0xffff;   /* magic value, see below */
                        goto out;
                }

                for (i = 0; i < bucket->size; i++)
                        work->perm[i] = i;
                work->perm_n = 0;
        } else if (work->perm_n == 0xffff) {
                /* clean up after the r=0 case above */
                for (i = 1; i < bucket->size; i++)
                        work->perm[i] = i;
                work->perm[work->perm[0]] = 0;
                work->perm_n = 1;
        }

        /* calculate permutation up to pr */
        for (i = 0; i < work->perm_n; i++)
                dprintk(" perm_choose have %d: %d\n", i, work->perm[i]);
        while (work->perm_n <= pr) {
                unsigned int p = work->perm_n;
                /* no point in swapping the final entry */
                if (p < bucket->size - 1) {
                        i = crush_hash32_3(bucket->hash, x, bucket->id, p) %
                                (bucket->size - p);
                        if (i) {
                                unsigned int t = work->perm[p + i];
                                work->perm[p + i] = work->perm[p];
                                work->perm[p] = t;
                        }
                        dprintk(" perm_choose swap %d with %d\n", p, p+i);
                }
                work->perm_n++;
        }
        for (i = 0; i < bucket->size; i++)
                dprintk(" perm_choose  %d: %d\n", i, work->perm[i]);

        s = work->perm[pr];
out:
        dprintk(" perm_choose %d sz=%d x=%d r=%d (%d) s=%d\n", bucket->id,
                bucket->size, x, r, pr, s);
        return bucket->items[s];
}

static void crush_uniform_bucket_choose_test()
{
	struct crush_bucket bucket;
	struct crush_work_bucket work;
	__u32 buf[32];
	__u32 items[32];
	int i;

	int x = 0x00000011;
	int r = 3;
	int ret;

	bucket.size = sizeof(buf)/sizeof(buf[0]);
	bucket.items = (void*)items;
	work.perm_x = 0;
	work.perm_n = 0;
	work.perm = (void*)buf;
	for (i = 0; i < bucket.size; i++) {
		buf[i] = i;
		items[i] = i;
	}

	ret = bucket_perm_choose(&bucket, &work, x, r);
	printf("ret %d\n", ret);
}
