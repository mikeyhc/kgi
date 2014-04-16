#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

#include <stdint.h>
#include <stdlib.h>

struct arraylist {
	size_t _array_size;
	unsigned long _pos;
	size_t size;
	void **list;
};

uint8_t arraylist_init(struct arraylist*);
void arraylist_destroy(struct arraylist*);
void arraylist_destroy_free(struct arraylist*);
void arraylist_destroy_freewith(struct arraylist*,void(*)(void*));
uint8_t arraylist_add(struct arraylist*,void*);
uint8_t arraylist_set(struct arraylist*,unsigned long,void*);
void *arraylist_get(struct arraylist*,unsigned long);
void *arraylist_find(struct arraylist*,const void*,
		int(*)(const void*,const void*));
size_t arraylist_size(struct arraylist*);
size_t arraylist_maxidx(struct arraylist*);
long  arraylist_indexof(struct arraylist*,const void*,
		int(*)(const void*,const void*));
uint8_t arraylist_contains(struct arraylist*,const void*,
		int(*)(const void*,const void*));
uint8_t arraylist_remove(struct arraylist*,const void*,
		int(*)(const void*,const void*));
void *arraylist_removeat(struct arraylist*,unsigned);
uint8_t arraylist_copy(struct arraylist*,struct arraylist*);

#endif
