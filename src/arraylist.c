#include <stdlib.h>

#include <arraylist.h>

void arraylist_init(struct arraylist *list)
{

}

void arraylist_destroy(struct arraylist *list)
{

}

void arraylist_add(struct arraylist *list, void *ele)
{

}

void arraylist_set(struct arraylist *list, int idx, void *ele)
{

}

void *arraylist_get(struct arraylist *list, int idx)
{

	return NULL;
}

int arraylist_size(struct arraylist *list)
{

	return -1;
}

int arraylist_indexof(struct arraylist *list, void *ele, 
		int (*cmp)(void*,void*))
{
	return -1;
}

int arraylist_contains(struct arraylist *list, void *ele,
		int (*cmp)(void*,void*))
{
	return -1;
}

int arraylist_remove(struct arraylist *list, void *ele,
		int (*cmp)(void*,void*))
{
	return -1;
}
