#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "utils.h"

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

	/* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
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
		/* Adica n == 0. */
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

	if (list->head == NULL) { /* Lista este goala. */
		return NULL;
	}

	/* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei.
	 */
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
		/* Adica n == 0. */
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}

	list->size--;

	return curr;
}

int get_size(struct LinkedList *list)
{
	if (list == NULL) {
		return -1;
	}

	return list->size;
}

void free_list(struct LinkedList **pp_list)
{
	struct Node *currNode;

	if (pp_list == NULL || *pp_list == NULL) {
		return;
	}

	while (get_size(*pp_list) > 0) {
		currNode = remove_nth_node(*pp_list, 0);
		free(currNode);
	}

	free(*pp_list);
	*pp_list = NULL;
}

void print_string_linkedlist(struct LinkedList *list)
{
	struct Node *curr;

	if (list == NULL) {
		return;
	}

	curr = list->head;
	while (curr != NULL) {
		printf("%s ", (char *)curr->data);
		curr = curr->next;
	}

	printf("\n");
}
