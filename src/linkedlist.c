#include "linkedlist.h"

void init_list(struct LinkedList *list)
{
	list->head = NULL;
	list->size = 0;
}

void add_nth_node(struct LinkedList *list, int n, void *new_data)
{
	struct Node *prev, *curr, *new_node;

	if (list == NULL) {
		return;
	}

	if (n > list->size) {
		n = list->size;
	} else if (n < 0) {
		return;
	}

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	new_node = (struct Node *)calloc(1, sizeof(struct Node));
	DIE(new_node == NULL, "Memory allocation for the new_node failed!");

	new_node->data = new_data;
	new_node->next = curr;
	if (prev == NULL) {
		list->head = new_node;
	} else {
		prev->next = new_node;
	}

	list->size++;
}

struct Node *remove_nth_node(struct LinkedList *list, int n)
{
	struct Node *prev, *curr;

	if (list == NULL) {
		return NULL;
	}

	if (list->head == NULL) {
		return NULL;
	}

	if (n > list->size - 1) {
		n = list->size - 1;
	} else if (n < 0) {
		return NULL;
	}

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	if (prev == NULL) {
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}

	list->size--;

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
	} else if (*pp_list == NULL) {
		return;
	}

	while (get_size(*pp_list) > 0) {
		currNode = remove_nth_node(*pp_list, 0);
		free(currNode);
	}

	free(*pp_list);
	*pp_list = NULL;
}
