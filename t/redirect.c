#include <kgi.h>
#include <stdio.h>

int main(void)
{
	struct kgi k;
	struct kgi_html *html, *c, *t;

	kgi_init(&k);
	kgi_set_status(&k, 302);
	kgi_add_header(&k, "Location", "redirect-tgt.cgi");
	html = kgi_html_new(HTML);
	kgi_set_html(&k, html);
	t = c = kgi_html_new(BODY);
	kgi_html_add_child(html, c);
	c = kgi_html_new(P);
	kgi_html_set_text(c, "Either this test failed or your browser "
			"doesn't follow redirects");
	kgi_html_add_child(t, c);
	kgi_output(&k, stdout);
	kgi_destroy(&k);

	return 0;
}
