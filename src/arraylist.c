#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>	/* memset */

#include <arraylist.h>

#define INIT_SIZE 10

static uint8_t _ensure_capacity(struct arraylist*,unsigned);

/* arraylist_init
 * initializes and arraylist structure
 *
 * param list: the list to initialize
 * return: 1 if initialized else 0
 *         failure indicates insufficient memory
 */
uint8_t arraylist_init(struct arraylist *list)
{
	assert(list);

	list->list = calloc(INIT_SIZE, sizeof(*list->list));
	if(!list->list)
		return 0;
	list->_array_size = INIT_SIZE;
	list->_pos = 0;
	list->size = 0;

	return 1;
}/* end: arraylist_init */

/* arraylist_destroy
 * frees any resources held by an arraylist structure
 * NB: does not zero list elements
 *
 * param list: the list to destroy
 */
void arraylist_destroy(struct arraylist *list)
{
	assert(list && list->list);

	free(list->list);
	list->_array_size = 0;
	list->_pos = 0;
	list->size = 0;
}/* end: arraylist_destroy */

/* arraylist_destroy_free
 * calls arraylist_destroy_freewith with free
 *
 * param list: the list to destroy
 */
void arraylist_destroy_free(struct arraylist *list)
{
	arraylist_destroy_freewith(list,free);
}/* end: arraylist_destroy_free */


/* arraylist_destroy_freewith
 * calls the given method on all non-NULL elements in the list then sets
 * them to NULL then calls destroy on the list
 *
 * param list: thest list to free
 * param func: the function to call
 */
void arraylist_destroy_freewith(struct arraylist *list, 
		void (*func)(void*))
{
	unsigned long i;

	assert(list && list->list && func);

	for(i=0; i<list->_pos; i++)
		if(list->list[i] != NULL){
			func(list->list[i]);
			list->list[i] = NULL;
		}
	arraylist_destroy(list);
}/* end: arraylist_destroy_freewith */

/* arraylist_add
 * attempts to add ELE to LIST
 *
 * param list: the list to add to
 * param ele: the element to add to the list
 * return: 1 if it was added else 0 
 *         failure indicates insufficient memory
 */
uint8_t arraylist_add(struct arraylist *list, void *ele)
{
	assert(list);

	return arraylist_set(list, list->_pos, ele);
}/* end: arraylist_add */

/* arraylist_set
 * attempts to IDX in LIST to ELE
 *
 * param list: the list to set the element in
 * param idx: the index to update
 * param ele: the element to add
 * return: 1 to add else 0
 *         failure indicates insufficient memory
 */
uint8_t arraylist_set(struct arraylist *list, unsigned long idx, void *ele)
{
	assert(list && list->list && ele);

	if(!_ensure_capacity(list, idx))
		return 0;
	list->list[idx] = ele;
	list->size++;
	list->_pos = idx+1;
	return 1;
}/* end: arraylist_set */

/* arraylist_get
 * returns the element at a given index
 *
 * param list: the list to search
 * param idx: the index to fetch from
 * return: either the element or NULL if there was none
 */
void *arraylist_get(const struct arraylist *list, unsigned long idx)
{
	assert(list && list->list);
	if(idx < list->_array_size)
		return list->list[idx];
	return NULL;
}/* end: arraylist_get */

/* arraylist_find
 * returns the element that matches ELE using CMP to compare
 *
 * param list: the list to search
 * param ele: the element to match
 * param cmp: the comparator to use
 * return: the element if found otherwise NULL
 */
void *arraylist_find(const struct arraylist *list, const void *ele, 
		int (*cmp)(const void*,const void*)){
	long i;

	i = arraylist_indexof(list, ele, cmp);
	if(i<0)
		return NULL;
	return arraylist_get(list, i);
}/* end: arraylist_find */

/* arraylist_size
 * returns the number of elements in the arraylist
 *
 * param list: the list to return the size of
 * return: the size of the list
 */
unsigned long arraylist_size(const struct arraylist *list)
{
	assert(list);
	return list->size;
}/* end: arraylist_size */

/* arraylist_maxidx
 * returns the highest possible index that might be in use
 *
 * param list: the list to return the index from
 * return: the highest index that might be in use
 */
size_t arraylist_maxidx(const struct arraylist *list)
{
	assert(list);
	return list->_pos;
}/* end: arraylist_maxidx */

/* arrraylist_indexof
 * returns the index of the ELE in LIST using CMP to compare
 *
 * param list: the list to search
 * param ele: the element to look for
 * param cmp: the comparator to use
 * return: the index if found else -1
 */
long arraylist_indexof(const struct arraylist *list, const void *ele, 
		int (*cmp)(const void*,const void*))
{
	unsigned i;
	
	assert(list && list->list && ele && cmp);

	for(i = 0; i < list->_pos; i++)
		if(!cmp(list->list[i],ele))
			return i;
	return -1;
}/* end: arraylist_indexof */

/* arraylist_contains
 * searches LIST for ELE using CMP to compare
 *
 * param list: the list to search
 * param ele: the element to search for
 * param cmp: the comparator to use
 * return: true if the element was found else false
 */
uint8_t arraylist_contains(const struct arraylist *list, const void *ele,
		int (*cmp)(const void*,const void*))
{
	return arraylist_indexof(list, ele, cmp) != -1;
}/* end: arraylist_contains */

/* arraylist_remove
 * attempts to remove ELE from LIST using CMP to find it
 *
 * param list: the list to remove from
 * param ele: the element to remove
 * param cmp: the comparator to use
 * return: true if removed else false
 */
uint8_t arraylist_remove(struct arraylist *list, const void *ele,
		int (*cmp)(const void*,const void*))
{
	int idx;

	idx = arraylist_indexof(list, ele, cmp);
	if(idx >= 0)
		arraylist_removeat(list, idx);
	return idx >= 0;
}/* end: arraylist_remove */

/* arraylist_removeat
 * attempts to remove the item at IDX in LIST and return it
 *
 * param list: the list to remove from
 * param idx: the index to remove at
 * return: NULL or a pointer
 */
void *arraylist_removeat(struct arraylist *list, unsigned idx)
{
	void *ele;

	assert(list);

	if(idx >= list->_pos)
		return NULL;
	ele = list->list[idx];
	if(ele){
		list->list[idx] = NULL;
		list->size--;
	}
	return ele;
}/* end: arraylist_removeat */

/* _ensure_capacity
 * makes sure LIST is large enough to have an element at IDX, if it isn't
 * attempt to grow it
 *
 * param list: the list to check
 * param idx: the index to check
 * return: 1 if the list was large enough or was successfully resized else 0
 *         failure indicates insufficient memory
 */
static uint8_t _ensure_capacity(struct arraylist *list, unsigned idx)
{
	if(idx < list->_array_size)
		return 1;
	list->list = realloc(list->list, (list->_array_size << 1) * 
			sizeof(*list->list));
	if(!list->list)
		return 0;
	memset(list->list + list->_array_size, 0, list->_array_size);
	list->_array_size <<= 1;
	return 1;
}/* end: _ensure_capacity */

/* arraylist_copy
 * attempts to copy A to B, will override B so make sure its empty if you 
 * dont like memory leaks
 *
 * param a: the list to copy to
 * param b: the list to copy from
 * return: 1 on success else 0
 *         failure indicates insufficient memory
 */
uint8_t arraylist_copy(struct arraylist *a, struct arraylist *b)
{
	void **t;

	assert(a && b);

	t = malloc(sizeof(*t)*b->_array_size);
	if(!t)
		return 0;
	a->list = t;
	a->_array_size = b->_array_size;
	a->_pos = b->_pos;
	a->size = b->size;
	memcpy(a->list, b->list, b->_array_size * sizeof(*t));
	return 1;
}/* end: arraylist_copy */
