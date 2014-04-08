#ifndef _KGI_H
#define _KGI_H

#include <stdio.h>

struct kgi {
	int status;
	char **cookies;
	char *data;
};

void kgi_init(struct kgi*);
void kgi_destroy(struct kgi*);

void kgi_output(struct kgi*, FILE*);

#endif
