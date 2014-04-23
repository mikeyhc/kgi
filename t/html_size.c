#include <kgi.h>
#include <kgi/html.h>
#include <stdio.h>
#include <string.h>

#define TEST1 "<html><body></body></html>"
#define TEST2 "<html><body><a href='http://google.com'>Go to google.com"\
	      "</a></body></html>"

void failed(struct kgi *k, struct kgi_html *html, char *str)
{
	char temp[128];

	kgi_add_data(k, "FAILED (got ");
	sprintf(temp, "%d", kgi_html_size(html));
	kgi_add_data(k, temp);
	kgi_add_data(k, " expected ");
	sprintf(temp, "%ld", strlen(str));
	kgi_add_data(k, temp);
	kgi_add_data(k, ")<br />");
}

int main(void)
{
	struct kgi k;
	struct kgi_html *html, *a;

	kgi_init(&k);
	kgi_add_data(&k, "<body>");

	html = kgi_html_new(HTML);
	kgi_html_add_child(html, kgi_html_new(BODY));
	kgi_add_data(&k, "testing basic tags: ");
	if(kgi_html_size(html) == strlen(TEST1))
		kgi_add_data(&k, "PASSED<br />");
	else
		failed(&k, html, TEST1);
	a = kgi_html_new(ANCHOR);
	kgi_html_add_attr(a, "href", "'http://google.com'");
	kgi_html_set_text(a, "Go to google.com");
	kgi_html_add_child(html, a);
	kgi_add_data(&k, "testing content and attributes: ");
	if(kgi_html_size(html) == strlen(TEST2))
		kgi_add_data(&k, "PASSED<br />");
	else
		failed(&k, html, TEST2);
	
	kgi_add_data(&k, "</body>");
	kgi_output(&k, stdout);
	kgi_html_destroy(html);
	kgi_destroy(&k);

	return 0;
}
