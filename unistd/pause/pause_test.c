#include <unistd.h> /* pause */
#include <stdio.h>
#include <signal.h> /* signal */

static void signal_handler(int signal);
static void signal_init(void);
int main(int argc, char **argv)
{
    printf("hello\n");
    signal_init();
    while (1) {
        int ret = pause();
        printf("ret = %d\n", ret);
    }
    return 0;
}

static void signal_handler(int signal)
{
    printf("%s():%d signal=%d\n", __func__, __LINE__, signal);
}

static void signal_init(void)
{
    signal(SIGHUP, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGILL, signal_handler);
    signal(SIGTRAP, signal_handler);
    signal(SIGABRT, signal_handler);
    signal(SIGIOT, signal_handler);
    signal(SIGBUS, signal_handler);
    signal(SIGFPE, signal_handler);
    signal(SIGKILL, signal_handler);
    signal(SIGUSR1, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGUSR2, signal_handler);
    signal(SIGPIPE, signal_handler);
    signal(SIGALRM, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGSTKFLT, signal_handler);
    signal(SIGCHLD, signal_handler);
    signal(SIGCONT, signal_handler);
    signal(SIGSTOP, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGTTIN, signal_handler);
    signal(SIGTTOU, signal_handler);
    signal(SIGURG, signal_handler);
    signal(SIGXCPU, signal_handler);
    signal(SIGXFSZ, signal_handler);
    signal(SIGVTALRM, signal_handler);
    signal(SIGPROF, signal_handler);
    signal(SIGWINCH, signal_handler);
    signal(SIGIO, signal_handler);
    signal(SIGPOLL, signal_handler);
    signal(SIGPWR, signal_handler);
    signal(SIGSYS, signal_handler);
    signal(SIGUNUSED, signal_handler);
    /*signal(SIGSWI, signal_handler); *//* cenos 7 undeclared */
}