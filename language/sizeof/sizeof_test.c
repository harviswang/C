#include <stdio.h>
/*
 * calng sizeof implementation : long long clang_Type_getSizeOf(CXType T) {}
 */

typedef unsigned char __u8;
typedef unsigned short __u16;
typedef unsigned long kernel_ulong_t;

struct sdio_device_id {
	__u8	class;			/* Standard interface or SDIO_ANY_ID */
	__u16	vendor;			/* Vendor or SDIO_ANY_ID */
	__u16	device;			/* Device ID or SDIO_ANY_ID */
	kernel_ulong_t driver_data;	/* Data private to the driver */
	/*	__attribute__((aligned(sizeof(kernel_ulong_t))));*/
};

static void sizeof_func_test();
static void sizeof_incomplete_test();
static void sizeof_void_test();
static void sizeof_bitfield_test();
int main(int argc, char **argv)
{
    struct sdio_device_id id;

    printf("sizeof(struct sdio_device_id) = %ld\n", sizeof(struct sdio_device_id));
    printf("&id = %p\n", &id);
    printf("&id.class = %p\n", &id.class);
    printf("&id.vendor = %p\n", &id.vendor);
    printf("&id.device = %p\n", &id.device);
    printf("&id.driver_data = %p\n", &id.driver_data);

    printf("sizeof(id) = %ld\n", sizeof(id));
    printf("sizeof(id.class) = %ld\n", sizeof(id.class));
    printf("sizeof(id.vendor) = %ld\n", sizeof(id.vendor));
    printf("sizeof(id.device) = %ld\n", sizeof(id.device));
    printf("sizeof(id.driver_data) = %ld\n", sizeof(id.driver_data));
    sizeof_func_test();
    sizeof_incomplete_test();
    sizeof_void_test();
    sizeof_bitfield_test();
    return 0;
}

static void sizeof_func_test()
{
	printf("sizeof(sizeof_func_test)=%ld\n", sizeof(sizeof_func_test));
}

static void sizeof_incomplete_test()
{
}

static void sizeof_void_test()
{
	printf("sizeof(void)=%ld\n", sizeof(void));
}

static void sizeof_bitfield_test()
{
	struct bitfield {
		short bitfield:9;
	};

	/*printf("sizeof(bitfield)=%ld\n", sizeof(b.bitfield));*/
	printf("sizeof(struct bitfield)=%ld\n", sizeof(struct bitfield));
}
