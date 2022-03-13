#include "hashmap.h"

int cmp_strings(void *a, void *b) { return strcmp((char *)a, (char *)b); }

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

void init_ht(struct Hashtable *ht, int hmax,
	     unsigned int (*hash_function)(void *),
	     int (*compare_function)(void *, void *))
{
	ht->size = 0;
	ht->hmax = hmax;
	ht->hash_function = hash_function;
	ht->compare_function = compare_function;

	int i;

	ht->buckets = malloc(hmax * sizeof(struct LinkedList));
	for (i = 0; i < hmax; i++) {
		init_list(&ht->buckets[i]);
	}
}

void put(struct Hashtable *ht, void *key, int key_size_bytes, void *value)
{
	int index;
	struct info *info_tmp;
	index = ht->hash_function(key) % ht->hmax;

	struct Node *it = ht->buckets[index].head;

	while (it != NULL) {
		if (ht->compare_function(((struct info *)it->data)->key, key) ==
		    0) {
			((struct info *)it->data)->value = value;
			return;
		}
		it = it->next;
	}

	info_tmp = malloc(sizeof(struct info));
	info_tmp->key = malloc(key_size_bytes);
	memcpy(info_tmp->key, key, key_size_bytes);

	info_tmp->value = value;

	add_nth_node(&ht->buckets[index], get_size(&ht->buckets[index]),
		     info_tmp);
	ht->size++;
}

void *get(struct Hashtable *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;
	struct Node *it = ht->buckets[index].head;

	while (it != NULL) {
		if (ht->compare_function(((struct info *)it->data)->key, key) ==
		    0) {
			return ((struct info *)it->data)->value;
		}
		it = it->next;
	}

	return NULL;
}

int has_key(struct Hashtable *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;
	struct Node *it = ht->buckets[index].head;

	while (it != NULL) {
		if (ht->compare_function(((struct info *)it->data)->key, key) ==
		    0) {
			return 1;
		}
		it = it->next;
	}

	return 0;
}

void remove_ht_entry(struct Hashtable *ht, void *key)
{
	int index;
	struct Node *tmp;
	index = ht->hash_function(key) % ht->hmax;

	struct Node *it = ht->buckets[index].head;
	int pozitie = 0;
	while (it != NULL) {
		if (ht->compare_function(((struct info *)it->data)->key, key) ==
		    0) {
			break;
		}
		pozitie++;
		it = it->next;
	}

	tmp = remove_nth_node(&ht->buckets[index], pozitie);

	free((((struct info *)tmp->data)->key));
	free(((struct info *)tmp->data));
	free(tmp);

	ht->size--;
}

void free_ht(struct Hashtable *ht)
{
	int i;
	struct LinkedList *lista_curenta;
	struct Node *it, *tmp;

	for (i = 0; i < ht->hmax; i++) {
		lista_curenta = &ht->buckets[i];
		it = lista_curenta->head;

		while (it != NULL) {
			tmp = it;
			it = it->next;
			tmp->next = NULL;
			free((((struct info *)tmp->data)->key));
			free((struct info *)tmp->data);
			free(tmp);
		}
	}
	free(ht->buckets);
	free(ht);
}

int get_ht_size(struct Hashtable *ht)
{
	if (ht == NULL) {
		return -1;
	}

	return ht->size;
}

int get_ht_hmax(struct Hashtable *ht)
{
	if (ht == NULL) {
		return -1;
	}

	return ht->hmax;
}
