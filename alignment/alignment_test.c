/*
 * A data structure or array is aligned in memory to the largest alignment
 * boundary required by any data type inside it. 
 *  __attribute__ ((__packed__))
 * __attribute__ ((__aligned__(8)))
 * __aligned__()
 * #pragma pack (1)
 * #pragma pack ()
 */
#include <stdio.h>

static void alginment_macro_test(void);
static void alginment_rule_test(void);
static void alginment_rule_basictype_test(void);
static void alginment_rule_struct_test(void);
static void alginment_rule_struct_test2(void);

int main(int argc, char **argv)
{
    alginment_macro_test();
    alginment_rule_test();
    alginment_rule_struct_test2();

	return 0;
}

/*
 * 测试对齐计算常用的宏
 * 对齐计算这里面用的是向上取整的策略
 * 使用了(typeof(6))7这种强制类型转换
 */
static void alginment_macro_test(void)
{
#define __ALIGN_KERNEL(x, a)        __ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
#define __ALIGN_KERNEL_MASK(x, mask)    (((x) + (mask)) & ~(mask))
#define ALIGN(x, a)     __ALIGN_KERNEL((x), (a))
    printf("__ALIGN_KERNEL_MASK(6, 7)=%d\n", __ALIGN_KERNEL_MASK(6, 7));
    printf("__ALIGN_KERNEL(6, 7)=%d\n", __ALIGN_KERNEL(6, 7));
    printf("(typeof(6))7=%d\n", (typeof(6))7);
    printf("ALIGN(7, 8)=%d\n", ALIGN(7, 8));
    printf("ALIGN(50, 32)=%d\n", ALIGN(50, 32));
    printf("ALIGN(8, 1)=%d\n", ALIGN(8, 1));
    printf("ALIGN(1, 8)=%d\n", ALIGN(1, 8));
}

/*
 * 对齐规则
 * 1. 数据类型自身的对齐值(char 1字节对齐, short 2字节对齐等, 按sizeof(typeof()))
 * 2. 结构体或者类的自身对齐值：其成员中自身对齐值最大的那个值
 * 3. 指定对齐值：#pragma pack (value)时的指定对齐值value
 * 4. 数据成员、结构体和类的有效对齐值：自身对齐值和指定对齐值中小的那个值
 */
static void alginment_rule_test(void)
{
    alginment_rule_basictype_test();
    alginment_rule_struct_test();
    char ch[8] = {'g', 'o', 'o', 'd', 'l', 'u', 'c', 'k'};
    char *p = &ch[1];
    int i = *(int *)p;
    printf("i = 0x%x\n", i);

    for (i = 0; i < 8; i++) {
        printf("ch[%d]=0x%2x\n", i, ch[i]);
    }
}
static void alginment_rule_basictype_test(void)
{
    int a = 8;

    printf("char   aligned by %ld bytes\n", sizeof(typeof(*(char *)&a)));
    printf("short  aligned by %ld bytes\n", sizeof(typeof(*(short *)&a)));
    printf("int    aligned by %ld bytes\n", sizeof(typeof(a)));
    printf("long   aligned by %ld bytes\n", sizeof(typeof(*(long *)&a)));
    printf("float  aligned by %ld bytes\n", sizeof(typeof(*(float *)&a)));
    printf("double aligned by %ld bytes\n", sizeof(typeof(*(double *)&a)));
    printf("long double aligned by %ld bytes\n", sizeof(typeof(*(long double *)&a)));
    printf("long long aligned by %ld bytes\n", sizeof(typeof(*(long long *)&a)));
}

static void alginment_rule_struct_test(void)
{
    struct student {
        double f; //8
        char sex; //1+3 3??
        //int bit:2; /*  */
        int length;//4
        char c; //1+1
        short s; //2
        struct {
            float fl; //4
            char buff[17];//17+3
        }name;
        //char pad;//4
    };

    struct student stu = {0};
    printf("sizeof(struct sudent)=%ld\n", sizeof(struct student));
    /*printf("sizeof(stu.bit)=%ld\n", sizeof(stu.bit));*/ /* bit-filed is not allowed by sizeof() */
    printf("&stu=%p\n", &stu);
    printf("&(stu.f)=%p\n", &(stu.f));
    printf("&(stu.sex)=%p\n", &(stu.sex));
    /*printf("&(stu.bit)=%p\n", &(stu.bit));*/ /* cannot take address of bit-field ‘bit’ */
    printf("&(stu.length)=%p\n", &(stu.length));
    printf("&(stu.c)=%p\n", &(stu.c));
    printf("&(stu.s)=%p\n", &(stu.s));
    printf("&(stu.name)=%p\n", &(stu.name));
    printf("&(stu.name.fl)=%p\n", &(stu.name.fl));
    printf("&(stu.name.buff[0])=%p\n", &(stu.name.buff[0]));
    printf("&(stu.name.buff[9])=%p\n", &(stu.name.buff[9]));
    printf("&(stu.name.buff[20])=%p\n", &(stu.name.buff[20]));
    //printf("&(stu.pad)=%p\n", &(stu.pad));

    //printf("&(stu.name[9])=%p\n", &(stu.name[9]));
    printf("(&stu + 1)=%p\n", (&stu + 1));
}

/*
 * a struct mixed will start on an eight-byte boundary
 */
struct mixed {
    char c;   /* byte 0 */
              /* bytes 1-7 are "padding" */
    double d; /* byte 8-15 */
    short s;  /* byte 16-17 */
              /* bytes 18-23 are tail "padding" */
} __aligned__();


struct ieee754_double_big_endian {
    unsigned long long sign:1;
    unsigned long long bexp:11;
    unsigned long long manthi:52;
};

static void alginment_rule_struct_test2(void)
{
    struct mixed array[3];
    printf("sizeof(struct mixed) = %ld\n", sizeof(struct mixed));
    printf("&array[0] = %p\n", &array[0]);
    printf("&array[1] = %p\n", &array[1]);
    printf("&array[2] = %p\n", &array[2]);
    printf("sizeof(struct ieee754_double_big_endian) = %ld\n", sizeof(struct ieee754_double_big_endian));
}