#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi.h>
#include <kgi/html.h>

/* kgi_set_html
 * sets the data of the kgi response to the given value
 *
 * param kgi: the kgi to update
 * param html: the html for the kgi
 */
void kgi_set_html(struct kgi *kgi, const struct kgi_html *html)
{
	assert(kgi && html);

	kgi->html = html;
}/* end: kgi_set_html */

/* kgi_clear_html
 * removes the html from the reply
 *
 * param kgi: the kgi to clear
 */
void kgi_clear_html(struct kgi *kgi)
{
	assert(kgi);

	kgi->html = NULL;
}/* end: kgi_clear_html */

/* kgi_size_html
 * returns the size of the html output (useful for content-length)
 *
 * param kgi: the kgi to check
 * return: the string length of the html
 */
unsigned kgi_size_html(struct kgi *kgi)
{
	assert(kgi);

	return kgi_html_size(kgi->html);
}/* end: kgi_size_html */

/* kgi_output_data
 * prints the html contained in the kgi to the given stream
 *
 * param kgi: the kgi to print from
 * param stream: the stream to print to
 */
void kgi_output_html(struct kgi *kgi, FILE *stream)
{
	char *out;

	assert(kgi && stream);

	if(!kgi->html)
		return;
	out = malloc(kgi_size_html(kgi));
	if(!out)
		return;
	kgi_html_render(kgi->html, out);
	fprintf(stream, out);
	free(out);
}/* end: kgi_output_html */
