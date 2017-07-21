#include <stdio.h>
#include <assert.h>
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

struct regulator;
static void sizeof_func_test();
static void sizeof_incomplete_test();
static void sizeof_void_test();
static void sizeof_bitfield_test();
static void sizeof_undefined_struct_test();
static void sizeof_string_test();
static void sizeof_function_name_test();
static void sizeof_array_test();
static void sizeof_FILE_test();
static void sizeof_basic_type_test();
static void sizeof_union_test();
static void sizeof_empty_struct_test();
static void sizeof_define_test();
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
    sizeof_undefined_struct_test();
    sizeof_string_test();
    sizeof_function_name_test();
    sizeof_array_test();
    sizeof_FILE_test();
    sizeof_basic_type_test();
    sizeof_union_test();
    sizeof_empty_struct_test();
    sizeof_define_test();

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

struct regulator {
    int volatage;
    int amps;
};
static void sizeof_undefined_struct_test()
{
    struct regulator *reg;

    printf("%ld\n", sizeof(typeof(*reg)));
}

/*
 * sizeof("hello") == 6, 包含字符串结束标识符'\0'
 */
static void sizeof_string_test()
{
    printf("sizeof(\"hello\") = %ld\n", sizeof("hello"));
}

/*
 * sizeof(function_name) == 1 这怎么解释呢? TODO
 */
static void sizeof_function_name_test()
{
    printf("sizeof(sizeof_undefined_struct_test) = %ld\n", sizeof(sizeof_undefined_struct_test));
}

/*
 * 字符串初始化字符数组, 会自动添加字符串结束标识符'\0'
 * sizeof是编译阶段起作用, 因此可以用作数组大小的声明.
 */
static void sizeof_array_test()
{
    char str[] = "good";
    printf("sizeof(%s) = %ld\n", str, sizeof(str));
    assert(sizeof(str) == sizeof("good"));

    do {
        char string[sizeof(FILE)];
        printf("sizeof(string) = %ld\n", sizeof(string));
    } while (0);

    {
        int * oof[3][4];
        printf("sizeof(oof) = %ld\n", sizeof(oof));
    }

    {
        int (* uof[3])[4];
        printf("sizeof(uof) = %ld\n", sizeof(uof));
    }

    {
        int * risks [10]; /* [] upper than * */
        printf("sizeof(risks) = %ld\n", sizeof(risks));
    }

    {
        int (* rusks) [10]; /* [] equal to () */
        printf("sizeof(rusks) = %ld\n", sizeof(rusks));
    }
}

/*
 * 对于结构体取sizeof, 可以用结构体指针变量解引用来计算
 * 这样做有个好处, 就是sizeof与类型名无直接关系
 */
static void sizeof_FILE_test()
{
    FILE *fp;

    printf("sizeof(*fp) = %ld\n", sizeof(*fp));
    printf("sizeof(FILE) = %ld\n", sizeof(FILE));
}

/*
 * 基本数据类型大小打印
 */
static void sizeof_basic_type_test()
{
#define SIZEOF_DUMP(type) (printf("sizeof(" #type ") = %ld\n", sizeof(type)))
    SIZEOF_DUMP(char);
    SIZEOF_DUMP(short int);
    SIZEOF_DUMP(int);
    SIZEOF_DUMP(long int);
    SIZEOF_DUMP(long unsigned int);
    SIZEOF_DUMP(long long);
    SIZEOF_DUMP(float);
    SIZEOF_DUMP(double);
    SIZEOF_DUMP(long double);
    SIZEOF_DUMP(unsigned);
    SIZEOF_DUMP(__signed);
#undef SIZEOF_DUMP
}

static void sizeof_union_test()
{
    struct data {
        union {
            union {
                int value[128];
                int *value_ptr;
            } integer;
            union {
                long long value[64];
                long long *value_ptr;
            } integer64;
            union {
                unsigned char value[512];
                unsigned char *value_ptr;
            } bytes;
        } value;
    };

	enum power_type {
		POWER_ON_LCD = 0x1,
		POWER_ON_BL = 0x2,
		POWER_OFF_LCD = 0x4,
		POWER_OFF_BL = 0x08
	};

    struct data dd;
    printf("sizeof(dd.value.integer.value[0]) = %ld\n", sizeof(dd.value.integer.value[0]));
	
	enum power_type pt;
	printf("sizoef(pt) = %ld\n", sizeof(pt));
}

/*
 * empty struct does not consume any memory space
 */
static void sizeof_empty_struct_test()
{
	typedef struct { } arch_spinlock_t;
	int a;
	arch_spinlock_t aoe;
	int b;
    
    printf("typedef struct { } arch_spinlock_t;\n");
    printf("int a;\n");
    printf("arch_spinlock_t aoe;\n");
    printf("int b;\n");
	printf("sizeof(arch_spinlock_t) = %ld\n", sizeof(arch_spinlock_t));
	printf("sizeof(aoe) = %ld\n", sizeof(aoe));
	printf("&a = %p\n", &a);
	printf("&aoe = %p\n", &aoe);
	printf("&b = %p\n", &b);
}

/*
 * 
 */
static void sizeof_define_test()
{
#define INTEGER 99 /* 4byts */
    printf("#define INTEGER 99\n");
    printf("sizeof(INTEGER) = %ld\n", sizeof(INTEGER));
#define UNSIGNED_INTEGER 99U /* 4byts */
    printf("#define UNSIGNED_INTEGER 99U\n");
    printf("sizeof(UNSIGNED_INTEGER) = %ld\n", sizeof(UNSIGNED_INTEGER));
#define LONG 99L /* 8bytes */
    printf("#define LONG 99L\n");
    printf("sizeof(LONG) = %ld\n", sizeof(LONG));
#define UNSIGNED_LONG 99UL /* 8bytes */
    printf("#define UNSIGNED_LONG 99UL\n");
    printf("sizeof(UNSIGNED_LONG) = %ld\n", sizeof(UNSIGNED_LONG));
}
