#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi.h>

#define COOKIE_DESTROY "; Expires=Thu, 01-Jan-1970 00:00:01 GMT;"
#define CD_LENGTH      strlen(COOKIE_DESTROY)

/* kgi_add_cookie
 * attempts to add a cookie to a kgi reply
 *
 * param kgi: the reply to add to
 * param name: the name of the cookie
 * param value: the value of the cookie
 * return: true if added else false
 *         failure occurs if there is insufficient memory
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

	assert(kgi != NULL);

	size = kgi->cookies._pos;
	for(i=0; i<size; i++)
		arraylist_removeat(&kgi->cookies, i);
}/* end: kgi_clear_cookies */
