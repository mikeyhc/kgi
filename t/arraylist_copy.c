#include <arraylist.h>
#include <kgi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	struct kgi k;
	struct kgi_html *html, *c, *t;
	struct arraylist a, b;
	char buf[128];

	arraylist_init(&b);
	arraylist_add(&b, (void*)1);
	arraylist_add(&b, (void*)2);
	arraylist_add(&b, (void*)3);
	arraylist_copy(&a, &b);

	kgi_init(&k);
	html = kgi_html_new(HTML);
	kgi_set_html(&k, html);
	t = c = kgi_html_new(BODY);
	kgi_html_add_child(html, t);
	c = kgi_html_new(PARAGRAPH);
	kgi_html_add_child(t, c);
	strcpy(buf, "arraylist_copy test: ");

	if(arraylist_get(&a, 0) == arraylist_get(&b, 0)
			&& arraylist_get(&a, 1) == arraylist_get(&b, 1)
			&& arraylist_get(&a, 2) == arraylist_get(&b, 2))
		strcat(buf, "passed");
	else{
		strcat(buf, "failed");
		kgi_html_set_text(c, buf);

		c = kgi_html_new(PARAGRAPH);
		kgi_html_add_child(t, c);
		sprintf(buf, "%ld = %ld", (long)arraylist_get(&a, 0), 
				(long)arraylist_get(&b, 0));
		kgi_html_set_text(c, buf);

		c = kgi_html_new(PARAGRAPH);
		kgi_html_add_child(t, c);
		sprintf(buf, "%ld = %ld", (long)arraylist_get(&a, 1), 
				(long)arraylist_get(&b, 1));
		kgi_html_set_text(c, buf);

		c = kgi_html_new(PARAGRAPH);
		kgi_html_add_child(t, c);
		sprintf(buf, "%ld = %ld", (long)arraylist_get(&a, 2), 
				(long)arraylist_get(&b, 2));
	}
	kgi_html_set_text(c, buf);
	kgi_output(&k, stdout);
	kgi_destroy(&k);
	arraylist_destroy(&a);
	arraylist_destroy(&b);

	return 0;
}
