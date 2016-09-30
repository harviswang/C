#include <stdio.h>
#include "hlist.h"

struct cup {
    struct hlist_node node;
    int price;
};

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

#define MAX 101
int main(int argc, char **argv)
{
    struct hlist_head head = { NULL }; /* must be initialed NULL */
    int sum;
    
    struct cup teacup[MAX];
    int i;
    
    for (i = 0; i < MAX; i ++) {
        teacup[i].node.next = NULL;
        teacup[i].node.pprev = NULL;
        teacup[i].price = i;
        
        hlist_add_head(&head, &teacup[i].node);
    }
 
    hlist_traverse_by_node(&head, &teacup[55].node, print_int, NULL);
    sum = 0;
    hlist_traverse_by_head(&head, sum_price, &sum);
    printf("sum = %d\n", sum);

    /* delete test */
    for (i = MAX / 2; i < MAX; i++) {
        hlist_delete_node(&teacup[i].node);
    }
    
    sum = 0;
    hlist_traverse_by_head(&head, sum_price, &sum);
    printf("sum = %d\n", sum);
    return 0;
}