#include <stdio.h>
#include <string.h>

static void macro_as_part_of_string(void);
/*static */int main(int argc, char **argv)
{
    macro_as_part_of_string();
    return 0;
}

/*
 * 宏的数字变成字符串
 * 1. 宏中的#的功能是将其后面的宏参数进行字符串化操作（Stringizing operator）
 *    简单说就是在它引用的宏变量的左右各加上一个双引号
 * 2. #@是加单引号（Charizing Operator）gcc/clang并不支持它
 * 3. 宏中遇到#或##时就不会再展开宏中嵌套的宏了, 需要使用一个中间转换宏达到展开宏的目的
 * 4. ##的功能，它可以拼接符号（Token-pasting operator）
 */
static void macro_as_part_of_string(void)
{
#define CONFIG_BAUDRATE 2000000
#define __STRING(x) #x /* (#x) is invalid */
#define STRING(x) __STRING(x)
#define BOOTARGS_CONSOLE "console=ttyS3,"STRING(CONFIG_BAUDRATE)"n8 "
    printf("%s\n", BOOTARGS_CONSOLE);

    char *pChar = "hello";
    char *pChar2 = __STRING(hello); /* hello -> "hello" */
    if(!strcmp(pChar, pChar2)) {
        printf("pChar:%s is equal to pChar2:%s\n", pChar, pChar2);
    }
/* gcc/clang not support #@
#define TOCHAR(x) #@x
    char c =  TOCHAR(A);
    printf("c is %c\n", c);
*/
    printf("%s\n", __STRING(__FILE__));
    printf("%s\n", STRING(__FILE__));

#define paster(n) printf("token"#n" = %d\n", token##n)
    int token9 = 100;
    paster(9);

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
    printf("%s\n", L"hello");
}
