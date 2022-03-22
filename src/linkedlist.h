#ifndef LINKEDLIST_H__
#define LINKEDLIST_H__

#include "utils.h"

struct Node {
	struct Node *next;
	void *data;
};

struct LinkedList {
	struct Node *head;
	int size;
};

void list_init(struct LinkedList *list);

void add_node(struct LinkedList *list, int nth_node, void *new_data,
		  unsigned int new_data_size);

void *remove_node(struct LinkedList *list, int nth_node);

void *get_node(struct LinkedList *list, int n);

int list_size(struct LinkedList *list);

void free_list_mem(struct LinkedList **list);

#endif /* LINKEDLIST_H__ */
