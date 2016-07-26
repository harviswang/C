#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dlist.h"

struct node;

struct dlist {
    struct node *head;  /* podlist_ret_t to the head of list */
    dlist_ret_t length; /* the number of list */
};

struct node {
    struct node *prev;
    struct node *next;
    void *data;
};

static struct node* node_new(void *data);
static dlist_ret_t node_insert(struct node *prev, struct node *node, struct node *next);
static dlist_ret_t node_add(struct node **head, struct node* node);
static struct node* node_search(struct node **head, void* data);
static void __node_delete(struct node* prev, struct node *next);
static void node_delete(struct node* node);
//static void dump_node(struct node* node);

struct dlist* dlist_init(void)
{
    struct dlist* list = (struct dlist*)malloc(sizeof(struct dlist));
    if (list) {
        list->head = NULL;
        list->length = 0;
        return list;
    } else {
        return NULL;
    }
}

dlist_ret_t dlist_add(struct dlist* list, void* data)
{
    dlist_ret_t ret;
    struct node *node = node_new(data);
    if (data && node) {
        ret = node_add(&list->head, node);
        list->length++;
        return ret;
    } else {
        return DLIST_RET_ERR;
    }
}

dlist_ret_t dlist_delete(struct dlist* list, void* data)
{
    if (data) {
        struct node *node = node_search(&list->head, data);
        if (node) {
            if (node == list->head) {
                list->head = node->next;
            }
            node_delete(node);
            list->length--;
            if (list->length == 0) {
                list->head = NULL;
            }

            return DLIST_RET_OK;
        }
    }
    return DLIST_RET_ERR;
}

dlist_ret_t dlist_search(struct dlist* list, void* data)
{
    return node_search(&list->head, data) ? DLIST_RET_OK : DLIST_RET_ERR;
}

/*
 * 先删除list->head->next, list->head->next->next, ....
 * 最后删除list->head
 */
dlist_ret_t dlist_destroy(struct dlist *list)
{
    if (list) {
        if (list->head) {
            struct node *temp;
            struct node *node = list->head;

            node = node->next;
            while (node != list->head) {
                temp = node->next;
                node_delete(node);
                node = temp;
            }
            node_delete(list->head);
            list->head = NULL;
            list->length = 0;
        }
        free(list);

        return DLIST_RET_OK;
    }
    return DLIST_RET_ERR;
}

dlist_ret_t dlist_printf(struct dlist *list, dlist_ret_t (*user_printf)(void *data))
{
    struct node *node = list->head;
    do {
        user_printf(node->data);
        node = node->next;
    } while (node != list->head);
    return DLIST_RET_OK;
}

dlist_ret_t dlist_foreach(struct dlist *list, dlist_ret_t (*visit)(void *context, void *data), void *cxt)
{
    struct node *node = list->head;
    do {
        visit(cxt, node->data);
        node = node->next;
    } while (node != list->head);
    return DLIST_RET_OK;
}

int dlist_length(struct dlist *list)
{
    return list->length;
}

static struct node* node_new(void *data)
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    if (node) {
        node->prev = node;
        node->next = node;
        node->data = data;
    }
    return node;
}

/*
 * prev, next
 * prev, node, next
 */
static dlist_ret_t node_insert(struct node *prev, struct node *node, struct node *next)
{
    node->next = next;
    node->prev = prev;
    prev->next = node;
    next->prev = node;
    return DLIST_RET_OK;
}

/*
 * head, head->next
 * head, node , head->next
 */
static dlist_ret_t node_add(struct node **head, struct node* node)
{
    if (*head) {
        if (node) {
            node_insert(*head, node, (*head)->next);
            return DLIST_RET_OK;
        }
    } else {
        if (node) {
            *head = node;
            return DLIST_RET_OK;
        }
    }
    return DLIST_RET_ERR;
}

static struct node* node_search(struct node **head, void* data)
{
    struct node *node = *head;

    do {
        if (node->data == data) {
            return node;
        }
        node = node->next;
    } while (node != *head);
    return NULL;
}

/*
 * prev, node, next
 *    prev, next
 */
static void __node_delete(struct node *prev, struct node *next)
{
    prev->next = next;
    next->prev = prev;
}

static void node_delete(struct node *node)
{
    __node_delete(node->prev, node->next);
    free(node);
}

//static void dump_node(struct node *node)
//{
//    printf("node = %p\n", node);
//    printf("prev = %p\n", node->prev);
//    printf("next = %p\n", node->next);
//    printf("data = %p\n", node->data);
//}
