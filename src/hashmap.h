#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include "linkedlist.h"

struct info {
	void *key;
	void *value;
};

struct Hashtable {
	struct LinkedList *buckets;
	int size;
	int hmax;
	unsigned int (*hash_function)(void *);
	int (*compare_function)(void *, void *);
};

void init_ht(struct Hashtable *ht, int hmax,
	     unsigned int (*hash_function)(void *),
	     int (*compare_function)(void *, void *));

void put(struct Hashtable *ht, void *key, int key_size_bytes, void *value);

void *get(struct Hashtable *ht, void *key);

int has_key(struct Hashtable *ht, void *key);

void remove_ht_entry(struct Hashtable *ht, void *key);

int get_ht_size(struct Hashtable *ht);

int get_ht_hmax(struct Hashtable *ht);

void free_ht(struct Hashtable *ht);

int cmp_strings(void *a, void *b);

unsigned int hash_function_string(void *a);

#endif /* __HASHMAP_H__ */
