#include <kgi.h>
#include <stdio.h>

int main(void){
	struct kgi k;

	kgi_init(&k);
	k.data = "<body><h1>hello, world!</h1></body>";
	kgi_output(&k, stdout);
	kgi_destroy(&k);

	return 0;
}
