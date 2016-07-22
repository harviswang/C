#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char str[] = "work hard to get a good life with your family!";
    int len = strlen(str) + 1; /* + 1 是必须的, 否则, 下面的printf读错误 */
    char *s = (char *)malloc(len * sizeof(char));
    if (s != NULL) {
        strncpy(s, str, len);
        printf("%s\n", s);
        free(s);
        
        /* Invalid write of size 1 */
        s[len - 1] = 'E';
        /* Invalid free() / delete / delete[] / realloc() */
        free(s);
    }
    
    return 0;
}