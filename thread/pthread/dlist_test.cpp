#include <stdio.h>
#include <stdlib.h> /* free */
#include <limits.h> /* INT_MIN */
#include <string.h>
#include <ctype.h> /* islower */
#include <assert.h>
#include "dlist.h"

#define MAX 3
static int dlist_char_test();
static int dlist_int_test();

int main(int argc, char **argv)
{
    dlist_int_test();

    //return_val_if_fail(NULL, -3);
    //dlist_char_test();
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

static dlist_ret_t string_toupper_cb(void *ctx, void *data)
{
    toupper((char *)data);
    return DLIST_RET_OK;
}

static dlist_ret_t string_printf(void *data)
{
    printf("%s\n", (char *)data);
    return DLIST_RET_OK;
}

static int dlist_char_test()
{
    char str[] = "dlist";
    char str2[] = "thinking";
    struct dlist *list = dlist_init();

    assert(list != NULL);
    return_val_if_fail(dlist_length(list) == 0, -1);
    char *pstr = NULL;
    dlist_add(list, str);
    return_val_if_fail(dlist_length(list) == 1, -1);
    dlist_add(list, str2);
    dlist_add(list, pstr = strdup("test"));
    dlist_foreach(list, string_toupper_cb, NULL);
    dlist_printf(list, string_printf);
    dlist_destroy(list);
    list = NULL;
    free(pstr);

    return 0;
}

static dlist_ret_t user_printf(void *data)
{
    printf("%d\n", *(int *)data);
    return DLIST_RET_OK;
}

static dlist_ret_t sum_cb(void *cxt, void *data)
{
    *(int *)cxt += *(int *)data;
    return DLIST_RET_OK;
}

static dlist_ret_t max_cb(void *cxt, void *data)
{
    if (*(int *)cxt < *(int *)data) {
        *(int *)cxt = *(int *)data;
    }
    return DLIST_RET_OK;
}

static int dlist_int_test()
{
    printf("double list test ... MAX=%d\n", MAX);
    int arr[MAX];
    int i;
    int sum = 0;
    int max = INT_MIN;
    printf("MAX = %d\n", MAX);
    struct dlist *list = dlist_init();
    if (list) {
        /*
         * init arr
         */
        for (i = 0; i < MAX; i++) {
            arr[i] = i;
        }

        /*
         * dlist_add/dlist_length test
         */
        for (i = 0; i < MAX; i++) {
            dlist_add(list, arr + i);
            assert(dlist_length(list) == i + 1);
        }

        /*
         * dlist_serch test
         */
        for (i = 0; i < MAX; i++) {
            assert(dlist_search(list, arr + i) == DLIST_RET_OK);
        }
    
        /*
         * dlist_printf test
         */
        assert(dlist_printf(list, user_printf) == DLIST_RET_OK);
        assert(dlist_foreach(list, sum_cb, &sum) == DLIST_RET_OK);
        assert(dlist_foreach(list, max_cb, &max) == DLIST_RET_OK);

        /*
         * dlist_delete test
         */
        for (i = MAX - 1; i >= 0; i--) {
            assert(dlist_length(list) == i + 1);
            assert(dlist_delete(list, arr + i) == DLIST_RET_OK);
            assert(dlist_length(list) == i);
        }

        /*
         * dlist_destroy test
         */
        assert(dlist_destroy(list) == DLIST_RET_OK);
    }

    printf("sum = %d\n", sum);
    printf("max = %d\n", max);

    return 0;
}
