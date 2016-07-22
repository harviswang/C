#include <stdio.h>
#include "dlist.h"

#define MAX 4096

static void user_printf(void *data)
{
    printf("%ld\n", (unsigned long)data);
}

int main(int argc, char **argv)
{
    printf("double list test ... MAX=%d\n", MAX);
    int arr[MAX];
    int i;
    int ret;
    struct dlist *list = dlist_init();
    if (list) {
        for (i = 0; i < MAX; i++) {
            dlist_add(list, arr + i);
            arr[i] = i;
        }
        i = 0;
        dlist_delete(list, arr + i);
        for (i = 1; i < MAX; i++) {
            ret = dlist_search(list, arr + i);
            if (!ret) {
                printf("Not found it %d\n", i);
            }
        }
    
        dlist_printf(list, user_printf);
        dlist_destroy(list);
    }

    return 0;
}
