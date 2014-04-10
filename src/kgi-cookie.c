#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi.h>

#define COOKIE_DESTROY	"; Expires=Thu, 01-Jan-1970 00:00:01 GMT;"
#define CD_LENGTH	strlen(COOKIE_DESTROY)
#define BUF_SIZE	256

/* kgi_add_cookie
 * attempts to add a cookie to a kgi reply
 *
 * param kgi: the reply to add to
 * param name: the name of the cookie
 * param value: the value of the cookie
 * return: true if added else false
 *         failure indicates insufficient memory
 */
int kgi_add_cookie(struct kgi *kgi, char *name, char *value)
{
	int r;
	char *t;

	assert(kgi != NULL && name != NULL && value != NULL);

	t = malloc(strlen(name) + strlen(value) + 2); /* 2 =\0 */
	if(!t)
		return 0;
	sprintf(t, "%s=%s", name, value);
	r = arraylist_add(&kgi->cookies, t);
	if(!r)
		free(t);
	return r;
}/* end: kgi_add_cookie */

/* read_till
 * moves the pointer till the character is found and removed or EOF is 
 * reached
 *
 * param str: the pointer to operate on
 * param c: the character to read till
 */
void read_till(char **str, char c)
{
	assert(str != NULL && *str != NULL);

	while(**str != '\0' && **str != c)
		(*str)++;
	if(**str)
		(*str)++;
}/* end: read_till */

/* kgi_get_cookie
 * attempts to read a cookie from the current environment
 * TODO: caching
 *
 * param name: the name of the cookie
 * return: the cookie value if found or NULL if not (or memory could not 
 *         be allocated)
 */
char *kgi_get_cookie(char *name)
{
	char *cookie, *tmp, *r;
	int len;

	tmp = getenv("HTTP_COOKIE");
	if(!tmp)
		return NULL;
	while(*tmp){
		cookie = tmp;
		read_till(&tmp, '=');
		if(!strncmp(name, cookie, tmp - cookie -1)){
			cookie = tmp;
			read_till(&tmp, ';');
			len = tmp - cookie - (*(tmp-1)==';' ? 1 : 0);
			r = malloc(len + 1);
			if(!r)
				return NULL;
			strncpy(r, cookie, len);
			r[len] = '\0';
			return r;
		}
		read_till(&cookie, ';');
	}
	return NULL;
}/* end: kgi_get_cookie */

static int _count = 0;
/* check_val
 * basic string comparator for a preset length
 *
 * param a: first value to compare
 * param b: second value to compare
 * return: <0 if a>b, 0 if a==b, >0 if a<b
 */
static int check_val(void *a, void *b){
	return strncmp((char*)a, (char*)b, _count);
}/* end: check_val */

/* kgi_remove_cookie
 * removes the given cookie from the kgi if found
 *
 * param kgi: the kgi to update
 * param name: the name of the cookie
 */
void kgi_remove_cookie(struct kgi *kgi, char *name)
{
	int idx;
	
	assert(kgi != NULL && name != NULL);

	_count = strlen(name);
	idx = arraylist_indexof(&kgi->cookies, name, check_val); 
	if(idx < 0)
		return;
	free(arraylist_get(&kgi->cookies, idx));
	arraylist_removeat(&kgi->cookies, idx);
}/* end: kgi_remove_cookie */

/* kgi_destroy_cookie
 * sets a blank cookie with a negative timestamp to rmeove the cookie from
 * the clients machine
 *
 * param kgi: the kgi to update
 * param name: the name of the cookie
 * return: true if set otherwise false
 */
int kgi_destroy_cookie(struct kgi *kgi, char *name)
{
	char *t;
	int r;

	assert(kgi != NULL && name != NULL);
	
	t = malloc(strlen(name) + CD_LENGTH + 2); /* 2 =\0 */
	if(!t)
		return 0;
	sprintf(t, "%s=;"COOKIE_DESTROY, name);
	r = arraylist_add(&kgi->cookies, t);
	if(!r)
		free(t);
	return r;
}/* end: kgi_destroy_cookie */

/* kgi_clear_cookies
 * removes all the cookies for the kgi
 *
 * param kgi: the kgi to clear
 */
void kgi_clear_cookies(struct kgi *kgi)
{
	int i, size;
	void *ele;

	assert(kgi != NULL);

	size = kgi->cookies._pos;
	for(i=0; i<size; i++)
		if((ele = arraylist_removeat(&kgi->cookies, i)))
			free(ele);
}/* end: kgi_clear_cookies */

/* kgi_output_cookies
 * prints all the cookies contained in the kgi to the give stream
 *
 * param kgi: the kgi to print from
 * param stream: the stream to print to
 */
void kgi_output_cookies(struct kgi *kgi, FILE *stream)
{
	int i, j, len;
	void *e;

	assert(kgi != NULL && stream != NULL);

	len = arraylist_size(&kgi->cookies);
	for(i=j=0; j < len; i++)
		if((e = arraylist_get(&kgi->cookies, i)))
			fprintf(stream, "Set-Cookie: %s\n", (char*)e), j++;
}/* end: kgi_output_cookies */
