#define _CRT_SECURE_NO_WARNINGS
#include "cmd_line.h"

/*Add the included define with -D to the map*/
void add_cmd_define(struct Hashmap *mappings, char *argv)
{
	char symbol[MAX_BUFF_SIZE] = {'\0'}, value[MAX_BUFF_SIZE] = {'\0'};
	int rv;

	rv = sscanf(argv, "%[^=]=%s", symbol, value);
	if (!rv)
		exit(-1);

	put(mappings, symbol, strlen(symbol) + 1, value, strlen(value) + 1);
}

/*Add the included directory with -I to the list*/
void add_cmd_directory(struct LinkedList *directories, char *argv)
{
	char directory[PATH_LENGTH] = {'\0'};
	int rv;

	rv = sscanf(argv, "%s", directory);
	if (rv < 0)
		exit(-1);

	add_node(directories, list_size(directories), directory,
		 strlen(directory) + 1);
}


int parse_cmd_arguments(struct Hashmap *mappings,
			struct LinkedList *directories, char *infile,
			char *outfile, char *argv[], int argc)
{
	int input_set = 0, i;

	for (i = 1; i < argc; i++) {
		if (!strncmp(argv[i], "-D", 2) && strlen(argv[i]) == 2) {
			/*we go to next argument if*/
			/*if we have -D SYM=VAL*/
			i++;
			add_cmd_define(mappings, argv[i]);
		} else if (!strncmp(argv[i], "-D", 2) && strlen(argv[i]) > 2) {
			add_cmd_define(mappings, argv[i] + 2);
		} else if (!strncmp(argv[i], "-I", 2) && strlen(argv[i]) == 2) {
			/*we go to next argument if*/
			/*if we have -I SYM=VAL*/
			i++;
			add_cmd_directory(directories, argv[i]);
		} else if (!strncmp(argv[i], "-I", 2) && strlen(argv[i]) > 2) {
			add_cmd_directory(directories, argv[i] + 2);
		} else if (!input_set) {
			/*we check if we have already*/
			/*parsed the input file and set the flag*/
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
