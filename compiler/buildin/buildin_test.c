
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)


int main(int argc, char **argv)
{
    int a;

    /* Get the value from somewhere GCC can't optimize */
    a = atoi(argv[1]);

    if (likely (a == 2)) {
        a++;
    } else {
        a--;
    }

//    printf("%d\n", a);

    return a;
}
