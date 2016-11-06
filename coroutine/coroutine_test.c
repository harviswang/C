/*
 * C coroutine sample
 * 协程(coroutine)顾名思义就是“协作的例程”（co-operative routines）
 * 跟具有操作系统概念的线程不一样，协程是在用户空间利用程序语言的语法语义就能实现逻辑上类似多任务的编程技巧。
 * 于协程来说，“挂起”的概念只不过是转让代码执行权并调用另外的协程，待到转让的协程告一段落后重新得到调用并从挂起点“唤醒”，
 * 这种协程间的调用是逻辑上可控的，时序上确定的，可谓一切尽在掌握中。
 * 
 * 协程的概念，如果将每个协程的上下文（比如程序计数器）保存在其它地方而不是堆栈上，协程之间相互调用时，
 * 被调用的协程只要从堆栈以外的地方恢复上次出让点之前的上下文即可，这有点类似于 CPU 的上下文切换.
 * 
 */
#include <stdio.h>

typedef struct 
{
    int i;
    int num;
    int state;
} task_t;

#define crBegin(state) \
    switch(state) { case 0:

#define crReturn(state, ret) \
    (state) = __LINE__; return (ret); case __LINE__:

#define crEnd() \
    }

int cb(task_t *t)
{
    crBegin(t->state);
    for (;;) {
        t->num = 1;
        for (t->i = 0; t->i < 20; t->i ++) {
            crReturn(t->state, t->num);
            t->num ++;
        }
    }
    crEnd();
    
    return -1;
}

int function(int max)
{
    static int i, state = 0;
    switch(state) {
        case 0: goto LABEL0;
        case 1: goto LABEL1;
    }
    LABEL0: /* start of function */
            for (i = 0; i < max; i++) {
                state = 1; /* so we will come back to LABEL1 */
                return i;
                LABEL1:; /* resume control straight after the return */
            }

    return -1;
}

int function2(int max)
{
    static int i, state = 0;
    switch(state) {
        case 0: /* start of function */
            for (i = 0; i < max; i++) {
                state = 1; /* so we will come back to LABEL1 */
                return i;
                case 1:; /* resume control straight after the return */
            }
    }

    return -1;
}

int function3(int max)
{
    static int i, state = 0;
    switch(state) {
        case 0: /* start of function */
            for (i = 0; i < max; i++) {
                state = __LINE__ + 2; /* so we will come back to LABEL1 */
                return i;
                case __LINE__:; /* resume control straight after the return */
            }
    }

    return -1;
}

#define Begin() static int state = 0; switch(state) { case 0:
#define Yield(x) do { state = __LINE__; return x; case __LINE__:; } while(0)
#define End() }

int function4(int max)
{
    static int i;

    Begin();
    for (i = 0; i < max; i++) {
        Yield(i);
    }
    End();

    return -1;
}



int main()
{
    task_t t;
    int i;
    
    t.state = 0;
    for (i = 0; i < 100; i++) {
        printf("%d ", cb(&t));
    }
    printf("\n");
    
    for (i = 0; i < 10; i++) {
        printf("%d ", function(10));
    }
    printf("\n");
    
    for (i = 0; i < 10; i++) {
        printf("%d ", function2(10));
    }
    printf("\n");
    
    for (i = 0; i < 10; i++) {
        printf("%d ", function3(10));
    }
    printf("\n");
 
    for (i = 0; i < 10; i++) {
        printf("%d ", function4(10));
    }
    printf("\n");
    return 0;
}