#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
	fprintf(stream, "Content-Length: %d\n\n", kgi_size_data(kgi));
	kgi_output_data(kgi, stream);
}/* end: kgi_output */
