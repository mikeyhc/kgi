#include <kgi.h>
#include <stdio.h>

int main(void)
{
	struct kgi k;

	kgi_init(&k);
	kgi_add_data(&k, "<body><h1>Test passed</h1></body");
	kgi_output(&k, stdout);

	return 0;
}
