#include <assert.h>
#include <stdio.h>
#include "hlist.h"

/*
 * add a node @n in a hlist head @h
 */
void hlist_add_head(struct hlist_head *h, struct hlist_node *n)
{
    assert(h != NULL);
    assert(n != NULL);

    n->next = h->first;
    if (h->first) {
        h->first->pprev = &n->next;
    }
    n->pprev = &h->first;
    h->first = n;
}

/*
 * delete a hlist node @n
 */
void hlist_delete_node(struct hlist_node *n)
{
    assert(n != NULL);

    struct hlist_node *next = n->next;
    struct hlist_node **pprev = n->pprev;
    
    if (pprev) {
        *pprev = n->next;
    }

    if (next) {
        next->pprev = pprev;
    }
}

void hlist_traverse(struct hlist_head *h, int (*callback)(void *ctx, void *arg), void *ctx)
{
    assert(h != NULL);

    struct hlist_node *n;
    for (n = h->first; n != NULL; n = n->next) {
        if ((*callback)(ctx, n)) {
            break;
        }
    }
}