#include "linkedlist.h"

void init_list(struct LinkedList *list)
{
	list->head = NULL;
	list->size = 0;
}

void add_nth_node(struct LinkedList *list, int n, void *new_data)
{
	struct Node *prev, *curr, *new_node;

	if (list == NULL || n < 0) {
		return;
	} else if (n > list->size) {
		n = list->size;
	}

	prev = NULL;
	curr = list->head;

	for (; n > 0; --n) {
		prev = curr;
		curr = curr->next;
	}

	new_node = (struct Node *)calloc(1, sizeof(struct Node));
	DIE(new_node == NULL, "Memory allocation for the new_node failed!");

	new_node->next = curr;
	new_node->data = new_data;
	list->size++;

	if (prev == NULL) {
		list->head = new_node;
	} else {
		prev->next = new_node;
	}
}

struct Node *remove_nth_node(struct LinkedList *list, int n)
{
	struct Node *prev, *curr;

	if (list == NULL || list->head == NULL) {
		return NULL;
	}

	if (n > list->size - 1) {
		n = list->size - 1;
	}

	if (n < 0) {
		return NULL;
	}

	prev = NULL;
	curr = list->head;

	for (; n > 0; --n) {
		prev = curr;
		curr = curr->next;
	}

	list->size--;
	if (prev == NULL) {
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}

	return curr;
}

int get_size(struct LinkedList *list)
{
	return (list == NULL) ? -1 : list->size;
}

void free_list(struct LinkedList **pp_list)
{
	struct Node *currNode;

	if (pp_list == NULL) {
		return;
	}

	if (*pp_list == NULL) {
		return;
	}

	for (; get_size(*pp_list) > 0;) {
		currNode = remove_nth_node(*pp_list, 0);
		free(currNode);
	}

	free(*pp_list);
	*pp_list = NULL;
}
