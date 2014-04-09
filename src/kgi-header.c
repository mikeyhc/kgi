#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi.h>

/* kgi_add_header
 * adds a header to a kgi reply
 *
 * param kgi: the kgi to update
 * param name: the name of the header
 * param value: the value of the header
 * return: true if added else false
 *         failure indicates insufficient memory
 */
int kgi_add_header(struct kgi *kgi, char *name, char *value)
{
	int r;
	char *t;

	assert(kgi != NULL && name != NULL && value != NULL);

	t = malloc(strlen(name) + strlen(value) + 3); /* 3 : \0 */
	if(!t)
		return 0;
	sprintf(t, "%s: %s", name, value);
	r = arraylist_add(&kgi->headers, t);
	if(!r)
		free(t);
	return r;
}/* end: kgi_add_header */

static unsigned _count = 0;
/* cmp
 * compares to void pointers are strings up to _COUNT chars
 *
 * param a: first element to compare
 * param b: second element to compare
 * return: <0 if a>b, 0 if a==b, >0 if a<b
 */
static int cmp(void *a, void *b){
	return strncmp((char*)a,(char*)b,_count);
}/* end: cmp */

/* kgi_remove_header
 * removes a header from the kgi reply
 *
 * param kgi: the kgi to update
 * param name: the name of the header
 */
void kgi_remove_header(struct kgi *kgi, char *name)
{
	int idx;

	assert(kgi != NULL && name != NULL);

	_count = strlen(name);
	idx = arraylist_indexof(&kgi->headers, name, cmp);
	if(idx<0)
		return;
	free(arraylist_get(&kgi->headers, idx));
	arraylist_removeat(&kgi->headers, idx);
}/* end: kgi_remove_header */

/* kgi_clear_headers
 * removes all headers from the kgi
 *
 * param kgi: the kgi to clear
 */
void kgi_clear_headers(struct kgi *kgi)
{
	int i, size;
	void *ele;

	assert(kgi != NULL);

	size = kgi->headers._pos;
	for(i=0; i<size; i++)
		if((ele = arraylist_removeat(&kgi->headers, i)))
			free(ele);
}/* end: kgi_clear_headers */

/* kgi_output_headers
 * prints all the headers contained in the kgi to the given stream
 *
 * param kgi: the kgi to print from
 * param stream: the stream to print to
 */
void kgi_output_headers(struct kgi *kgi, FILE *stream)
{
	int i, j, len;
	void *e;
	
	assert(kgi != NULL);

	len = arraylist_size(&kgi->headers);
	for(i=j=0; j<len; i++)
		if((e = arraylist_get(&kgi->headers, i)))
			fprintf(stream, "%s\n", (char*)e), j++;
}/* end: kgi_output_headers */
