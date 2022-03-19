#include "hashmap.h"
// Refacut makefile si includes
// Inlocuit functii periculoase cu altele
// REMOVE SSCANF CHECKS

#define PATH_LENGTH 50
#define SMALL_BUFF 20
#define HT_ENTRIES 100
#define MAX_BUFF_SIZE 256

int file_exists(const char *name)
{
	FILE *file = fopen(name, "r");
	if (name) {
		fclose(file);
		return 1;
	}
	return 0;
}

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

void replace_str(char *haystack, char *needle, char *replc)
{
	char start[256], end[256], *last_pos;
	size_t l_origLen;

	l_origLen = strnlen(needle, 256);
	while ((last_pos = strstr(haystack, needle))) {
		memset(start, '\0', 256);
		memset(end, '\0', 256);

		memcpy(start, haystack,
		       (long unsigned int)(last_pos - haystack));
		memcpy(end, last_pos + l_origLen, strlen(last_pos + l_origLen));

		sprintf(haystack, "%s%s%s", start, replc, end);
	}
}

void add_simple_define(struct Hashmap *mappings, char *buffer)
{
	char symbol[SMALL_BUFF] = {'\0'}, value[SMALL_BUFF] = {'\0'};
	char value_copy[MAX_BUFF_SIZE] = {'\0'};
	char *delim = "\t []{}<>=+-*/%!&|^.,:;()\\", *token;

	sscanf(buffer, "#define %s %[^\n]s", symbol, value);
	strncpy(value_copy, value, SMALL_BUFF);

	token = strtok(value, delim);
	while (token != NULL) {
		if (has_key(mappings, token)) {
			replace_str(value_copy, token, get(mappings, token));
		}
		token = strtok(NULL, delim);
	}

	put(mappings, symbol, strnlen(symbol, SMALL_BUFF) + 1, value_copy,
	    strnlen(value_copy, MAX_BUFF_SIZE) + 1);
}

void add_text_define(struct Hashmap *mappings, FILE *infile, char *buffer)
{
	if (buffer[strlen(buffer) - 2] == '\\') {
		// execute multi-line define procedure
	} else {
		add_simple_define(mappings, buffer);
	}
}
void solve_simple_line_sub(struct Hashmap *mappings, FILE *outfile,
			   char *buffer)
{
	char value_copy[MAX_BUFF_SIZE] = {'\0'};
	char *delim = "\t []{}<>=+-*/%!&|^.,:;()\\", *token;

	strncpy(value_copy, buffer, MAX_BUFF_SIZE);

	token = strtok(buffer, delim);
	while (token != NULL) {
		if (has_key(mappings, token)) {
			replace_str(value_copy, token, get(mappings, token));
		}
		token = strtok(NULL, delim);
	}

	fprintf(outfile, "%s", value_copy);
}

void parse_file(struct Hashmap *mappings, struct LinkedList *directories,
		FILE *infile, FILE *outfile)
{
	char buffer[MAX_BUFF_SIZE] = {'\0'};

	while (fgets(buffer, MAX_BUFF_SIZE, infile)) {
		if (!strncmp(buffer, "#define", 7)) {
			add_text_define(mappings, infile, buffer);
		} else {
			solve_simple_line_sub(mappings, outfile, buffer);
		}
		// prelucrare si afisare restul de siruri in fisier sau la
		// iesirea standard
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
	int rv;
	char infile[PATH_LENGTH] = {'\0'}, outfile[PATH_LENGTH] = {'\0'};
	FILE *input_file, *output_file;

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

	/*Parse cmd line arguments and check if there is any invalid falig*/
	rv = parse_cmd_arguments(mappings, directories, infile, outfile, argv,
				 argc);
	if (rv) {
		free_list(&directories);
		free_ht(mappings);
		exit(ENOMEM);
	}

	/*Opening and checking the input/output files*/
	if (strlen(infile)) {
		input_file = fopen(infile, "r");
		if (input_file == NULL) {
			free_list(&directories);
			free_ht(mappings);
			exit(ENOMEM);
		}
	} else {
		input_file = stdin;
	}

	if (strlen(outfile)) {
		output_file = fopen(outfile, "w");
		if (output_file == NULL) {
			fclose(input_file);
			free_list(&directories);
			free_ht(mappings);
			exit(ENOMEM);
		}
	} else {
		output_file = stdout;
	}
	/*Finished the checking phase*/

	parse_file(mappings, directories, input_file, output_file);

	/*Memory clean-up*/
	fclose(input_file);
	fclose(output_file);
	free_list(&directories);
	free_ht(mappings);

	return 0;
}
