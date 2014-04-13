#include <assert.h>
#include <stdio.h>
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
int kgi_set_data(struct kgi *kgi, const char *data)
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
int kgi_add_data(struct kgi *kgi, const char *data)
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

/* kgi_size_data
 * returns the size of the data output (useful for content-length)
 *
 * param kgi: the kgi to check
 * return: the amount of data in the structure
 */
unsigned kgi_size_data(struct kgi *kgi)
{
	int i, j, len;
	unsigned r;
	void *e;

	assert(kgi != NULL);

	len = arraylist_size(&kgi->data);
	for(i=j=r=0; j<len; i++)
		if((e = arraylist_get(&kgi->data, i)))
			r += strlen((char*)e), j++;
	return r;
}/* end: kgi_size_data */

/* kgi_output_data
 * prints all the data contained in the kgi to the given stream
 *
 * param kgi: the kgi to print from
 * param stream: the stream to print to
 */
void kgi_output_data(struct kgi *kgi, FILE *stream)
{
	int i, j, len;
	void *e;

	assert(kgi != NULL && stream != NULL);

	len = arraylist_size(&kgi->data);
	for(i=j=0; j<len; i++)
		if((e = arraylist_get(&kgi->data, i)))
			fprintf(stream, (char*)e), j++;
}/* end: kgi_output_data */
