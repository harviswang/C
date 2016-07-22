/*
 * valgrind内存泄露的类型:
 * 1. 一种是可能的内存泄露（Possibly lost）
 *    Possibly lost 是指仍然存在某个指针能够访问某块内存，但该指针指向的已经不是该内存首地址.
 * 2. 另外一种是确定的内存泄露（Definitely lost）
 *    Definitely lost 是指已经不能够访问这块内存。
 *    Definitely lost又分为两种：直接的（direct）和间接的（indirect）
 *       直接是没有任何指针指向该内存,间接是指指向该内存的指针都位于内存泄露处。
 *       在下面的例子中，根节点是directly lost，而其他节点是indirectly lost。
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    struct node *l;
    struct node *r;
    void *data;
} node_t;

node_t *node_create(node_t *l, node_t *r, void *data)
{
    node_t *node = (node_t *)malloc(sizeof(*node));
    if (node != NULL) {
        node->l = l;
        node->r = r;
        node->data = data;
    }
    return node;
}

void node_destroy(node_t *node)
{
    if (node != NULL) {
        node_destroy(node->l);
        node_destroy(node->r);
        node->data = NULL;
        free(node);
    }
}

int main(int argc, char *argv[])
{
    node_t *tree1, *tree2, *tree3;
    
    tree1 = node_create(node_create(node_create(NULL, NULL, "3"), NULL, "2"), NULL, "1");
    tree2 = node_create(node_create(node_create(NULL, NULL, "6"), NULL, "5"), NULL, "4");
    tree3 = node_create(node_create(tree1, tree2, "8"), NULL, "7");
    
    /*node_destroy(tree3);*/
    return 0;
}