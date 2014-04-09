#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi.h>

/* kgi_set_data
 * sets the data of the kgi response to the given value
 *
 * param kgi: the kgi to update
 * param data: the data to add
 * return: true if success else false
 *         failure indicates insufficient memory
 */
int kgi_set_data(struct kgi *kgi, char *data)
{
	kgi_clear_data(kgi);
	return kgi_add_data(kgi, data);
}/* end: kgi_set_data */

/* kgi_add_data
 * appends data to the end of the reply
 *
 * param kgi: the kgi to update
 * param data: the data to add
 * return: true if added else false
 *         failure indicates insufficient memory
 */
int kgi_add_data(struct kgi *kgi, char *data)
{
	char *t;
	int r;

	assert(kgi != NULL && data != NULL);

	t = malloc(strlen(data) + 1);
	strcpy(t, data);
	r = arraylist_add(&kgi->data, t);
	if(!r)
		free(t);
	return r;
}/* end: kgi_add_data */

/* kgi_clear_data
 * removes all the data from the reply
 *
 * param kgi: the kgi to clear
 */
void kgi_clear_data(struct kgi *kgi)
{
	unsigned i, size;
	void *ele;

	assert(kgi!=NULL);

	size = kgi->data._pos;
	for(i=0; i<kgi->data._pos; i++)
		if((ele = arraylist_removeat(&kgi->data, i)))
			free(ele);
}/* end: kgi_clear_data */
