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

void init_ht(struct Hashmap *ht, int hmax,
	     unsigned int (*hash_function)(void *),
	     int (*compare_function)(void *, void *))
{
	ht->size = 0;
	ht->hmax = hmax;
	ht->hash_function = hash_function;
	ht->compare_function = compare_function;

	int i;

	ht->buckets =
	    (struct LinkedList *)calloc(hmax, sizeof(struct LinkedList));
	DIE(ht->buckets == NULL, "Memory allocation failed for ht->buckets!");
	for (i = 0; i < hmax; i++) {
		init_list(&ht->buckets[i]);
	}
}

void put(struct Hashmap *ht, void *key, int key_size_bytes, void *value)
{
	struct pair *info_tmp;
	int index = ht->hash_function(key) % ht->hmax;
	struct Node *it = ht->buckets[index].head;

	while (it != NULL) {
		if (ht->compare_function(((struct pair *)it->data)->key, key) ==
		    0) {
			((struct pair *)it->data)->value = value;
			return;
		}
		it = it->next;
	}

	info_tmp = (struct pair *)calloc(1, sizeof(struct pair));
	DIE(info_tmp == NULL, "Memory allocation for info_tmp failed!");
	info_tmp->key = calloc(key_size_bytes, sizeof(char));
	DIE(info_tmp->key == NULL,
	    "Memory allocation for info_tmp->key failed!");

	memcpy(info_tmp->key, key, key_size_bytes);

	info_tmp->value = value;

	add_nth_node(&ht->buckets[index], get_size(&ht->buckets[index]),
		     info_tmp);
	ht->size++;
}

void *get(struct Hashmap *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;
	struct Node *it = ht->buckets[index].head;

	while (it != NULL) {
		if (ht->compare_function(((struct pair *)it->data)->key, key) ==
		    0) {
			return ((struct pair *)it->data)->value;
		}
		it = it->next;
	}

	return NULL;
}

int has_key(struct Hashmap *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;
	struct Node *it = ht->buckets[index].head;

	while (it != NULL) {
		if (ht->compare_function(((struct pair *)it->data)->key, key) ==
		    0) {
			return 1;
		}
		it = it->next;
	}

	return 0;
}

void remove_ht_entry(struct Hashmap *ht, void *key)
{
	int index;
	struct Node *tmp;
	index = ht->hash_function(key) % ht->hmax;

	struct Node *it = ht->buckets[index].head;
	int pozitie = 0;
	while (it != NULL) {
		if (ht->compare_function(((struct pair *)it->data)->key, key) ==
		    0) {
			break;
		}
		pozitie++;
		it = it->next;
	}

	tmp = remove_nth_node(&ht->buckets[index], pozitie);

	free((((struct pair *)tmp->data)->key));
	free(((struct pair *)tmp->data));
	free(tmp);

	ht->size--;
}

void free_ht(struct Hashmap *ht)
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
			free((((struct pair *)tmp->data)->key));
			free((struct pair *)tmp->data);
			free(tmp);
		}
	}
	free(ht->buckets);
	free(ht);
}

int get_ht_size(struct Hashmap *ht)
{
	if (ht == NULL) {
		return -1;
	}

	return ht->size;
}

int get_ht_hmax(struct Hashmap *ht)
{
	if (ht == NULL) {
		return -1;
	}

	return ht->hmax;
}


int compare_function_ints(void *a, void *b) {
    int int_a = *((int *)a);
    int int_b = *((int *)b);

    if (int_a == int_b) {
        return 0;
    } else if (int_a < int_b) {
        return -1;
    } else {
        return 1;
    }
}


unsigned int hash_function_int(void *a) {
    /*
     * Credits: https://stackoverflow.com/a/12996028/7883884
     */
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}
