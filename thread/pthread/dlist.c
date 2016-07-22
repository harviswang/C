#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node;

struct dlist {
    struct node *head;
};

struct node {
    struct node *prev;
    struct node *next;
    void *data;
};

static struct node* node_new(void *data);
static int node_insert(struct node *prev, struct node *node, struct node *next);
static int node_add(struct node **head, struct node* node);
static struct node* node_search(struct node **head, void* data);
static void __node_delete(struct node* prev, struct node *next);
static void node_delete(struct node* node);
//static void dump_node(struct node* node);

struct dlist* dlist_init(void)
{
    struct dlist* list = (struct dlist*)malloc(sizeof(struct dlist));
    if (list) {
        list->head = NULL;
        return list;
    } else {
        return NULL;
    }
}

int dlist_add(struct dlist* list, void* data)
{
    struct node *node = node_new(data);
    if (data && node) {
        return node_add(&list->head, node);
    } else {
        return 0;
    }
}

int dlist_delete(struct dlist* list, void* data)
{
    if (data) {
        struct node *node = node_search(&list->head, data);
        if (node) {
            if (node == list->head) {
                list->head = node->next;
            }
            node_delete(node);
            node = NULL;

            return 1;
        }
    }
    return 0;
}

int dlist_search(struct dlist* list, void* data)
{
    return node_search(&list->head, data) != NULL;
}

/*
 * 先删除list->head->next, list->head->next->next, ....
 * 最后删除list->head
 */
int dlist_destroy(struct dlist *list)
{
    if (list && list->head) {
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

        free(list);
        memset(list, 0, sizeof(*list));

        return 1;
    }
    return 0;
}

int dlist_printf(struct dlist *list, void (*user_printf)(void *data))
{
    struct node *node = list->head;
    do {
        user_printf(node->data);
        node = node->next;
    } while (node != list->head);
    return 1;
}

int dlist_foreach(struct dlist *list, void (*visit)(void *context, void *data), void *cxt)
{
    struct node *node = list->head;
    do {
        visit(cxt, node->data);
        node = node->next;
    } while (node != list->head);
    return 1;
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
static int node_insert(struct node *prev, struct node *node, struct node *next)
{
    node->next = next;
    node->prev = prev;
    prev->next = node;
    next->prev = node;
    return 1;
}

/*
 * head, head->next
 * head, node , head->next
 */
static int node_add(struct node **head, struct node* node)
{
    if (*head) {
        if (node) {
            node_insert(*head, node, (*head)->next);
            return 1;
        }
    } else {
        if (node) {
            *head = node;
            return 1;
        }
    }
    return 0;
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
    memset(node, 0, sizeof(*node));
}

//static void dump_node(struct node *node)
//{
//    printf("node = %p\n", node);
//    printf("prev = %p\n", node->prev);
//    printf("next = %p\n", node->next);
//    printf("data = %p\n", node->data);
//}
