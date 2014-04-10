#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi.h>

#define CONTENT_TYPE	"Content-type: text/html; charset=iso-8839-1\n"

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

/* kgi_get_param
 * searches the query string for a given key, if found returns the value 
 * in the given array
 *
 * param key: the key to look for
 * param buf: the buffer to store in
 * param n: the length of BUF
 * param len: the number of characters written to BUF
 * return: a pointer to BUF if the key was found otherwise NULL
 */
char *kgi_get_param(const char *key, char *buf, unsigned n, unsigned *len)
{
	char *qs, *p;

	assert(buf);

	p = getenv("QUERY_STRING");
	while(*p){
		qs = p;
		read_till(&p, '=');
		if(*p && *(p+1) && !strncmp(key, qs, p-qs)){
			qs = ++p;
			read_till(&p, '&');
			if(p-qs > n - 1)
				return NULL;
			*len = p-qs;
			strncpy(buf, qs, *len);
			buf[p-qs] = '\0';
			return buf;
		}
		
		/* eat up to and including & or \0 */
		if(*p){
			read_till(&p, '&');
			if(*p)
				p++;
		}
	}
	return NULL;
}/* end: kgi_get_param */

/* kgi_post_param
 * searches through stdin for a given key, if found returns the value in
 * the given array
 *
 * param key: the key to look for
 * param buf: the buffer to store in
 * param n: the size of BUF
 * param len: the number of characters written to BUF
 * return: a pointer to BUF if the key was found otherwise NULL
 */
char *kgi_post_param(const char *key, char *buf, unsigned n, unsigned *len)
{
	fprintf(stderr, "%s is a stub\n", __func__);
	return NULL;
}/* end: kgi_post_param */

/* kgi_post_boundary_param
 * searchs through stdin using the given boundary to delimit values and 
 * keys, if the key is found then the matched value is returned in the 
 * given buffer
 *
 * param key: the key to look for
 * param buf: the buffer to store in
 * param bound: the boundary string to use
 * param n: the size of BUF
 * param len: the number of characters written to BUF
 * return: a point to BUF if the key was found otherwise NULL
 */
char *kgi_post_boundary_param(const char *key, char *buf, char *bound, 
		unsigned n, unsigned *len)
{
	fprintf(stderr, "%s is a stub\n", __func__);
	return NULL;
}/* end: kgi_post_boundary_param */
