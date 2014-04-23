#include <kgi.h>
#include <kgi/html.h>
#include <stdio.h>

int main(void)
{
	struct kgi k;
	struct kgi_html *html, *h1, *body;

	kgi_init(&k);
	html = kgi_html_new(HTML);
	body = kgi_html_new(BODY);
	kgi_html_add_child(html, body);
	h1 = kgi_html_new(H1);
	kgi_html_add_attr(h1, "style", "'font-family: sans-serif;'");
	kgi_html_set_text(h1, "Check the source (dear god check it)");
	kgi_html_add_child(body, h1);
	kgi_set_html(&k, html);
	
	kgi_output(&k, stdout);
	kgi_destroy(&k);

	return 0;
}
