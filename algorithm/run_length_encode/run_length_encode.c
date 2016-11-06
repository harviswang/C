#include <stdio.h>

char buf[1024];
// 0 -'0' = 9 - '9'

char *run_length_encode(const char *str)
{
    int count = 0;
    int index = 0;
    char last = *str;
    for (; *str;) {
        buf[index++] = *str;
        count = 1;
        
        while (last == *++str) {
            count ++;
        }
        last = *str;
        buf[index++] = count + '0';        
    }
    return buf;
}

int main(int argc, char *argv[])
{
    char *str = "aaaaaaaaabbbaxxxxyyyzyx";
    char *rle = run_length_encode(str);
    printf("%s\n", rle);
    
    return 0;
}