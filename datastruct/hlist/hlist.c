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

void hlist_traverse_by_head(struct hlist_head *h, int (*callback)(void *ctx, void *arg), void *ctx)
{
    assert(h != NULL);

    struct hlist_node *n;
    for (n = h->first; n != NULL; n = n->next) {
        if ((*callback)(ctx, n)) {
            break;
        }
    }
}

void hlist_traverse_by_node(struct hlist_head *h, struct hlist_node *n, int (*callback)(void *ctx, void *arg), void *ctx)
{
    assert(h != NULL);
    assert(n != NULL);

    struct hlist_node *next, **pprev;
        
    /* traverse from n to the end of the list */
    for (next = n; next != NULL; next = next->next) {
        if ((*callback)(ctx, next)) {
            break;
        }
    }
    
    /* traverse from n->pprev to the head of the list */
    for (pprev = n->pprev; pprev != &h->first ; pprev = ((struct hlist_node *)pprev)->pprev) {
        if ((*callback)(ctx, (struct hlist_node *)pprev)) {
            break;
        }
    }
}