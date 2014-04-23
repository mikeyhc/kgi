#include <kgi.h>
#include <kgi/html.h>
#include <stdio.h>
#include <string.h>

#define TEST1 "<html><body></body></html>"
#define TEST2 "<html><body><a href='http://google.com'>Go to google.com"\
	      "</a></body></html>"

void failed(struct kgi_html *html, char *str, char *target)
{
	sprintf(target, "FAILED (got %d expected %ld)", 
			kgi_html_size(html), strlen(str));
}

int main(void)
{
	struct kgi k;
	struct kgi_html *html, *a;
	struct kgi_html *out_html, *c, *t;
	char content[124];

	kgi_init(&k);
	out_html = kgi_html_new(HTML);
	kgi_set_html(&k, out_html);
	t = c = kgi_html_new(BODY);
	kgi_html_add_child(out_html, c);

	html = kgi_html_new(HTML);
	kgi_html_add_child(html, kgi_html_new(BODY));
	c = kgi_html_new(PARAGRAPH);
	kgi_html_add_child(t, c);
	strcpy(content, "testing basic tags: ");
	if(kgi_html_size(html) == strlen(TEST1))
		strcat(content, "PASSED");
	else
		failed(html, TEST1, content + strlen(content));
	kgi_html_set_text(c, content);
	a = kgi_html_new(ANCHOR);
	kgi_html_add_attr(a, "href", "'http://google.com'");
	kgi_html_set_text(a, "Go to google.com");
	kgi_html_add_child(html, a);
	c = kgi_html_new(PARAGRAPH);
	kgi_html_add_child(t, c);
	strcpy(content, "testing content and attributes: ");
	if(kgi_html_size(html) == strlen(TEST2))
		strcat(content, "PASSED");
	else
		failed(html, TEST2, content + strlen(content));
	kgi_html_set_text(c, content);
	kgi_output(&k, stdout);
	kgi_html_destroy(html);
	kgi_destroy(&k);

	return 0;
}
