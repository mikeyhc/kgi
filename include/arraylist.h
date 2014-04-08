#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

struct arraylist {
	int _array_size;
	int _pos;
	int _max;
	void *list;
};

void arraylist_init(struct arraylist*);
void arraylist_destroy(struct arraylist*);
void arraylist_add(struct arraylist*, void*);
void arraylist_set(struct arraylist*, int, void*);
void *arraylist_get(struct arraylist*, int);
int arraylist_size(struct arraylist*);
int arraylist_indexof(struct arraylist*,void*,int(*)(void*,void*));
int arraylist_contains(struct arraylist*,void*,int(*)(void*,void*));
int arraylist_remove(struct arraylist*,void*,int(*)(void*,void*));

#endif
