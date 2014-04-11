#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi/html.h>

#define ATTR_MAX_LEN	512

const char *tags[] =
	{ "a"
	, "input"
	, "select"
	, "div"
	};
const unsigned length[] = 
	{ 1
	, 5
	, 6
	, 3
	};
/* kgi_html_init
 * initializes a kgi_html struct
 *
 * param html: the struct to init
 * param t: the type of html it is
 */
void kgi_html_init(struct kgi_html *html, unsigned char t)
{
	assert(html);

	html->type = t;
	arraylist_init(&html->hattr);
	if(t & CAN_CHILD)
		html->_content_init = CONTENT_UNINIT;
	else
		html->_content_init = NO_CONTENT;
}/* end: kgi_html_init */

/* kgi_html_destroy
 * frees all the resources associated with the kgi_html
 *
 * param html: the struct to free
 */
void kgi_html_destroy(struct kgi_html *html)
{
	assert(html);

	html->type = 0;
	arraylist_destroy_free(&html->hattr);
	switch(html->_content_init){
	case CONTENT_ARRAY:
		arraylist_destroy_free(&html->content.children);
		break;
	case CONTENT_TEXT:
		free(html->content.text);
		break;
	}
	html->_content_init = CONTENT_UNINIT;
}/* end: kgi_html_destroy */

/* count_attrs
 * counts the length of all of the attrs (includes leading space)
 *
 * param attrs: the list of attrs
 * return: total number of characters required to print the list
 */
static unsigned count_attrs(struct arraylist *attrs)
{
	return 0;
}/* end: count_attrs */

/* kgi_html_size
 * returns the number of characters that would be printed in
 * its rendered form
 *
 * param html: the html to calculate the size of
 * return: the size of the rendered html (in char's)
 */
unsigned kgi_html_size(struct kgi_html *html)
{
	unsigned count;
	unsigned i, j, l;
	void *e;

	count = length[html->type] + 1; /* < */
	count += (html->type & CAN_CHILD ? count*2 : 0) + 3;
	count += count_attrs(&html->hattr);
	if(html->_content_init == CONTENT_ARRAY){
		l = arraylist_size(&html->content.children);
		for(i=0; j<l; i++)
			if((e = arraylist_get(&html->content.children, 
					i))){
				count += kgi_html_size(
						(struct kgi_html*)e);
				j++;
			}
	}else if(html->_content_init == CONTENT_TEXT)
		count += strlen(html->content.text);

	return count;
}/* end: kgi_html_size */

/* kgi_html_render
 * writes the html to the given buffer
 *
 * param html: the html to render
 * param str: the string to write to, assumes correct size
 */
void kgi_html_render(struct kgi_html *html, char *str)
{

}/* end: kgi_html_render */
