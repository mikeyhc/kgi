#include <arraylist.h>
#include <kgi.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	struct kgi k;
	struct arraylist a, b;
	char buf[32];

	arraylist_init(&a);
	arraylist_add(&a, (void*)1);
	arraylist_add(&a, (void*)2);
	arraylist_add(&a, (void*)3);
	arraylist_copy(&a, &b);

	kgi_init(&k);
	kgi_add_data(&k, "<body><p>arraylist_copy test: ");
	if(arraylist_get(&a, 0) == arraylist_get(&b, 0)
			&& arraylist_get(&a, 1) == arraylist_get(&b, 1)
			&& arraylist_get(&a, 2) == arraylist_get(&b, 2))
		kgi_add_data(&k, "passed");
	else{
		kgi_add_data(&k, "failed</p><p>");
		snprintf(buf, 32, "%ld", (long)arraylist_get(&a, 0));
		kgi_add_data(&k, buf);
		kgi_add_data(&k, " = ");
		snprintf(buf, 32, "%ld", (long)arraylist_get(&b, 0));
		kgi_add_data(&k, buf);
		kgi_add_data(&k, "</p><p>");
		snprintf(buf, 32, "%ld", (long)arraylist_get(&a, 1));
		kgi_add_data(&k, buf);
		kgi_add_data(&k, " = ");
		snprintf(buf, 32, "%ld", (long)arraylist_get(&b, 1));
		kgi_add_data(&k, buf);
		kgi_add_data(&k, "</p><p>");
		snprintf(buf, 32, "%ld", (long)arraylist_get(&a, 2));
		kgi_add_data(&k, buf);
		kgi_add_data(&k, " = ");
		snprintf(buf, 32, "%ld", (long)arraylist_get(&b, 2));
		kgi_add_data(&k, buf);
		kgi_add_data(&k, "</p><p>");

	}
	kgi_add_data(&k, "</p></body>");
	kgi_output(&k, stdout);
	kgi_destroy(&k);
	arraylist_destroy(&a);
	arraylist_destroy(&b);

	return 0;
}
