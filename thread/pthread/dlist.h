#ifndef _DLIST_H
#define _DLIST_H

/*
 * 隐藏数据结构方法: 在*.h代码中声明数据结构, 在*.c中定义数据结构
 */
struct dlist;

/*
 * 操作'struct dlist;'的方法
 */
struct dlist* dlist_init(void);
int dlist_add(struct dlist* list, void* data, int len);
int dlist_delete(struct dlist* list, void* data, int len);
int dlist_search(struct dlist* list, void* data, int len);
int dlist_destroy(struct dlist *list);
#endif