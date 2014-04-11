#ifndef _KGI_HTML_H
#define _KGI_HTML_H

#include <stdio.h>

#include <arraylist.h>

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

struct kgi_html_attr {
	char *key, *value;	
};

struct kgi_html {
	union {
		struct arraylist children;
		char *text;
	} content;
	struct arraylist hattr;
	unsigned char type;
	unsigned char _content_init;
};

void kgi_html_init(struct kgi_html*,unsigned char);
void kgi_html_destroy(struct kgi_html*);
unsigned kgi_html_size(struct kgi_html*);
void kgi_html_render(struct kgi_html*,char*);

#endif
