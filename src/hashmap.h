#pragma once

#include <stdlib.h>
#include <string.h>


int cmp_strings(void *a, void *b);

/*
 * Credits: http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned int hash_function_string(void *a);