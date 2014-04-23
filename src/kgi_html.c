#include <arraylist.h>
#include <assert.h>
#include <kgi/html.h>
#include <kstring.h>
#include <stdlib.h>
#include <string.h>

#define ATTR_MAX_LEN	512

#define ALL_TAGS (CAN_TEXT|CAN_CHILD)

#define GET_TAG(x) tags[x & ~ALL_TAGS]
#define GET_TAG_LENGTH(x) tag_length[x & ~ALL_TAGS]


/* could automate this but when the tables get larger the overhead will
 * become noticable */
static const char *tags[] = {
	"html",		"body",		"a",		"input",
	"select",	"div",		"h1",		"head",
	"p"
	};

static const uint8_t tag_length[] = {
	4,	4,	1,	5,
	6,	3,	2,	4,
	1,
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
	if(t & (CAN_CHILD | CAN_TEXT))
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

/* kgi_html_new
 * allocates and returns a new html of the given type
 *
 * param type: the type of the new html
 * return: the new html or NULL if one could not be created
 *         failure indicates insufficient memory
 */
struct kgi_html *kgi_html_new(html_type type)
{
	struct kgi_html *html;

	html = malloc(sizeof(*html));
	if(!html)
		return NULL;
	kgi_html_init(html, type);
	return html;
}/* end: kgi_html_new */

/* count_attrs
 * counts the length of all of the attrs (includes leading space)
 *
 * param attrs: the list of attrs
 * return: total number of characters required to print the list
 */
static unsigned count_attrs(const struct arraylist *attrs)
{
	unsigned i, j, l, count;
	struct kgi_html_attr *a;

	assert(attrs);

	count = 0;
	l = arraylist_size(attrs);
	for(i=j=0; j<l; i++)
		if((a = (struct kgi_html_attr*)arraylist_get(attrs, i))){
			/* 2 = " =" */
			count += kstrlen(a->key) + kstrlen(a->value) + 2;
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
unsigned kgi_html_size(const struct kgi_html *html)
{
	unsigned count;
	unsigned i, j, l;
	void *e;

	assert(html);

	count = GET_TAG_LENGTH(html->type) + 1; /* < */
	count += (html->type & ALL_TAGS ? count : 0) + 3;
	count += count_attrs(&html->hattr);
	if(html->_content_init == CONTENT_ARRAY){
		l = arraylist_size(&html->content.children);
		for(i=j=0; j<l; i++)
			if((e = arraylist_get(&html->content.children, 
					i))){
				count += kgi_html_size(
						(struct kgi_html*)e);
				j++;
			}
	}else if(html->_content_init == CONTENT_TEXT)
		count += kstrlen(html->content.text);

	return count;
}/* end: kgi_html_size */

/* attr_render
 * renders the list of attributes
 *
 * param attrs: the list of attributes
 * param str: the string to render them to
 * return: a pointer to after the rendered tags
 */
static char *attr_render(const struct arraylist *attrs, char *str)
{
	unsigned i, j, len;
	void *e;
	struct kgi_html_attr *a;

	len = arraylist_size(attrs);
	for(i=j=0; j < len; i++)
		if((e = arraylist_get(attrs, i))){
			a = (struct kgi_html_attr*)e;
			sprintf(str, " %s=%s", a->key, a->value);
			str += kstrlen(a->key) + kstrlen(a->value) + 2;
			j++;
		}
	return str;
}/* end: attr_render */

/* html_render
 * actually does the html render
 *
 * param html: the html to render
 * param str: the string to write to, assumes correct size
 * return: a pointer to after the rendered html
 */
static char *html_render(const struct kgi_html *html, char *str)
{
	unsigned i, j, len;
	void *e;

	assert(html && str);

	sprintf(str, "<%s", GET_TAG(html->type));
	str = attr_render(&html->hattr, str + 1 + 
			GET_TAG_LENGTH(html->type));
	if(html->type & ALL_TAGS){
		sprintf(str, ">"); 
		str++;
	}else{
		sprintf(str, " />");
		return str + 3;
	}

	if(html->_content_init == CONTENT_ARRAY){
		len = arraylist_size(&html->content.children);
		for(i=j=0; j < len; i++)
			if((e = arraylist_get(&html->content.children, i)))
				str = html_render((struct kgi_html*)e, 
						str), j++;
	}else if(html->_content_init == CONTENT_TEXT){
		sprintf(str, html->content.text);
		str += kstrlen(html->content.text);
	}

	sprintf(str, "</%s>", GET_TAG(html->type));
	return str + GET_TAG_LENGTH(html->type) + 3;
}/* end: html_render */

/* kgi_html_render
 * writes the html to the given buffer
 *
 * param html: the html to render
 * param str: the string to write to, assumes correct size
 */
void kgi_html_render(const struct kgi_html *html, char *str)
{
	assert(html && str);

	*(html_render(html, str)) = '\0';
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
	assert(html && (html->_content_init == CONTENT_UNINIT ||
			html->_content_init == CONTENT_TEXT) &&
			(html->type & CAN_TEXT) && str);

	if(html->_content_init == CONTENT_TEXT)
		kgi_html_clear_text(html);
	html->content.text = kstring_new(str);
	if(!html->content.text)
		return 0;
	html->_content_init = CONTENT_TEXT;
	return 1;
}/* end: kgi_html_set_text */

/* kgi_html_clear_text
 * sets the content to nothing and frees any used memroy
 *
 * param html: the html to clear the content of
 */
void kgi_html_clear_text(struct kgi_html *html)
{
	assert(html && (html->_content_init == CONTENT_UNINIT ||
			html->_content_init == CONTENT_TEXT));

	kstring_destroy(html->content.text);
	html->content.text = NULL;
	html->_content_init = CONTENT_UNINIT;
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
	assert(html && (html->_content_init == CONTENT_UNINIT ||
			html->_content_init == CONTENT_ARRAY) && 
			(html->type & CAN_CHILD) && child);
	
	if(html->_content_init == CONTENT_ARRAY)
		kgi_html_clear_children(html);
	return kgi_html_add_child(html, child);
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
	assert(html && (html->_content_init == CONTENT_UNINIT || 
			html->_content_init == CONTENT_ARRAY) && 
			(html->type & CAN_CHILD) && child);

	if(html->_content_init == CONTENT_UNINIT)
		if(!arraylist_init(&html->content.children))
			return 0;
	if(!arraylist_add(&html->content.children, child))
		return 0;
	html->_content_init = CONTENT_ARRAY;
	return 1;
}/* end: kgi_html_add_child */

/* kgi_html_clear_children
 * clears all the children elements from the html
 *
 * param html: the html to clear
 */
void kgi_html_clear_children(struct kgi_html *html)
{
	assert(html && html->_content_init == CONTENT_ARRAY);

	arraylist_destroy(&html->content.children);
	html->_content_init = CONTENT_UNINIT;
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
	assert(html && key && value);

	if(arraylist_size(&html->hattr))
		kgi_html_clear_attrs(html);
	return kgi_html_add_attr(html, key, value);
}/* end: kgi_html_set_attr */

/* free_attr
 * frees the given attribute
 *
 * param attr: the attribute to free
 */
static void free_attr(void *attr)
{
	assert(attr);

	kstring_destroy(((struct kgi_html_attr*)attr)->key);
	kstring_destroy(((struct kgi_html_attr*)attr)->value);
	free(attr);
}/* end: free_attr */

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
	struct kgi_html_attr *attr;

	assert(html && key && value);

	attr = malloc(sizeof(*attr));
	if(!attr)
		return 0;
	attr->key = kstring_new(key);
	attr->value = kstring_new(value);
	
	if(!attr->key || !attr->value){
		if(attr->key) kstring_destroy(attr->key);
		if(attr->value) kstring_destroy(attr->value);
		free(attr);
		return 0;
	}

	if(!arraylist_add(&html->hattr, attr)){
		free_attr(attr);
		return 0;
	}
	return 1;
}/* end: kgi_html_add_attr */

/* kgi_html_clear_attrs
 * clears all the attributes of the html
 *
 * param html: the html to clear
 */
void kgi_html_clear_attrs(struct kgi_html *html)
{
	assert(html);

	arraylist_destroy_freewith(&html->hattr, free_attr);
	arraylist_init(&html->hattr);
}/* end: kgi_html_clear_attr */
