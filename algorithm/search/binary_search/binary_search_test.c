#include <stdio.h>

static void binary_search_test();
static int binary_search(int key, int arr[], int length);
static int binary_search_helper(int key, int arr[], int start_index, int end_index);
static void binary_search_test_debug_stat();
int main(int argc, char **argv)
{
	printf("binary search test\n");
	binary_search_test();
	return 0;
}

static void binary_search_test()
{
	int key = 999;
	int arr[1024];
	int i;
	int res;

	for (i = 0; i < 1024; i++) {
		arr[i] = i;
	}

	res = binary_search(key, arr, 1024);
	if (res == -1) {
		printf("key is not found\n");
	} else {
		printf("key is found in arr[%d]\n", arr[res]);
	}
}

static int binary_search(int key, int arr[], int length)
{
	return binary_search_helper(key, arr, 0, length - 1);
}

static int binary_search_helper(int key, int arr[], int start_index, int end_index)
{
	int middle;

	if (start_index > end_index) {
		return -1;
	}

	binary_search_test_debug_stat();
	middle = (start_index + end_index) / 2;
	if (key == arr[middle]) {
		return middle;
	} else if (key < arr[middle]) {
		return binary_search_helper(key, arr, start_index, middle - 1);
	} else {
		return binary_search_helper(key, arr, middle + 1, end_index);
	}
}

static void binary_search_test_debug_stat()
{
	static int count = 0;

	printf("%d: %s()\n", count, __func__);
	count++;
}
