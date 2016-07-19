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
    int len;
};

static struct node* new_node(void *data, int len);
static int insert_node(struct node *prev, struct node *node, struct node *next);
static int add_node(struct node **head, struct node* node);
static struct node* search_node(struct node **head, void* data, int len);
static void __delete_node(struct node* prev, struct node *next);
static void delete_node(struct node* node);
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

int dlist_add(struct dlist* list, void* data, int len)
{
    struct node *node = new_node(data, len);
    if (data && len && node) {
        return add_node(&list->head, node);
    } else {
        return 0;
    }
}

int dlist_delete(struct dlist* list, void* data, int len)
{
    if (data && len) {
        struct node *node = search_node(&list->head, data, len);
        if (node) {
            if (node == list->head) {
                list->head = node->next;
            }
            delete_node(node);
            return 1;
        }
    }
    return 0;
}

int dlist_search(struct dlist* list, void* data, int len)
{
    return search_node(&list->head, data, len) != NULL;
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
            delete_node(node);
            node = temp;
        }
        delete_node(list->head);

        free(list);
        memset(list, 0, sizeof(*list));
        list = NULL;

        return 1;
    }
    return 0;
}

static struct node* new_node(void *data, int len)
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    if (node) {
        node->prev = node;
        node->next = node;
        node->data = data;
        node->len  = len;
    }
    return node;
}

/*
 * prev, next
 * prev, node, next
 */
static int insert_node(struct node *prev, struct node *node, struct node *next)
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
static int add_node(struct node **head, struct node* node)
{
    if (*head) {
        if (node) {
            insert_node(*head, node, (*head)->next);
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

static struct node* search_node(struct node **head, void* data, int len)
{
    struct node *node = *head;

    do {
        if (node->data == data && node->len == len) {
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
static void __delete_node(struct node *prev, struct node *next)
{
    prev->next = next;
    next->prev = prev;
}

static void delete_node(struct node *node)
{
    __delete_node(node->prev, node->next);
    free(node);
    memset(node, 0, sizeof(*node));
}

//static void dump_node(struct node *node)
//{
//    printf("node = %p\n", node);
//    printf("prev = %p\n", node->prev);
//    printf("next = %p\n", node->next);
//    printf("data = %p\n", node->data);
//    printf("len = %d\n", node->len);
//}
