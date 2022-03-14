#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include "linkedlist.h"

struct pair {
	void *key;
	void *value;
};

struct Hashmap {
	struct LinkedList *buckets;
	int size;
	int hmax;
	unsigned int (*hash_function)(void *);
	int (*compare_function)(void *, void *);
};

void init_ht(struct Hashmap *ht, int hmax,
	     unsigned int (*hash_function)(void *),
	     int (*compare_function)(void *, void *));

void put(struct Hashmap *ht, void *key, int key_size_bytes, void *value);

void *get(struct Hashmap *ht, void *key);

int has_key(struct Hashmap *ht, void *key);

void remove_ht_entry(struct Hashmap *ht, void *key);

int get_ht_size(struct Hashmap *ht);

int get_ht_hmax(struct Hashmap *ht);

void free_ht(struct Hashmap *ht);

int cmp_strings(void *a, void *b);

unsigned int hash_function_string(void *a);


unsigned int hash_function_int(void *a);

int compare_function_ints(void *a, void *b);


#endif /* __HASHMAP_H__ */
