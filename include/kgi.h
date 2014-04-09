#ifndef _KGI_H
#define _KGI_H

#include <stdio.h>

#include <arraylist.h>

/* TODO: could use binary trees or hashmaps here for better performance */
struct kgi {
	int status;
	struct arraylist cookies;
	struct arraylist data;
	struct arraylist headers;
};

void kgi_init(struct kgi*);
void kgi_destroy(struct kgi*);

int kgi_add_cookie(struct kgi*, char*, char*);
void kgi_remove_cookie(struct kgi*, char*);
int kgi_destroy_cookie(struct kgi*, char*);
void kgi_clear_cookies(struct kgi*);

int kgi_set_data(struct kgi*, char*);
int kgi_add_data(struct kgi*, char*);
void kgi_clear_data(struct kgi*);

int kgi_add_header(struct kgi*, char*, char*);
void kgi_remove_header(struct kgi*, char*);
void kgi_clear_headers(struct kgi*);

void kgi_set_status(struct kgi*, int);
int kgi_get_status(struct kgi*);

void kgi_output(struct kgi*, FILE*);

#endif
