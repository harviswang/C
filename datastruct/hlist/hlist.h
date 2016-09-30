#ifndef _HLIST_H
#define _HLIST_H

/*
 * hash list
 * it's a double link list, linked by next and pprev
 * pprev just store &prev->next, and &prev->next == &prev
 */

struct hlist_node {
    struct hlist_node *next, **pprev;
};

struct hlist_head {
    struct hlist_node *first;
};

void hlist_add_head(struct hlist_head *h, struct hlist_node *n);
void hlist_delete_node(struct hlist_node *n);
void hlist_traverse_by_head(struct hlist_head *h, int (*callback)(void *ctx, void *arg), void *ctx);
void hlist_traverse_by_node(struct hlist_head *h, struct hlist_node *n, int (*callback)(void *ctx, void *arg), void *ctx);

#endif
