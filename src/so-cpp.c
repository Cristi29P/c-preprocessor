#include "hashmap.h"
// Refacut makefile si includes
#define PATH_LENGTH 50
#define SMALL_BUFF 20
#define HT_ENTRIES 1000

void add_cmd_define(struct Hashmap *mappings, char *argv)
{
	char symbol[SMALL_BUFF] = {'\0'}, value[SMALL_BUFF] = {'\0'};
	sscanf(argv, "%[^=]=%s", symbol, value);
	put(mappings, symbol, strlen(symbol) + 1, value, strlen(value) + 1);
}

void add_cmd_directory(struct LinkedList *directories, char *argv) {
	char directory[PATH_LENGTH] = {'\0'};
	sscanf(argv, "%s", directory);
	add_nth_node(directories, get_size(directories), directory, strlen(directory) + 1);
}

void parse_cmd_defines(char *argv[], int argc, struct Hashmap *mappings)
{
	for (int i = 1; i < argc; i++) {
		if (!strncmp(argv[i], "-D", 2) && strlen(argv[i]) == 2) {
			i++; /*inaintez la urmatorul argument*/
			add_cmd_define(mappings, argv[i]);
		} else if (!strncmp(argv[i], "-D", 2) && strlen(argv[i]) > 2) {
			add_cmd_define(mappings, argv[i] + 2);
		}
	}
}

void parse_cmd_directories(char *argv[], int argc,
			   struct LinkedList *directories)
{
	for (int i = 1; i < argc; i++) {
		if (!strncmp(argv[i], "-I", 2) && strlen(argv[i]) == 2) {
			i++; /*inaintez la urmatorul argument*/
			add_cmd_directory(directories, argv[i]);
		} else if (!strncmp(argv[i], "-I", 2) && strlen(argv[i]) > 2) {
			add_cmd_directory(directories, argv[i] + 2);
		}
	}
}

void print_string_linkedlist(struct LinkedList *list)
{
	struct Node *curr;
	curr = list->head;

	if (list == NULL) {
		return;
	}

	while (curr != NULL) {
		printf("%s ", (char *)(curr->data));
		curr = curr->next;
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	char infile[PATH_LENGTH] = {'\0'}, outfile[PATH_LENGTH] = {'\0'};
	struct LinkedList *directories =
	    (struct LinkedList *)calloc(1, sizeof(struct LinkedList));
	struct Hashmap *mappings =
	    (struct Hashmap *)calloc(1, sizeof(struct Hashmap));
	// Verificat si intors codul de eroare daca esueaza calloc-ul

	init_list(directories);
	init_ht(mappings, HT_ENTRIES, hash_function_string, cmp_strings);

	parse_cmd_defines(argv, argc, mappings);
	parse_cmd_directories(argv, argc, directories);
	
	print_string_linkedlist(directories);

	char *ret = get(mappings, "DEBUG");
	printf("Value: ---%s---\n", ret);

	ret = get(mappings, "CUSTOM_DBG");
	printf("Value: ---%s---\n", ret);


	free_list(&directories);
	free_ht(mappings);

	return 0;
}