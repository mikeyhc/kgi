#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <arraylist.h>
#include <kgi.h>

#define CONTENT_TYPE	"Content-type: text/html; charset=iso-8839-1\n"
#define DEFAULT_PROTO	"HTTP/1.1"

/* get_code
 * returns the message associated with a particular HTTP code
 *
 * param code: the code to fetch
 * return: the HTTP message (defaults to 200 OK)
 */
static char *get_code(unsigned code)
{
	switch(code){
	}
	return "200 OK";
}/* end: get_code */

/* get_server_proto
 * returns the server protocol or defaults to DEFAULT_PROTO
 *
 * return: the server protocol or the default
 */
char *get_server_proto(void)
{
	char *r;
	return (r = getenv("SERVER_PROTOCOL")) ? r : DEFAULT_PROTO;
}/* end: get_server_proto */

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

/* kgi_output
 * outputs the given kgi to the given stream, no error checking performed
 *
 * param kgi: the kgi to output
 * param stream: the stream to output to
 */
void kgi_output(struct kgi *kgi, FILE *stream)
{
	assert(kgi != NULL);

	if(kgi->status != 200)
		fprintf(stream, "%s %s\n\n", get_server_proto(), 
				get_code(kgi->status)); 
	fprintf(stream, "Content-Length: %d\n", kgi_size_data(kgi));
	fprintf(stream, CONTENT_TYPE"\n");
	kgi_output_data(kgi, stream);
}/* end: kgi_output */
