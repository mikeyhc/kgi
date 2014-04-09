#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arraylist.h>
#include <kgi.h>

static char* _get_code(int code)
{
	switch(code){
	}
	return "200 OK";
}

void kgi_init(struct kgi *k)
{
	assert(k != NULL);

	k->status = 200;
	arraylist_init(&k->cookies);
	arraylist_init(&k->data);
	arraylist_init(&k->headers);
}

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
}

void kgi_output(struct kgi *kgi, FILE *stream)
{
	assert(kgi != NULL);

	fprintf(stream, "HTTP/1.1 %s\n", _get_code(kgi->status));
	/*fprintf(stream, "Content-length: %d\n", strlen(kgi->data)); */
	fprintf(stream, "Content-type: text/html\n\n");
	/* fprintf(stream, kgi->data); */
}
