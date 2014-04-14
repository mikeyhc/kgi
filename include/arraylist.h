#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

struct arraylist {
	unsigned _array_size;
	unsigned _pos;
	unsigned size;
	void **list;
};

int arraylist_init(struct arraylist*);
void arraylist_destroy(struct arraylist*);
void arraylist_destroy_free(struct arraylist*);
int arraylist_add(struct arraylist*,void*);
int arraylist_set(struct arraylist*,unsigned,void*);
void *arraylist_get(struct arraylist*,unsigned);
void *arraylist_find(struct arraylist*,const void*,
		int(*)(const void*,const void*));
int arraylist_size(struct arraylist*);
int arraylist_maxidx(struct arraylist*);
int arraylist_indexof(struct arraylist*,const void*,
		int(*)(const void*,const void*));
int arraylist_contains(struct arraylist*,const void*,
		int(*)(const void*,const void*));
int arraylist_remove(struct arraylist*,const void*,
		int(*)(const void*,const void*));
void *arraylist_removeat(struct arraylist*,unsigned);
int arraylist_copy(struct arraylist*,struct arraylist*);

#endif
