//���� : Damien Diederen<dd@crosstwine.com>
//��C����ģ��lisp�бհ�(closure) 
//������,���ڽػ�һ�ַ���,��һ�������ַ�(@��:)������Ǹ��ַ�
//intptr_t��ͷ�ļ�stdint.h�ж���, "typedef unsigned int uintptr_t;" 
//
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct closure {
        void *fn;
        void *data;
};

typedef struct closure (* state_fn) (void *d, int c);

struct closure make_closure (state_fn f, void *d) {
        struct closure c = { f, d };
        return c;
}

struct closure before_special (void *d, int c);
struct closure seen_special (void *d, int c);

struct closure before_special (void *d, int c) {
        switch (c) {
        case '@':
        case ':':
                return make_closure(seen_special, (void*)(intptr_t)c);
        case '\0':
                return make_closure(0, 0);
        default:
                return make_closure(before_special, 0);
        }
}

struct closure seen_special (void *d, int c) {
        int special_c = (intptr_t)d;

        printf("After special %c; char: %c\n", special_c, c);

        return make_closure(before_special, 0);
}

int main (int argc, char **argv) {
        struct closure c = make_closure(before_special, 0);
        char *v = argv[1];

        if (argc < 2) {
                printf("usage:\n");
                printf("%s ab:c@9\n", argv[0]);
                exit(-1);
        }

        while (c.fn) {
                state_fn f = c.fn;
                c = f(c.data, *v++);
        }

        return 0;
}
