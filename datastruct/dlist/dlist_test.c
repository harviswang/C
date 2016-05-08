#include <stdio.h>
#include <string.h>

static void dlist_test(void);
int main(int argc, char **argv)
{
	dlist_test();
	return 0;
}

struct dlist {
	struct dlist *prev;
	struct dlist *next;
	void *data;
	unsigned int length;
};

static int dlist_init(struct dlist *dl, void *data, unsigned int length)
{
	if (dl == NULL) {
		return -1;
	} else {
		dl->prev = dl;
		dl->next = dl;
		dl->data = data;
		dl->length = length;

		return 0;
	}
}

/*
 * dl <-> item <-> dl->next
 */
static int dlist_add(struct dlist *dl, struct dlist *item)
{
	if (dl == NULL || item == NULL) {
		return -1;
	} else {
		item->prev = dl;
		item->next = dl->next;
		dl->next->prev = item;
		dl->next = item;

		return 0;
	}
}

/*
 * dl->prev <-> item <-> dl
 */
static int dlist_add_tail(struct dlist *dl, struct dlist *item)
{
	return dlist_add(dl->prev, item);
}

static int dlist_dump(struct dlist *dl)
{
	if (dl == NULL) {
		return -1;
	} else {
		struct dlist *item;

		item = dl;
		do {
			printf("data: %s\n", (char *)item->data);	
			printf("length: %d\n", item->length);	
			item = item->next;
		} while (item != dl);

		return 0;
	}
}
static void dlist_test(void)
{
	struct dlist dl;
	struct dlist item;
	struct dlist arr[sys_nerr];
	int i;

	char str[] = "good luck";
	char str2[] = "bad luck";

	dlist_init(&dl, str, sizeof(str));
	dlist_init(&item, str2, sizeof(str2));
	dlist_add_tail(&dl, &item);

	for (i = 0; i < sys_nerr; i++) {
		dlist_init(&arr[i], (void *)sys_errlist[i], strlen(sys_errlist[i]) + 1);
		dlist_add_tail(&dl, &arr[i]);
	}
	dlist_dump(&item);
}
