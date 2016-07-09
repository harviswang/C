#include <stdio.h>
#include <pthread.h>
#include <assert.h>
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

void* start_routine(void* param)
{
    char* str = (char*)param;
    printf("%s:%s\n", __func__, str);
    return NULL;
}

pthread_t create_test_thread()
{
    pthread_t id = 0;
    char str[] = "is is ok";
    pthread_create(&id, NULL, start_routine, str);
    return id;
}

int main(int argc, char **argv)
{
    void* ret = NULL;
    pthread_t id = create_test_thread();
    pthread_join(id, &ret);
    
    return 0;
}
