#ifndef _DLIST_H
#define _DLIST_H

/*
 * C++编译器会对函数名进行重新编码, 由于C++支持函数重名, 重载, 多态
 * extern "C" { body } 目的是禁止C++对body中的函数名重新编码
 */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DLIST_RET_OK = 0,
    DLIST_RET_ERR,
    DLIST_RET_OOM,
    DLIST_RET_INVALID_PARAMS
} dlist_ret_t;

/*
 * Debug
 */
#define return_if_fail(p) if(!(p))            \
do {printf("%s:%d Warning: " #p " failed.\n", \
    __func__, __LINE__); return; } while (0)

#define return_val_if_fail(p, ret) if(!(p))   \
do {printf("%s:%d Warning: " #p " failed.\n", \
    __func__, __LINE__); return (ret);} while(0)

/*
 * 隐藏数据结构方法: 在*.h代码中声明数据结构, 在*.c中定义数据结构
 */
struct dlist;

/*
 * 操作'struct dlist;'的方法
 */
struct dlist* dlist_init(void);
dlist_ret_t dlist_add(struct dlist* list, void* data);
dlist_ret_t dlist_delete(struct dlist* list, void* data);
dlist_ret_t dlist_search(struct dlist* list, void* data);
dlist_ret_t dlist_destroy(struct dlist *list);
dlist_ret_t dlist_printf(struct dlist *list, dlist_ret_t (*user_printf)(void *data));
dlist_ret_t dlist_foreach(struct dlist *list, dlist_ret_t (*visit)(void *context, void *data), void *cxt);
int dlist_length(struct dlist *list);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _DLIST_H */
