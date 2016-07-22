#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char x[50];
    int i;
    
    for (i = 0; i < 50; i++) {
        x[i] = i + 1;
    }
    
    strncpy(x + 20, x, 20);
    /* Source and destination overlap in strncpy */
    strncpy(x + 20, x, 21);
    
    x[39] = 39;
    x[40] = '\0';
    /* Source and destination overlap in strcpy */
    strcpy(x, x + 20);
}