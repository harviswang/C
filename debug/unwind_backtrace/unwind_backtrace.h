#ifndef UNWIND_BACKTRACE_H
#define UNWIND_BACKTRACE_H

/*
 * 打开"/system/lib/libcorkscrew.so"提取其中的so,搜索需要的函数
 */
int getCallStack(void);

/*
 * 动态链接"/system/lib/libcorkscrew.so"
 */
void do_backtrace(void);

#endif /* #ifndef UNWIND_BACKTRACE_H */