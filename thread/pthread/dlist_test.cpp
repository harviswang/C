#include <stdio.h>
#include <stdlib.h> /* free */
#include <limits.h> /* INT_MIN */
#include <string.h>
#include <ctype.h> /* islower */
#include "dlist.h"

#define MAX 3
static int dlist_char_test();
static int dlist_int_test();

int main(int argc, char **argv)
{
    //dlist_int_test();

    dlist_char_test();
    return 0;
}

static void toupper(char *ch)
{
    int i;
    int len = (int)strlen(ch);
    for (i = 0; i < len; i++, ch++) {
        if (islower(*ch)) {
            *ch = toupper(*ch);
        }
    }
}

static void string_toupper_cb(void *ctx, void *data)
{
    toupper((char *)data);
}

static void string_printf(void *data)
{
    printf("%s\n", (char *)data);
}

static int dlist_char_test()
{
    char str[] = "dlist";
    char str2[] = "thinking";
    struct dlist *list = dlist_init();

    printf("%s\n", str);
    printf("%s\n", *(&str));
    printf("str  = %p\n", str);
    printf("&str = %p\n", &str);
    if (list) {
        char *pstr = NULL;
        dlist_add(list, str);
        dlist_add(list, str2);
        dlist_add(list, pstr = strdup("test"));
        dlist_foreach(list, string_toupper_cb, NULL);
        dlist_printf(list, string_printf);
        dlist_destroy(list);
        list = NULL;
        free(pstr);
    }


    return 0;
}

static void user_printf(void *data)
{
    printf("%p\n", data);
}

static void sum_cb(void *cxt, void *data)
{
    *(int *)cxt += *(int *)data;
}

static void max_cb(void *cxt, void *data)
{
    if (*(int *)cxt < *(int *)data) {
        *(int *)cxt = *(int *)data;
    }
}

static int dlist_int_test()
{
    printf("double list test ... MAX=%d\n", MAX);
    int arr[MAX];
    int i;
    int ret;
    int sum = 0;
    int max = INT_MIN;
    printf("max = %d\n", max);
    struct dlist *list = dlist_init();
    if (list) {
        for (i = 0; i < MAX; i++) {
            dlist_add(list, arr + i);
            arr[i] = i;
        }
        i = 0;
        //dlist_delete(list, arr + i);
        for (i = 1; i < MAX; i++) {
            ret = dlist_search(list, arr + i);
            if (!ret) {
                printf("Not found it %d\n", i);
            }
        }
    
        dlist_printf(list, user_printf);
        dlist_foreach(list, sum_cb, &sum);
        dlist_foreach(list, max_cb, &max);
        dlist_destroy(list);
    }

    printf("sum = %d\n", sum);
    printf("max = %d\n", max);

    return 0;
}
