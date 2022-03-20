#include "so-cpp.h"
// Refacut makefile si includes
// Inlocuit functii periculoase cu altele
// ADD SSCANF CHECKS
// ADD COMMENTS IN THE WHOLE CODE BASE AND CHECK FOR ANY COMMENTS OR
// UNNECCESSARY DEAD CODE FURTHER REFACTORING
// REFACUT SO_CPP.h

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
	add_node(directories, list_size(directories), directory,
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
	char start[MAX_BUFF_SIZE], end[MAX_BUFF_SIZE], *last_pos;
	size_t len;

	len = strnlen(needle, MAX_BUFF_SIZE);
	while ((last_pos = strstr(haystack, needle))) {
		memset(start, '\0', MAX_BUFF_SIZE);
		memset(end, '\0', MAX_BUFF_SIZE);

		memcpy(start, haystack, (unsigned long)(last_pos - haystack));
		memcpy(end, last_pos + len, strlen(last_pos + len));

		sprintf(haystack, "%s%s%s", start, replc, end);
	}
}

void define_symbol(struct Hashmap *mappings, FILE *infile, char *buffer)
{
	char symbol[SMALL_BUFF] = {'\0'}, value[SMALL_BUFF] = {'\0'};
	char value_copy[MAX_BUFF_SIZE] = {'\0'};
	char final[MAX_BUFF_SIZE] = {'\0'};
	char *delim = "\t []{}<>=+-*/%!&|^.,:;()\\", *token;

	sscanf(buffer, "#define %s %[^\n]s", symbol, value);
	strncpy(value_copy, value, SMALL_BUFF);

	while (value[strlen(value) - 1] == '\\') {
		token = strtok(value, delim);
		while (token != NULL) {
			if (has_key(mappings, token)) {
				replace_str(value_copy, token,
					    get(mappings, token));
			}
			token = strtok(NULL, delim);
		}
		value_copy[strlen(value_copy) - 1] = '\0';
		strncat(final, value_copy, MAX_BUFF_SIZE);

		memset(value, '\0', SMALL_BUFF);
		memset(value_copy, '\0', MAX_BUFF_SIZE);

		fscanf(infile, " %[^\n]s", value);
		strncpy(value_copy, value, SMALL_BUFF);
	}

	token = strtok(value, delim);
	while (token != NULL) {
		if (has_key(mappings, token)) {
			replace_str(value_copy, token, get(mappings, token));
		}
		token = strtok(NULL, delim);
	}
	strncat(final, value_copy, MAX_BUFF_SIZE);

	put(mappings, symbol, strnlen(symbol, SMALL_BUFF) + 1, final,
	    strnlen(final, MAX_BUFF_SIZE) + 1);
}

void solve_simple_line_sub(struct Hashmap *mappings, FILE *outfile,
			   char *buffer)
{
	char value_copy[MAX_BUFF_SIZE] = {'\0'};
	char *delim = "\n\t []{}<>=+-*/%!&|^.,:;()\\", *token;

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

void undefine_symbol(struct Hashmap *mappings, char *buffer)
{
	char symbol[MAX_BUFF_SIZE] = {'\0'};
	sscanf(buffer, "#undef %[^\n]s", symbol);

	if (has_key(mappings, symbol)) {
		remove_ht_entry(mappings, symbol);
	}
}

void choose_action(struct Hashmap *mappings, FILE *infile, FILE *outfile,
		   char *buffer)
{
	if (!strncmp(buffer, "#define", 7)) {
		define_symbol(mappings, infile, buffer);
	} else if (!strncmp(buffer, "#undef", 6)) {
		undefine_symbol(mappings, buffer);
	} else if (!strncmp(buffer, "#ifdef", 6)) {
		replace_str(buffer, "#ifdef", "#if");
		check_if_cond(mappings, infile, outfile, buffer);
	} else if (!strncmp(buffer, "#if", 3)) {
		check_if_cond(mappings, infile, outfile, buffer);
	} else {
		solve_simple_line_sub(mappings, outfile, buffer);
	}
}

void go_to_endif(FILE *infile)
{
	char buffer[MAX_BUFF_SIZE] = {'\0'};
	while (strncmp(buffer, "#endif", 6)) {
		fgets(buffer, MAX_BUFF_SIZE, infile);
	}
}

void solve_if(struct Hashmap *mappings, FILE *infile, FILE *outfile)
{
	char new_line[MAX_BUFF_SIZE] = {'\0'};
	fgets(new_line, MAX_BUFF_SIZE, infile);
	do {
		choose_action(mappings, infile, outfile, new_line);
		fgets(new_line, MAX_BUFF_SIZE, infile);
	} while (strncmp(new_line, "#endif", 6) &&
		 strncmp(new_line, "#else", 5) &&
		 strncmp(new_line, "#elif", 5));
	/*Nu am ajuns la #endif, ne ducem acolo*/
	if (strncmp(new_line, "#endif", 6)) {
		go_to_endif(infile);
	}
}

void check_if_cond(struct Hashmap *mappings, FILE *infile, FILE *outfile,
		   char *buffer)
{
	char cond[MAX_BUFF_SIZE] = {'\0'}, cond_aux[MAX_BUFF_SIZE] = {'\0'},
	     *aux;
	char new_line[MAX_BUFF_SIZE] = {'\0'};
	char *ret;
	long value_cond;

	sscanf(buffer, "#if %[^\n]s", cond);
	if (has_key(mappings, cond)) {
		strncpy(cond_aux, cond, MAX_BUFF_SIZE);
		memset(cond, '\0', MAX_BUFF_SIZE);
		ret = (char *)get(mappings, cond_aux);
		
		if (!strlen(ret)) {
			strncpy(cond, "1", MAX_BUFF_SIZE);
		} else {
			strncpy(cond, ret, MAX_BUFF_SIZE);
		}
		
	}

	value_cond = strtol(cond, &aux, 10);
	strncpy(new_line, buffer, MAX_BUFF_SIZE);
	if (value_cond) {
		solve_if(mappings, infile, outfile);
	} else {
		while (strncmp(new_line, "#endif", 6) &&
		       strncmp(new_line, "#else", 5) &&
		       strncmp(new_line, "#elif", 5)) {
			fgets(new_line, MAX_BUFF_SIZE, infile);
		}

		if (!strncmp(new_line, "#else", 5)) {
			fgets(new_line, MAX_BUFF_SIZE, infile);
			do {
				choose_action(mappings, infile, outfile,
					      new_line);
				fgets(new_line, MAX_BUFF_SIZE, infile);
			} while (strncmp(new_line, "#endif", 6));

		} else if (!strncmp(new_line, "#elif", 5)) {
			replace_str(new_line, "#elif", "#if");
			check_if_cond(mappings, infile, outfile, new_line);
		}
	}
}

void parse_file(struct Hashmap *mappings, struct LinkedList *directories,
		FILE *infile, FILE *outfile)
{
	char buffer[MAX_BUFF_SIZE] = {'\0'};

	while (fgets(buffer, MAX_BUFF_SIZE, infile)) {
		choose_action(mappings, infile, outfile, buffer);
	}
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

	list_init(directories);
	init_ht(mappings, HT_ENTRIES, hash_function_string, cmp_strings);

	/*Parse cmd line arguments and check if there is any invalid falig*/
	rv = parse_cmd_arguments(mappings, directories, infile, outfile, argv,
				 argc);
	if (rv) {
		free_list_mem(&directories);
		free_ht(mappings);
		exit(ENOMEM);
	}

	/*Opening and checking the input/output files*/
	if (strlen(infile)) {
		input_file = fopen(infile, "r");
		if (input_file == NULL) {
			free_list_mem(&directories);
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
			free_list_mem(&directories);
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
	free_list_mem(&directories);
	free_ht(mappings);

	return 0;
}
