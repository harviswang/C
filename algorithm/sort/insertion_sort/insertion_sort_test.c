#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static void insertion_sort(int arr[], int length)
{
	int i, j, k;
	int sp = 0; /* sort point */
	int tmp;
	for (i = 1; i < length; i++) {
		for (j = 0; j < i; j++) {
			if (arr[i] < arr[j]) {
				tmp = arr[i];
				for (k = i; k > j; k--) {
					arr[k] = arr[k-1];
				}
				arr[sp] = tmp;
				sp = i;
				break;
			}
		}
	}
}


int main()
{
	int *reverse;
	int weights[] = {-2, 4, 8, -9, 22};
	int size = sizeof(weights)/sizeof(weights[0]);
	int i, j, k;

       /* reverse sort by weight (simple insertion sort) */
        reverse = malloc(sizeof(int) * size);
        if (!reverse)
                return -ENOMEM;
        if (size)
                reverse[0] = 0;
        for (i=1; i<size; i++) {
                for (j=0; j<i; j++) {
                        if (weights[i] < weights[reverse[j]]) {
                                /* insert here */
                                for (k=i; k>j; k--)
                                        reverse[k] = reverse[k-1];
                                reverse[j] = i;
                                break;
                        }
                }
                if (j == i)
                        reverse[i] = i;
        }

	for (i = 0; i < size; i++) {
		printf("reverse[%d] = %d\n", i, reverse[i]);
	}
	free(reverse);
	insertion_sort(weights, size);
	for (i = 0; i < size; i++) {
		printf("weights[%d] = %d\n", i, weights[i]);
	}
	return 0;
}
