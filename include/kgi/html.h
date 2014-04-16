#ifndef _KGI_HTML_H
#define _KGI_HTML_H

#include <arraylist.h>
#include <stdint.h>
#include <stdio.h>


#define NO_CONTENT	-1
#define CONTENT_UNINIT	 0
#define CONTENT_ARRAY	 1
#define CONTENT_TEXT	 2

#define CAN_CHILD	0x80

/* html elements, must be less than 0x80 */
/* any changes here must update kgi_html.c: tags/length array */
#define ANCHOR		CAN_CHILD | 0x00
#define INPUT			    0x01
#define SELECT	        CAN_CHILD | 0x02
#define DIV		CAN_CHILD | 0x03

typedef uint8_t html_type;

struct kgi_html_attr {
	char *key, *value;	
};

struct kgi_html {
	union {
		struct arraylist children;
		const char *text;
	} content;
	struct arraylist hattr;
	html_type type;
	unsigned char _content_init;
};

void kgi_html_init(struct kgi_html*,html_type);
void kgi_html_destroy(struct kgi_html*);
struct kgi_html *kgi_html_new(html_type);
unsigned kgi_html_size(struct kgi_html*);
void kgi_html_render(struct kgi_html*,char*);

uint8_t kgi_html_set_text(struct kgi_html*,const char*);
void kgi_html_clear_text(struct kgi_html*);
uint8_t kgi_html_set_child(struct kgi_html*,struct kgi_html*);
uint8_t kgi_html_add_child(struct kgi_html*,struct kgi_html*);
void kgi_html_clear_children(struct kgi_html*);

uint8_t kgi_html_set_attr(struct kgi_html*,const char*,const char*);
uint8_t kgi_html_add_attr(struct kgi_html*,const char*,const char*);
void kgi_html_clear_attrs(struct kgi_html*);

#endif
