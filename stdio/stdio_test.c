#include <stdio.h>

static void printf_test();
static void std_in_out_err_test();
static void sscanf_test();
int main(int argc, char **argv)
{
    printf_test();
    std_in_out_err_test();
    sscanf_test();
    return 0;
}

const char warn_no_pathmtu[] =
"WARNING: %s Path MTU Discovery may not be enabled.\n";

static void printf_test(void)
{
    /* printf(warn_no_pathmtu ); */ /* error version */
    printf("%s", warn_no_pathmtu );
}

static void std_in_out_err_test(void)
{
/*
/bionic/libc/include/stdio.h
#define  stdin   (&__sF[0])
#define  stdout  (&__sF[1])
#define  stderr  (&__sF[2])
*/
    int delta, delta2, size;
    printf("stdin =0x%x\n", *(unsigned int *)stdin);
    printf("stdout=0x%x\n", *(unsigned int *)stdout);
    printf("stderr=0x%x\n", *(unsigned int *)stderr);
    printf("sizeof(FILE) = 0x%lx\n", sizeof(FILE));

    delta = *(unsigned int *)stdout - *(unsigned int *)stdin;
    delta2= *(unsigned int *)stderr - *(unsigned int *)stdout;
    size = sizeof(FILE);

    if (delta == delta2) {
        printf("stdin/stdout/stderr are in order\n");
        if (delta == size) {
            printf("stdin/stdout/stderr are type of FILE\n");
        } else {
            printf("stdin/stdout/stderr are not type of FILE\n");
            printf("sizeof(*stdout) = %ud\n", delta);
            printf("sizeof(*stdout) = %ld\n", sizeof(*stdout));
        }
    } else {
        printf("stdin/stdout/stderr are not in order\n");
    }
}

/*
 * sscanf()返回匹配赋值成功的数目
 * 例子中x,y为两个匹配
 */
static void sscanf_test()
{
    char buf[] = "7 8";
    int x;
    int y;
    int res = sscanf(buf, "%d %d", &x, &y);
    printf("res = %d\n", res);
    printf("x = %d\n", x);
    printf("y = %d\n", y);
}
