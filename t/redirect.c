#include <kgi.h>
#include <stdio.h>

int main(void){
	struct kgi k;

	kgi_init(&k);
	kgi_set_status(&k, 302);
	kgi_add_header(&k, "Location", "redirect-tgt.cgi");
	kgi_add_data(&k, "<body><p>Either this test failed or your "
			"browser doesn't follow redirects</p></body>");
	kgi_output(&k, stdout);
	kgi_destroy(&k);

	return 0;
}
