#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi.h>

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
static int check_val(void *a, void *b){
	return strncmp((char*)a, (char*)b, _count);
}

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
}

void kgi_destroy_cookie(struct kgi *kgi, char *name)
{

}

void kgi_clear_cookies(struct kgi *kgi)
{

}
