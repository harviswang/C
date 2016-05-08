#include <stdio.h>

static void bubble_test(void);
int main(int argc, char **argv)
{
	bubble_test();
	return 0;
}

static void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static int bubble_sort(int arr[], const int len)
{
	int i, j;

	for (j = len - 1; j > 1; j--) {
		for (i = 0; i < j; i++) {
			if (arr[i] > arr[i+1]) {
				swap(&arr[i], &arr[i+1]);
			}
		}
	}

	return 0;
}

static void bubble_test(void)
{
	int i;
	int arr[] = {2, 9, 4, 7, 3, 100, 5};
	int len = sizeof(arr)/sizeof(arr[0]);

	printf("bubble sort before: ");
	for (i = 0; i < len; i++) {
		printf("%d\t", arr[i]);
	}
	printf("\n");

	bubble_sort(arr, sizeof(arr)/sizeof(arr[0]));

	printf("bubble sort after:  ");
	for (i = 0; i < len; i++) {
		printf("%d\t", arr[i]);
	}
	printf("\n");
}
