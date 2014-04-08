#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	k->cookies = NULL;
	k->data = NULL;
}

void kgi_destroy(struct kgi *k)
{
	assert(k != NULL);
	k->status = 0;
	/* TODO: deal with pointers */
}

void kgi_output(struct kgi *kgi, FILE *stream)
{
	assert(kgi != NULL && kgi->data != NULL);

	fprintf(stream, "HTTP/1.0 %s\n", _get_code(kgi->status));
	/*fprintf(stream, "Content-length: %d\n", strlen(kgi->data)); */
	fprintf(stream, "Content-type: text/html\n\n");
	/* fprintf(stream, kgi->data); */
}
