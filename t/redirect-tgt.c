#include <kgi.h>
#include <stdio.h>

int main(void)
{
	struct kgi k;
	struct kgi_html *html, *c, *t;

	kgi_init(&k);
	html = kgi_html_new(HTML);
	kgi_set_html(&k, html);
	t = c = kgi_html_new(BODY);
	kgi_html_add_child(html, c);
	c = kgi_html_new(H1);
	kgi_html_set_text(c, "Test Passed");
	kgi_html_add_child(t, c);
	kgi_output(&k, stdout);

	return 0;
}
