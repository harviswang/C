/*
 * label用法goto label或者取label的地址
 * local label 作用域为一个语句块, 必须在一个语句块的最开始.
 * 使用方法:{__label__ label; ..... label: ....}
 * ordianry label 作用域为一个函数
 */
#include <stdio.h>


static void local_label_test();
int main(int argc, char **argv)
{
    printf("label keyword test ...\n");
    
    local_label_test();
    return 0;
}

/*
 * Return current * instruction pointer ("parogram counter")
 */
#define current_text_addr() ({__label__ _l; _l: &&_l;})

#define SEARCH(max, array, target)                       \
do {                                                     \
    __label__ found;                                     \
    typeof (target) _SEARCH_target = (target);           \
    typeof (*(array)) *_SEARCH_array = (array);          \
    int i, j;                                            \
    for (i = 0; i < max; i++) {                          \
        for (j = 0; j < max; j++) {                      \
            if (_SEARCH_array[i][j] == _SEARCH_target) { \
                (value) = i;                             \
                goto found ;                             \
            }                                            \
            (value) = -1;                                \
        }                                                \
    }                                                    \
    found: ;                                             \
} while (0)
        
#define SEARCH2(max, array, target)                      \
({                                                       \
    __label__ found;                                     \
    typeof (target) _SEARCH_target = (target);           \
    typeof (*(array)) *_SEARCH_array = (array);          \
    int i, j;                                            \
    int value;                                           \
    for (i = 0; i < max; i++) {                          \
        for (j = 0; j < max; j++) {                      \
            if (_SEARCH_array[i][j] == _SEARCH_target) { \
                (value) = i;                             \
                goto found ;                             \
            }                                            \
            (value) = -1;                                \
        }                                                \
    }                                                    \
    found:                                               \
        value;                                           \
})

static void local_label_test()
{
    int table[2][2] = {{1, 2}, {4, 5}};
    int value;
    printf("PC = 0x%lx\n", (unsigned long)current_text_addr());

    SEARCH(2, table, 5);
    printf("value = %d\n", value);
    
    printf("SEARCH2(2, table, 5) = %d\n", SEARCH2(2, table, 5));
}