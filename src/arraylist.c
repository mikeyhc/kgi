#include <assert.h>
#include <stdlib.h>
#include <string.h>	/* memset */

#include <arraylist.h>

#define INIT_SIZE 10

static int _ensure_capacity(struct arraylist*,unsigned);

/* arraylist_init
 * initializes and arraylist structure
 *
 * param list: the list to initialize
 * return: 1 if initialized else 0
 *         failure indicates insufficient memory
 */
int arraylist_init(struct arraylist *list)
{
	assert(list != NULL);

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
	assert(list != NULL && list->list != NULL);

	free(list->list);
	list->_array_size = 0;
	list->_pos = 0;
	list->size = 0;
}/* end: arraylist_destroy */


/* arraylist_add
 * attempts to add ELE to LIST
 *
 * param list: the list to add to
 * param ele: the element to add to the list
 * return: 1 if it was added else 0 
 *         failure indicates insufficient memory
 */
int arraylist_add(struct arraylist *list, void *ele)
{
	assert(list != NULL);

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
int arraylist_set(struct arraylist *list, unsigned idx, void *ele)
{
	assert(list != NULL && list->list != NULL && ele != NULL);

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
void *arraylist_get(struct arraylist *list, unsigned idx)
{
	assert(list != NULL && list->list != NULL);
	if(idx < list->_array_size)
		return list->list[idx];
	return NULL;
}/* end: arraylist_get */

/* arraylist_size
 * returns the number of elements in the arraylist
 *
 * param list: the list to return the size of
 * return: the size of the list
 */
int arraylist_size(struct arraylist *list)
{
	assert(list != NULL);
	return list->size;
}/* end: arraylist_size */

/* arrraylist_indexof
 * returns the index of the ELE in LIST using CMP to compare
 *
 * param list: the list to search
 * param ele: the element to look for
 * param cmp: the comparator to use
 * return: the index if found else -1
 */
int arraylist_indexof(struct arraylist *list, void *ele, 
		int (*cmp)(void*,void*))
{
	unsigned i;
	
	assert(list != NULL && list->list != NULL);
	assert(ele != NULL && cmp != NULL);

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
int arraylist_contains(struct arraylist *list, void *ele,
		int (*cmp)(void*,void*))
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
int arraylist_remove(struct arraylist *list, void *ele,
		int (*cmp)(void*,void*))
{
	int idx;

	idx = arraylist_indexof(list, ele, cmp);
	if(idx >= 0){
		list->list[idx] = NULL;
		list->size--;
	}
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

	assert(list != NULL);

	if(idx >= list->_pos)
		return NULL;
	ele = list->list[idx];
	if(ele)
		list->size--;
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
static int _ensure_capacity(struct arraylist *list, unsigned idx)
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
