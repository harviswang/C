#include <stdio.h>
#include "hlist.h"

struct cup {
    struct hlist_node node;
    int price;
};

struct hlist_head head;

int print_int(void *ctx, void *arg)
{
    int error = 0;
    struct cup *c = (struct cup *)arg;

    printf("price: %d\n", c->price);
    
    return error;
}

int sum_price(void *ctx, void *arg)
{
    int error = 0;
    int *sum = (int *)ctx;
    struct cup *c = (struct cup *)arg;

    *sum += c->price;
    
    return error;
}

int main(int argc, char **argv)
{
    int sum;
    struct cup teacup[2] = {{{NULL, NULL}, 1}, {{NULL, NULL}, 2}};
    hlist_add_head(&head, &teacup[0].node);
    hlist_add_head(&head, &teacup[1].node);
    hlist_traverse(&head, print_int, NULL);
    
    sum = 0;
    hlist_traverse(&head, sum_price, &sum);
    printf("sum = %d\n", sum);

    return 0;
}