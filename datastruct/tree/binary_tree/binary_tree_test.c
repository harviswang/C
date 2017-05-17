#include <stdio.h>

/*
 * map an array to binary tree
 *                 ------------------------      
 *                 | 1  |  1   |  1  |  1 |    ----> array item
 *                 ------------------------
 *                  /      |       |     \
 *                 /       |       |      \
 *          ----------------------------------
 *  0       | 0  | 1 | 2 | 1 | 4 | 1 | 2 | 1 | ----> binary tree item, 1 means actual node(come from array), non-1 means non-leaf node
 *          ----------------------------------
 *                 \      /         \      /
 *                  \    /           \    /
 *  1                  +               +
 *                      \             /
 *                       \           /
 *                        \         /
 *                         \       /
 *                          \     /
 *                           \   /
 *  2                          +
 */

/*
 * f(array item index i) => binary tree item index  
 */
static inline int crush_calc_tree_node(int i)
{
        return ((i+1) << 1)-1;
}


/*
 * n: non-0, the index in binary item array, index 0 is not used
 *    when n is 1,3,5.. height(n)=0, they are leaf node
 */
static int height(int n) {
	int h = 0;
	while ((n & 1) == 0) {
		h++;
		n = n >> 1;
	}
	return h;
}

/*
 * check node at index n is right node or not
 * n > 0
 * h = height(n)
 */
static int on_right(int n, int h) {
	return n & (1 << (h+1));
}

/*
 * get node's parent
 */
static int parent(int n) {
	int h = height(n);
	if (on_right(n, h)) {
		return n - (1<<h);
	} else {
		return n + (1<<h);
	}
}

/*
 * depth of binary tree, which is generate by an array sized size 
 * size is not the size of binary tree
 */
static int calc_depth(int size)
{
	if (size == 0) {
		return 0;
	}

	int depth = 1;
	int t = size - 1;
	while (t) {
		t = t >> 1;
		depth++;
	}
	return depth;
}

int main()
{
	int node[] = {1, 2, 3, 4, 5, 6, 7};
	int size = sizeof(node)/sizeof(node[0]);
	int i;

	for (i = 0; i < size; i++) {
		printf("height(%d) = %d\n", node[i], height(node[i]));
	}
	printf("------------------\n");
	for (i = 0; i < size; i++) {
		printf("parent(%d) = %d\n", node[i], parent(node[i]));
	}
	printf("------------------\n");
	for (i = 0; i < size; i++) {
		printf("calc_depth(%d) = %d\n", node[i], calc_depth(node[i]));
	}
	return 0;
}
