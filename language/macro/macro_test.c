#include <stdio.h>
#include <stdlib.h>

/*
 * assert macro
 */
#define LOG_ASSERT(condition, format...) \
    do {if (!(condition)) {printf("Assert failed: " #condition ": "format);}}while(0);

static void syscall_macro_test();
static void tlv_macro_test();
static void variadic_macro_test();
static void compile_date_time_test();
static void variable_argument_macro_function_test();
static void gcc_version_dump_test();
static void var_name_unchange_test();
static void function_name_ifdef_test();
int main(int argc, char **argv)
{
    variadic_macro_test();
    compile_date_time_test();
    var_name_unchange_test();
	function_name_ifdef_test();
	gcc_version_dump_test();
	variable_argument_macro_function_test();
	return 0;
}

/*
 * 宏参数原样使用, 不做任何解析
 */
static void var_name_unchange_test()
{
    /* 宏作用域只有上下位置之分, 没有{}块内外之分 */
    {
        /*
         * 打印变量名(不对变量求值，实际是将变量名当作字符串)和行号
         * 主要使用宏语法里面的关键字'#'
         */
        #define MY_MUTEX_LOC(_lock) do {         \
            printf("%s %d\n", #_lock, __LINE__); \
            printf("unlock .....\n");            \
        }while(0)
    }
    int a = 88;
    (void)a; /* avoid annoying warning*/
    MY_MUTEX_LOC(&a); /* ‘&a’ 被当作一个整体打印出来 */
}

static void barrier(){}
/*
 * 测试函数名是否可以被#ifdef识别
 * *不会被识别*
 */
static void function_name_ifdef_test()
{
#ifdef barrier
    printf("%s %s():%d\n", __FILE__, __func__, __LINE__);
#endif
    barrier();
}

/*
 * works both gcc and clang in centos 7
 */
static void gcc_version_dump_test()
{
    /*
     * Common definitions for all gcc versions go here.
     */
    #define GCC_VERSION (__GNUC__ * 10000 \
               + __GNUC_MINOR__ * 100 \
               + __GNUC_PATCHLEVEL__)

    printf("GCC_VERSION is %d\n", GCC_VERSION);
    printf("__GNUC__ is %d\n", __GNUC__);
    printf("__GNUC_MINOR__ is %d\n", __GNUC_MINOR__);
    printf("__GNUC_PATCHLEVEL__ is %d\n", __GNUC_PATCHLEVEL__);
}

static void variable_argument_macro_function_test()
{
    int a = 8;
    LOG_ASSERT(a == 8, "%d is not bigger than 9\n", a);
}

static void compile_date_time_test()
{
    printf("compile date/time: %s %s\n", __DATE__, __TIME__);
}

static void tlv_macro_test()
{
#define TLV_LENGTH(...) \
    ((unsigned int)sizeof((const unsigned int[]) { __VA_ARGS__ }))

#define TLV_ITEM(type, ...) \
    (type), TLV_LENGTH(__VA_ARGS__), __VA_ARGS__

#define SNDRV_CTL_TLVT_DB_SCALE 1       /* dB scale */
#define TLV_DB_SCALE_MASK   0xffff
#define TLV_DB_SCALE_MUTE   0x10000
#define TLV_DB_SCALE_ITEM(min, step, mute)          \
    TLV_ITEM(SNDRV_CTL_TLVT_DB_SCALE,           \
         (min),                 \
         ((step) & TLV_DB_SCALE_MASK) |     \
            ((mute) ? TLV_DB_SCALE_MUTE : 0))

#define DECLARE_TLV_DB_SCALE(name, min, step, mute) \
    unsigned int name[] = { TLV_DB_SCALE_ITEM(min, step, mute) }

    static const DECLARE_TLV_DB_SCALE(dac_tlv, -3100, 100, 0);
    int i;
    for (i = 0; i < sizeof(dac_tlv)/sizeof(dac_tlv[0]); i++) {
        printf("dac_tlv[%d] = %d\n", i, dac_tlv[i]);
    }
    printf("%d\n", TLV_LENGTH(dac_tlv[0], dac_tlv[1]));
}

/*
 * gcc: sendto在预先编译后后面的';'出现了错行的问题, mips_map就没有分号错行的问题.
 * clang: 没有gcc的错行的问题.
 */
static void syscall_macro_test()
{
#define asmlinkage __attribute__((regparm(0)))
#define __user  __attribute__(())

#define __SC_DECL1(t1, a1)    t1 a1
#define __SC_DECL2(t2, a2, ...) t2 a2, __SC_DECL1(__VA_ARGS__)
#define __SC_DECL3(t3, a3, ...) t3 a3, __SC_DECL2(__VA_ARGS__)
#define __SC_DECL4(t4, a4, ...) t4 a4, __SC_DECL3(__VA_ARGS__)
#define __SC_DECL5(t5, a5, ...) t5 a5, __SC_DECL4(__VA_ARGS__)
#define __SC_DECL6(t6, a6, ...) t6 a6, __SC_DECL5(__VA_ARGS__)

#define __SYSCALL_DEFINEx(x, name, ...)                 \
    asmlinkage long sys##name(__SC_DECL##x(__VA_ARGS__))

#define SYSCALL_DEFINEx(x, sname, ...)              \
    __SYSCALL_DEFINEx(x, sname, __VA_ARGS__)

#define SYSCALL_DEFINE6(name, ...) SYSCALL_DEFINEx(6, _##name, __VA_ARGS__)

    struct sockaddr; /* 声明结构体struct sockaddr, 下面用的是指针, 避免编译器报错 */
    SYSCALL_DEFINE6(sendto, int, fd, void __user *, buff, size_t, len,
            unsigned, flags, struct sockaddr __user *, addr,
            int, addr_len);
    SYSCALL_DEFINE6(mips_mmap, unsigned long, addr, unsigned long, len, unsigned long, prot, unsigned long, flags, unsigned long, fd, off_t, offset);
}

/*
 * 可变参数宏,-std=c89和-std=c99均支持
 */
static void variadic_macro_test()
{
    syscall_macro_test();
    tlv_macro_test();
}
