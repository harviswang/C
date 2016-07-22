#ifndef _DLIST_H
#define _DLIST_H

/*
 * C++编译器会对函数名进行重新编码, 由于C++支持函数重名, 重载, 多态
 * extern "C" { body } 目的是禁止C++对body中的函数名重新编码
 */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * 隐藏数据结构方法: 在*.h代码中声明数据结构, 在*.c中定义数据结构
 */
struct dlist;

/*
 * 操作'struct dlist;'的方法
 */
struct dlist* dlist_init(void);
int dlist_add(struct dlist* list, void* data);
int dlist_delete(struct dlist* list, void* data);
int dlist_search(struct dlist* list, void* data);
int dlist_destroy(struct dlist *list);
int dlist_printf(struct dlist *list, void (*user_printf)(void *data));
int dlist_foreach(struct dlist *list, void (*visit)(void *context, void *data), void *cxt);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _DLIST_H */
