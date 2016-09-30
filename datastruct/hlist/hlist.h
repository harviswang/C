#ifndef _HLIST_H
#define _HLIST_H

/*
 * hash list
 * it's a single link list, linked by next
 * pprev just store &prev->next
 */

struct hlist_node {
    struct hlist_node *next, **pprev;
};

struct hlist_head {
    struct hlist_node *first;
};

void hlist_add_head(struct hlist_head *h, struct hlist_node *n);
void hlist_delete_node(struct hlist_node *n);
void hlist_traverse(struct hlist_head *h, int (*callback)(void *ctx, void *arg), void *ctx);

#endif
