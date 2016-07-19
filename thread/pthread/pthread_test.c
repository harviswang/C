/*
 * pthread_t
 * pthread_mutex_t
 * pthread_cond_t
 * -------------------
 * pthread_create()
 * pthread_cond_signal()
 * pthread_join()
 * pthread_mutex_lock()
 * pthread_mutex_unlock()
 * pthread_cond_wait()
 * pthread_cond_signal()
 * pthread_self()
 * pthread_once()
 * TODO
 */
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

static void pthread_local_variable_as_argument_test(void);
static void pthread_argument_share_test(void);
static void pthread_false_concurrent_test(void);
int main(int argc, char **argv)
{
//    pthread_local_variable_as_argument_test();
//    pthread_argument_share_test();
    pthread_false_concurrent_test();

    return 0;
}

static void* start_routine(void* param)
{
    char* str = (char*)param;
    printf("%s:%s\n", __func__, str);
    return NULL;
}

static pthread_t create_test_thread()
{
    pthread_t id = 0;
    char str[] = "it is ok";
    pthread_create(&id, NULL, start_routine, str);
    return id;
}

/*
 * 用临时变量作线程参数的问题
 */
static void pthread_local_variable_as_argument_test(void)
{
    void* ret = NULL;
    pthread_t id = create_test_thread();
    pthread_join(id, &ret);
}

static void* start_routine2(void* param)
{
    int index = *(int *)param;
    printf("%s:%d\n", __func__, index);
    return NULL;
}

/*
 * 多个线程共享变量i
 * 由于新线程和当前线程是并发的, 谁先谁后是无法预测的.
 * i在不断变化, 所以新线程拿到的参数是无法预知的.
 * 打印出来的数字自然也是随机的
 */
#define THREADS_NR 10
static void create_test_threads(void)
{
    int i = 0;
    void* ret = NULL;
    pthread_t ids[THREADS_NR] = { 0 };
    for (i = 0; i < THREADS_NR; i++) {
        pthread_create(ids + i, NULL, start_routine2, &i);
    }
    for (i = 0; i < THREADS_NR; i++) {
        pthread_join(ids[i], &ret);
    }

    return ;
}

/*
 * 线程参数共享的问题
 */
static void pthread_argument_share_test(void)
{
    create_test_threads();
    return ;
}

/*
 * 因为pthread_join会阻塞知道线程退出, 所以这些线程实际上是串行执行的,
 * 一个退出了, 才创建下一个.
 */
static void create_test_threads2(void)
{
    int i = 0;
    void* ret = NULL;
    pthread_t ids[THREADS_NR] = { 0 };
    for (i = 0; i < THREADS_NR; i++) {
        pthread_create(ids + i, NULL, start_routine2, &i);
        pthread_join(ids[i], &ret);
    }

    return ;
}
/*
 * 虚假并发
 */
static void pthread_false_concurrent_test()
{
    create_test_threads2();
}
