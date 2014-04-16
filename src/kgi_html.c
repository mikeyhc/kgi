#include <arraylist.h>
#include <assert.h>
#include <kgi/html.h>
#include <kstring.h>
#include <stdlib.h>
#include <string.h>


#define ATTR_MAX_LEN	512

/* TODO: autogenerate these 2 arrays */
const char *tags[] =
	{ "html"
	, "a"
	, "input"
	, "select"
	, "div"
	};

const unsigned length[] = 
	{ 4
	, 1
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
	unsigned i, j, l;
	struct kgi_html *e;

	assert(html);

	html->type = 0;
	/* need an attribute free method */
	/* arraylist_destroy_free(&html->hattr); */
	switch(html->_content_init){
	case CONTENT_ARRAY:
		kgi_html_clear_children(html);
		break;
	case CONTENT_TEXT:
		kgi_html_clear_text(html);
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
	unsigned i, j, l, count;
	struct kgi_html_attr *a;

	count = 0;
	l = arraylist_size(attrs);
	for(i=j=0; j<l; i++)
		if((a = (struct kgi_html_attr*)arraylist_get(attrs, i))){
			/* 4 = "'' =" */
			count += kstrlen(a->key) + kstrlen(a->value) + 4;
			j++;
		}
	return count;
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

	count = length[html->type & ~CAN_CHILD] + 1; /* < */
	count += (html->type & CAN_CHILD ? count : 0) + 3;
	count += count_attrs(&html->hattr);
	if(html->_content_init == CONTENT_ARRAY){
		l = arraylist_size(&html->content.children);
		fprintf(stderr, "found children: %d\n", l);
		for(i=j=0; j<l; i++)
			if((e = arraylist_get(&html->content.children, 
					i))){
				count += kgi_html_size(
						(struct kgi_html*)e);
				j++;
			}
	}else if(html->_content_init == CONTENT_TEXT)
		count += kstrlen(html->content.text);

	fprintf(stderr, "count: %d\n", count);
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

/* kgi_html_set_text
 * sets the content of the html tag to the given text
 *
 * param html:  the html to update
 * param str: the content to add
 * return: true if content set else false
 *         failure indicates 
 */
uint8_t kgi_html_set_text(struct kgi_html *html, const char *str)
{
	assert(html && html->_content_init != CONTENT_ARRAY && str);

	if(html->_content_init == CONTENT_TEXT)
		kgi_html_clear_text(html);
	html->content.text = kstring_new(str);
	if(!html->content.text)
		return 0;
	html->_content_init = CONTENT_TEXT;
	return 0;
}/* end: kgi_html_set_text */

/* kgi_html_clear_text
 * sets the content to nothing and frees any used memroy
 *
 * param html: the html to clear the content of
 */
void kgi_html_clear_text(struct kgi_html *html)
{
}/* end: kgi_html_clear_text */

/* kgi_html_set_child
 * sets the child of HTML to CHILD
 *
 * param html: the html to update
 * param child: the child element
 * return: true if set otherwise false
 *         return is derived from kgi_html_add_child
 */
uint8_t kgi_html_set_child(struct kgi_html *html, struct kgi_html *child)
{
	return 0;
}/* end: kgi_html_set_child */

/* kgi_html_add_child
 * adds a child element to HTML
 *
 * param html: the html to update
 * param child: the child element
 * return: true if added else false
 *         failure indicates unable to add to or initialise arraylist
 */
uint8_t kgi_html_add_child(struct kgi_html *html, struct kgi_html *child)
{
	return 0;
}/* end: kgi_html_add_child */

/* kgi_html_clear_children
 * clears all the children elements from the html
 *
 * param html: the html to clear
 */
void kgi_html_clear_children(struct kgi_html *html)
{

}/* end: kgi_html_clear_children */

/* kgi_html_set_attr
 * sets the attributes of the html to the given values
 *
 * param html: the html to update
 * param key: the key of the attribute
 * param value: the value of the attribute
 * return: true if the attribute is set else false
 *         return is derived from kgi_html_add_attr
 */
uint8_t kgi_html_set_attr(struct kgi_html *html, const char *key,
		const char *value)
{
	return 0;
}/* end: kgi_html_set_attr */

/* kgi_html_add_attr 
 * adds the given attribute to the html
 *
 * param html: the html to update
 * param key: the key of the attribute
 * param value: the value of the attribute
 * return: true if attribute is added else false
 *         failure indicates unable to add to or intialise arraylist
 */
uint8_t kgi_html_add_attr(struct kgi_html *html, const char *key,
		const char *value)
{
	return 0;
}/* end: kgi_html_add_attr */

/* kgi_html_clear_attrs
 * clears all the attributes of the html
 *
 * param html: the html to clear
 */
void kgi_html_clear_attrs(struct kgi_html *html)
{
	
}/* end: kgi_html_clear_attr */
