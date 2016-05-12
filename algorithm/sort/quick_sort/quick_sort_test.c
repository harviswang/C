#include <stdio.h>

static void quick_sort_test();
int main(int argc, char **argv)
{
    quick_sort_test();
    return 0;
}

static void array_dump(int arr[], int length)
{
    int i = 0;
    
    printf("%d", arr[i]);
    for (i = 1; i < length; i++) {
        printf(" %d", arr[i]);
    }
}

static void swap(int *one, int *two)
{
    int temp = *one;
    *one = *two;
    *two = temp;
}
static int move_right2left(int arr[], int low, int high);
static void quick_sort_helper(int arr[], int start, int end)
{
    int m;

    if (start >= end) {
        return;
    }

    m = move_right2left(arr, start, end);
    printf("m = %d\n", m);
    quick_sort_helper(arr, start, m - 1);
    quick_sort_helper(arr, m + 1, end);
}

static void quick_sort(int arr[], int length)
{
    quick_sort_helper(arr, 0, length - 1);
    //quick_sort(&arr[0], i + 1);
    //quick_sort(&arr[m], length - m + 1);
    
}

static int move_left2right(int arr[], int low, int high)
{
    int index = high;
    if (low == high) {
        printf("index = %d\n", index);
        return index;
    }
    printf("index=%d low=%d high=%d\n", index, low, high);

    if (arr[index] < arr[low]) {
        swap(&arr[index], &arr[low]);
        return move_right2left(arr, low, high-1);
    } else {
        return move_left2right(arr, low + 1, high);
    }
}

/*
 * 从最右边往左边,找到第一个比arr[index]小的数据,交换,然后从最左边开始找第一个比arr[index]大的数
 * 一次快速排序的示例, 5左边的均比它小, 5右边的均比它大
 * <- sort: 5 1 2 9 3 8 0 4 6 7
 * <- sort: 4 1 2 9 3 8 0 5 6 7
 * <- sort: 4 1 2 5 3 8 0 9 6 7
 * <- sort: 4 1 2 0 3 8 5 9 6 7
 * <- sort: 4 1 2 0 3 5 8 9 6 7
 */
static int move_right2left(int arr[], int low, int high)
{
    int index = low;

    if (low == high) {
        printf("index2 = %d\n", index);
        return index;
    }
    printf("index=%d low=%d high=%d\n", index, low, high);
    if (arr[index] > arr[high]) {
        swap(&arr[index], &arr[high]);
        return move_left2right(arr, low+1, high);
    } else {
        return move_right2left(arr, low, high-1);
    }
}

static void quick_sort_test()
{
    int arr[] = {1, 5, 2, 9, 3, 8, 0, 4, 6, 7};
    
    printf("<- sort: ");
    array_dump(arr, sizeof(arr)/sizeof(arr[0]));
    printf("\n");
    quick_sort(arr, sizeof(arr)/sizeof(arr[0]));
    //move_left_right(arr, 0, 9);
    printf("-> sort: ");
    array_dump(arr, sizeof(arr)/sizeof(arr[0]));
    printf("\n");
}
