#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi.h>

#define CONTENT_TYPE	"Content-type: text/html; charset=iso-8839-1\n"

struct param {
	char *key;
	struct arraylist val;
};

static int get_param_init  = 0;
static int post_param_init = 0;
static struct arraylist post_param;
static struct arraylist get_param;

/* get_code
 * returns the message associated with a particular HTTP code
 *
 * param code: the code to fetch
 * return: the HTTP message (defaults to 200 OK)
 */
static char *get_code(unsigned code)
{
	switch(code){
		case 302: return "302 Found";
		case 404: return "404 Not Found";
	}
	return "200 OK";
}/* end: get_code */

/* kgi_init
 * initialize a kgi struct
 *
 * param k: the kgi to initialize
 */
void kgi_init(struct kgi *k)
{
	assert(k != NULL);

	k->status = 200;
	arraylist_init(&k->cookies);
	arraylist_init(&k->data);
	arraylist_init(&k->headers);
}/* end: kgi_init */

/* kgi_destroy
 * clears and zeros a kgi struct (list components handled externally, may
 * not be zero'd)
 *
 * param k: the kgi to destroy
 */
void kgi_destroy(struct kgi *k)
{
	assert(k != NULL);

	k->status = 0;
	kgi_clear_cookies(k);
	arraylist_destroy(&k->cookies);
	kgi_clear_data(k);
	arraylist_destroy(&k->data);
	kgi_clear_headers(k);
	arraylist_destroy(&k->headers);
}/* end: kgi_destroy */

/* kgi_set_status
 * sets the HTTP status code
 *
 * param kgi: the kgi to update
 * param status: the status to set
 */
void kgi_set_status(struct kgi *kgi, unsigned status)
{
	assert(kgi != NULL);
	kgi->status = status;
}/* end: kgi_set_status */

/* kgi_get_status
 * returns the HTTP status code
 *
 * param kgi: the kgi to get the code from
 * return: the HTTP status code
 */
unsigned kgi_get_status(struct kgi *kgi)
{
	assert(kgi != NULL);
	return kgi->status;
}

/* kgi_output
 * outputs the given kgi to the given stream, no error checking performed
 *
 * param kgi: the kgi to output
 * param stream: the stream to output to
 */
void kgi_output(struct kgi *kgi, FILE *stream)
{
	assert(kgi != NULL);

	fprintf(stream, "Status: %s\n", get_code(kgi_get_status(kgi)));
	fprintf(stream, CONTENT_TYPE);
	kgi_output_headers(kgi, stream);
	kgi_output_cookies(kgi, stream);
	fprintf(stream, "Content-Length: %d\n\n", kgi_size_data(kgi));
	kgi_output_data(kgi, stream);
}/* end: kgi_output */

/* read_till
 * moves the pointer till the character is found and removed or EOF is 
 * reached
 *
 * param str: the pointer to operate on
 * param c: the character to read till
 */
static void read_till(char **str, char c)
{
	assert(str != NULL && *str != NULL);

	while(**str != '\0' && **str != c)
		(*str)++;
}/* end: read_till */

/* param_key_cmp
 * compares a string to the key value in a param struct
 *
 * param param: the param struct to compare against
 * param key: the key to check for
 * return: <0 if param->key > key, 0 if equal, >0 if key >
 */
int param_key_cmp(const void *param, const void *key)
{
	return strcmp(((struct param*)param)->key, key);
}/* end: postparam_key_cmp */

/* store_query_string
 * breaks up QUERY and stores it in LIST
 *
 * param query: the query string to break up
 * param list: the list to store it in
 */
void store_query_string(char *query, struct arraylist *list)
{
	char *key, *val, *tkey, *tval;
	struct param *param;
	unsigned long keylen, vallen;
	int idx;

	/* NB: this need some serious attention */
	while(*query){
		key = query;
		read_till(&query, '=');
		keylen = query - key;
		if(*query && *(query+1)){
			val = ++query;
			read_till(&query, '&');
			vallen = query - val;
			if(vallen && keylen){
				tkey = malloc(keylen + 1);
				tval = malloc(vallen + 1);
				if(!tkey || !tval){
					if(tkey)
						free(tkey);
					return;
				}
				strncpy(tkey, key, keylen);
				tkey[keylen] = '\0';
				/* check if already have a param struct */
				if((idx = arraylist_indexof(list, key,
						param_key_cmp)) < 0){
					param = malloc(sizeof(*param));
					if(!param){
						free(tkey);
						free(tval);
						return;
					}
					param->key = tkey;
					arraylist_init(&param->val);
					arraylist_add(list, param);
					idx = arraylist_size(list) - 1;
				}else
					free(tkey);

				strncpy(tval, val, vallen);
				tval[vallen] = '\0';
				arraylist_add((struct arraylist*)
						&((struct param*)
						arraylist_get(list, idx))
						->val, tval);
			}
		}
		
		/* eat up to and including & or \0 */
		if(*query){
			read_till(&query, '&');
			if(*query)
				query++;
		}
	}
}/* end: store_query_string */

/* init_getparam
 * initializes the get_param array
 */
void init_getparam(void)
{
	char *qs;

	if(get_param_init)
		return;
	arraylist_init(&get_param);
	get_param_init = 1;
	if((qs = getenv("QUERY_STRING")))
		store_query_string(qs, &get_param);
}/* end: init_getparam */

/* init_postparam
 * initializes the post_param array
 */
void init_postparam(void)
{
	char *content, *t;
	unsigned long len;

	if(post_param_init)
		return;
	arraylist_init(&post_param);
	post_param_init = 1;
	t = getenv("CONTENT_LENGTH");
	if(!t)
		return;
	content = malloc((len = atoi(t) + 1));
	if(!content)
		return;
	fgets(content, len, stdin);
	store_query_string(content, &post_param);
}/* end: init_postparam */

/* kgi_get_param
 * searches the query string for a given key, if found returns the value 
 * in the given array
 *
 * param key: the key to look for
 * param list: an uninitialised arraylist to store values in
 * return: the number of values found
 */
unsigned kgi_get_param(const char *key, struct arraylist *list)
{
	void *val;

	assert(key && list);

	init_getparam();
	val = arraylist_find(&get_param, key , param_key_cmp);
	if(!val)
		return 0;
	arraylist_copy(list, &((struct param*)val)->val);
	return arraylist_size(list);
}/* end: kgi_get_param */

/* kgi_post_param
 * searches through stdin for a given key, if found returns the value in
 * the given array
 *
 * param key: the key to look for
 * param list: an uninitialized arraylist to store the values in
 * return: the number of values found
 */
unsigned kgi_post_param(const char *key, struct arraylist *list)
{
	void *val;

	assert(key && list);

	init_postparam();
	val = arraylist_find(&post_param, key, param_key_cmp);
	if(!val)
		return 0;
	arraylist_copy(list, &((struct param*)val)->val);
	return arraylist_size(list);
}/* end: kgi_post_param */

/* kgi_post_boundary_param
 * searchs through stdin using the given boundary to delimit values and 
 * keys, if the key is found then the matched value is returned in the 
 * given buffer
 *
 * param key: the key to look for
 * param bound: the boundary string to use
 * param list: au uninitialised arraylist to store the values in
 * return: the number of values found
 */
unsigned kgi_post_boundary_param(const char *key, const char *bound, 
		struct arraylist *list)
{
	fprintf(stderr, "%s is a stub\n", __func__);
	return 0;
}/* end: kgi_post_boundary_param */
