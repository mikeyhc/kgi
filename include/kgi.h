#ifndef _KGI_H
#define _KGI_H

#include <arraylist.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


/* TODO: could use binary trees or hashmaps here for better performance */
struct kgi {
	unsigned status;
	struct arraylist cookies;
	struct arraylist data;
	struct arraylist headers;
};

void kgi_init(struct kgi*);
void kgi_destroy(struct kgi*);
void kgi_set_status(struct kgi*, unsigned);
unsigned kgi_get_status(struct kgi*);
void kgi_output(struct kgi*,FILE*);

size_t kgi_param(const char*,struct arraylist*);
size_t kgi_param_keys(struct arraylist*);

uint8_t kgi_add_cookie(struct kgi*, const char*, const char*);
const char *kgi_get_cookie(const char*);
void kgi_remove_cookie(struct kgi*, const char*);
uint8_t kgi_destroy_cookie(struct kgi*, const char*);
void kgi_clear_cookies(struct kgi*);
void kgi_output_cookies(struct kgi*,FILE*);
#define kgi_set_message(kgi,msg) kgi_add_cookie(kgi,"FormMessage", msg)
#define kgi_get_message() kgi_get_cookie("FormMessage")
#define kgi_clear_message(kgi) kgi_destroy_cookie(kgi, "FormMessage")

int kgi_set_data(struct kgi*,const char*);
int kgi_add_data(struct kgi*,const char*);
void kgi_clear_data(struct kgi*);
unsigned kgi_size_data(struct kgi*);
void kgi_output_data(struct kgi*,FILE*);

int kgi_add_header(struct kgi*,char*,char*);
void kgi_remove_header(struct kgi*, char*);
void kgi_clear_headers(struct kgi*);
void kgi_output_headers(struct kgi*,FILE*);

#endif
