#include <hashmap.h>

int cmp_strings(void *a, void *b) { return strmcp((char *)a, (char *)b); }

unsigned int hash_function_string(void *a)
{
	unsigned char *puchar_a = (unsigned char *)a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++)) {
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */
	}

	return hash;
}
