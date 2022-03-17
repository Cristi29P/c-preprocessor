#include "hashmap.h"
// Refacut makefile si includes
#define PATH_LENGTH 50
#define SMALL_BUFF 20
#define HT_ENTRIES 1000

void add_cmd_define(struct Hashmap *mappings, char *argv)
{
	char symbol[SMALL_BUFF] = {'\0'}, value[SMALL_BUFF] = {'\0'};
	int rv;

	rv = sscanf(argv, "%[^=]=%s", symbol, value);
	if (rv <= 0) {
		printf("Sscanf failed!\n");
	}
	put(mappings, symbol, strlen(symbol) + 1, value, strlen(value) + 1);
}

void add_cmd_directory(struct LinkedList *directories, char *argv)
{
	char directory[PATH_LENGTH] = {'\0'};
	int rv;

	rv = sscanf(argv, "%s", directory);
	if (rv < 0) {
		printf("Sscanf failed!\n");
	}
	add_nth_node(directories, get_size(directories), directory,
		     strlen(directory) + 1);
}

int check_param(char *argv)
{
	if (strncmp(argv, "-D", 2) || strncmp(argv, "-I", 2)) {
		return -1;
	}
	return 0;
}

int parse_cmd_arguments(struct Hashmap *mappings,
			struct LinkedList *directories, char *infile,
			char *outfile, char *argv[], int argc)
{
	int input_set = 0;
	for (int i = 1; i < argc; i++) {
		if (!strncmp(argv[i], "-D", 2) && strlen(argv[i]) == 2) {
			i++; /*inaintez la urmatorul argument*/
			add_cmd_define(mappings, argv[i]);
		} else if (!strncmp(argv[i], "-D", 2) && strlen(argv[i]) > 2) {
			add_cmd_define(mappings, argv[i] + 2);
		} else if (!strncmp(argv[i], "-I", 2) && strlen(argv[i]) == 2) {
			i++; /*inaintez la urmatorul argument*/
			add_cmd_directory(directories, argv[i]);
		} else if (!strncmp(argv[i], "-I", 2) && strlen(argv[i]) > 2) {
			add_cmd_directory(directories, argv[i] + 2);
		} else if (!input_set) {
			strncpy(infile, argv[i], PATH_LENGTH);
			input_set = 1;
		} else if ((input_set == 1) && (i == argc - 1)) {
			strncpy(outfile, argv[i], PATH_LENGTH);
		} else if (check_param(argv[i])) {
			return -1;
		}
	}
	return 0;
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
	int rv;
	char infile[PATH_LENGTH] = {'\0'}, outfile[PATH_LENGTH] = {'\0'};
	struct LinkedList *directories =
	    (struct LinkedList *)calloc(1, sizeof(struct LinkedList));

	if (directories == NULL) {
		exit(ENOMEM);
	}
	struct Hashmap *mappings =
	    (struct Hashmap *)calloc(1, sizeof(struct Hashmap));

	if (mappings == NULL) {
		exit(ENOMEM);
	}

	init_list(directories);
	init_ht(mappings, HT_ENTRIES, hash_function_string, cmp_strings);

	rv = parse_cmd_arguments(mappings, directories, infile, outfile, argv,
				 argc);
	if (rv) {
		free_list(&directories);
		free_ht(mappings);
		exit(ENOMEM);
	}

	FILE *f;
	f = fopen(infile, "r");
	if (f == NULL) {
		free_list(&directories);
		free_ht(mappings);
		exit(ENOMEM);
	}

	// printf("HT->SIZE: %d\n", get_ht_size(mappings));
	// char *rc = get(mappings, "CUSTOM_DBG");
	// printf("--%s--\n", rc);
	// printf("INFILE: --%s--\n", infile);
	// printf("OUTFILE: --%s--\n", outfile);
	// printf("DIRECTORIES: ");
	// print_string_linkedlist(directories);

	free_list(&directories);
	free_ht(mappings);

	return 0;
}
