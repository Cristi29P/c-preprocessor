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

	if (file) {
		fclose(file);
		return 1;
	}
	return 0;
}

void add_cmd_define(struct Hashmap *mappings, char *argv)
{
	char symbol[MAX_BUFF_SIZE] = {'\0'}, value[MAX_BUFF_SIZE] = {'\0'};
	int rv;

	rv = sscanf(argv, "%[^=]=%s", symbol, value);
	if (rv <= 0) {
		printf("Sscanf failed!\n");
		exit(-1);
	}
	put(mappings, symbol, strlen(symbol) + 1, value, strlen(value) + 1);
}

void add_cmd_directory(struct LinkedList *directories, char *argv)
{
	char directory[PATH_LENGTH] = {'\0'};
	int rv;

	rv = sscanf(argv, "%s", directory);
	if (rv < 0)
		printf("Sscanf failed!\n");

	add_node(directories, list_size(directories), directory,
		 strlen(directory) + 1);
}

int check_param(char *argv)
{
	if (strncmp(argv, "-D", 2) || strncmp(argv, "-I", 2))
		return -1;

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

void expand_str(char *haystack, char *needle, char *replc)
{
	char start[MAX_BUFF_SIZE] = {'\0'}, end[MAX_BUFF_SIZE] = {'\0'},
	     *last_pos;

	size_t len = 0;

	last_pos = NULL;

	len = strnlen(needle, MAX_BUFF_SIZE);
	last_pos = strstr(haystack, needle);
	while (last_pos) {
		memset(start, '\0', MAX_BUFF_SIZE);
		memset(end, '\0', MAX_BUFF_SIZE);

		memcpy(start, haystack, (unsigned long)(last_pos - haystack));
		memcpy(end, last_pos + len, strlen(last_pos + len));

		sprintf(haystack, "%s%s%s", start, replc, end);
		last_pos =
		    strstr(haystack + (unsigned long)(last_pos - haystack) +
			       strlen(replc),
			   needle);
	}
}

void replace_str(char *haystack, char *needle, char *replc)
{
	char str1[MAX_BUFF_SIZE] = {'\0'}, str2[MAX_BUFF_SIZE] = {'\0'},
	     str3[MAX_BUFF_SIZE] = {'\0'};
	char haystack_cpy[MAX_BUFF_SIZE] = {'\0'};
	char *pos1, *pos2;
	int rt;

	strncpy(haystack_cpy, haystack, MAX_BUFF_SIZE);

	pos1 = strstr(haystack_cpy, "\"");

	if (pos1 != NULL) {
		pos1++;
		pos2 = strstr(pos1, "\"") + 1;

		rt = sscanf(haystack_cpy, "%[^\"]s", str1);
		if (!rt)
			exit(-1);
		rt = sscanf(pos1, "%[^\"]s", str2);
		if (!rt)
			exit(-1);
		rt = sscanf(pos2, "%[^\"]n", str3);
		if (!rt)
			exit(-1);

		expand_str(str1, needle, replc);
		expand_str(str3, needle, replc);
		sprintf(haystack, "%s\"%s\"%s", str1, str2, str3);
	} else {
		expand_str(haystack_cpy, needle, replc);
		sprintf(haystack, "%s", haystack_cpy);
	}
}

void define_symbol(struct Hashmap *mappings, FILE *infile, char *buffer)
{
	char symbol[MAX_BUFF_SIZE] = {'\0'}, value[MAX_BUFF_SIZE] = {'\0'};
	char value_copy[MAX_BUFF_SIZE] = {'\0'};
	char final[MAX_BUFF_SIZE] = {'\0'};
	char *delim = "\t []{}<>=+-*/%!&|^.,:;()\\", *token;
	int rt;

	rt = sscanf(buffer, "#define %s %[^\n]s", symbol, value);
	if (!rt)
		exit(-12);
	strncpy(value_copy, value, MAX_BUFF_SIZE);

	if (strlen(value) == 0) {
		put(mappings, symbol, strnlen(symbol, MAX_BUFF_SIZE) + 1, "",
		    1);
		return;
	}

	while (value[strlen(value) - 1] == '\\') {
		token = strtok(value, delim);
		while (token != NULL) {
			if (has_key(mappings, token))
				replace_str(value_copy, token,
					    get(mappings, token));

			token = strtok(NULL, delim);
		}
		value_copy[strlen(value_copy) - 1] = '\0';
		strncat(final, value_copy, MAX_BUFF_SIZE);

		memset(value, '\0', MAX_BUFF_SIZE);
		memset(value_copy, '\0', MAX_BUFF_SIZE);

		fscanf(infile, " %[^\n]s", value);
		strncpy(value_copy, value, MAX_BUFF_SIZE);
	}

	token = strtok(value, delim);
	while (token != NULL) {
		if (has_key(mappings, token))
			replace_str(value_copy, token,
				    (char *)get(mappings, token));

		token = strtok(NULL, delim);
	}
	strncat(final, value_copy, MAX_BUFF_SIZE);
	put(mappings, symbol, strnlen(symbol, MAX_BUFF_SIZE) + 1, final,
	    strnlen(final, MAX_BUFF_SIZE) + 1);
}

void solve_simple_line_sub(struct Hashmap *mappings, FILE *outfile,
			   char *buffer)
{
	char value_copy[MAX_BUFF_SIZE] = {'\0'};
	char *delim = "\t []{}<>=+-*/%!&|^.,:;()\\", *token;

	strncpy(value_copy, buffer, MAX_BUFF_SIZE);
	token = strtok(buffer, delim);
	while (token != NULL) {
		if (has_key(mappings, token))
			replace_str(value_copy, token,
				    (char *)get(mappings, token));

		token = strtok(NULL, delim);
	}
	fprintf(outfile, "%s", value_copy);
}

void undefine_symbol(struct Hashmap *mappings, char *buffer)
{
	char symbol[MAX_BUFF_SIZE] = {'\0'};
	int rv;

	rv = sscanf(buffer, "#undef %[^\n]s", symbol);
	if (!rv)
		return;

	if (has_key(mappings, symbol))
		remove_ht_entry(mappings, symbol);
}

int solve_include(struct Hashmap *mappings, struct LinkedList *directories,
		  FILE *infile, FILE *outfile, char *buffer, char *infile_name)
{
	char file_name[PATH_LENGTH] = {'\0'}, full_path[MAX_BUFF_SIZE] = {'\0'};
	char *ptr;
	struct Node *tmp_dir;
	FILE *new_infile;
	int ret, i, flag = 1;

	ret = sscanf(buffer, "#include \"%[^\"]s", file_name);
	if (!ret)
		return -1;

	if (infile == stdin) {
		snprintf(full_path, MAX_BUFF_SIZE, "./%s", file_name);

		if (!file_exists(full_path))
			return -1;

		new_infile = fopen(full_path, "r");
		if (new_infile == NULL)
			return -1;

		ret = parse_file(mappings, directories, new_infile, outfile,
				 full_path);
		if (ret) {
			fclose(new_infile);
			return ret;
		}
		fclose(new_infile);
	} else if (strlen(infile_name) != 0) {
		ptr = strrchr(infile_name, '/') + 1;
		strncpy(full_path, infile_name, PATH_LENGTH);
		replace_str(full_path, ptr, file_name);

		if (!file_exists(full_path))
			flag = 0;

		if (flag) {
			new_infile = fopen(full_path, "r");
			if (new_infile == NULL)
				return -1;

			ret = parse_file(mappings, directories, new_infile,
					 outfile, full_path);
			if (ret == -1) {
				fclose(new_infile);
				return ret;
			}
			fclose(new_infile);
		} else {
			if (list_size(directories) == -1)
				return -1;

			for (i = 0; i < list_size(directories); i++) {
				tmp_dir = get_node(directories, i);
				sprintf(full_path, "%s/%s",
					(char *)tmp_dir->data, file_name);
				if (file_exists(full_path)) {
					new_infile = fopen(full_path, "r");
					if (new_infile == NULL)
						return -1;

					ret = parse_file(mappings, directories,
							 new_infile, outfile,
							 full_path);
					if (ret == -1) {
						fclose(new_infile);
						return -1;
					}
					fclose(new_infile);
					return 0;
				}
			}
			return -1;
		}
	}
	return 0;
}

int choose_action(struct Hashmap *mappings, struct LinkedList *directories,
		  FILE *infile, FILE *outfile, char *buffer, char *infile_name)
{
	char aux[MAX_BUFF_SIZE] = {'\0'};
	int rv;

	if (!strncmp(buffer, "#define", 7))
		define_symbol(mappings, infile, buffer);
	else if (!strncmp(buffer, "#undef", 6))
		undefine_symbol(mappings, buffer);
	else if (!strncmp(buffer, "#ifdef", 6)) {
		rv = sscanf(buffer, "#ifdef %[^\n]s", aux);
		if (!rv)
			return -1;
		if (has_key(mappings, aux))
			snprintf(buffer, MAX_BUFF_SIZE, "#if 1");
		else
			snprintf(buffer, MAX_BUFF_SIZE, "#if 0");

		check_if_cond(mappings, directories, infile, outfile, buffer,
			      infile_name);
	} else if (!strncmp(buffer, "#ifndef", 7)) {
		rv = sscanf(buffer, "#ifndef %[^\n]s", aux);
		if (!rv)
			return -1;
		if (has_key(mappings, aux))
			snprintf(buffer, MAX_BUFF_SIZE, "#if 0");
		else
			snprintf(buffer, MAX_BUFF_SIZE, "#if 1");

		check_if_cond(mappings, directories, infile, outfile, buffer,
			      infile_name);
	} else if (!strncmp(buffer, "#if", 3))
		check_if_cond(mappings, directories, infile, outfile, buffer,
			      infile_name);
	else if (!strncmp(buffer, "#include", 8)) {
		rv = solve_include(mappings, directories, infile, outfile,
				   buffer, infile_name);
		if (rv)
			return -1;

	} else
		solve_simple_line_sub(mappings, outfile, buffer);

	return 0;
}

void go_to_endif(FILE *infile)
{
	char buffer[MAX_BUFF_SIZE] = {'\0'};

	while (strncmp(buffer, "#endif", 6))
		fgets(buffer, MAX_BUFF_SIZE, infile);
}

void solve_if(struct Hashmap *mappings, struct LinkedList *directories,
	      FILE *infile, FILE *outfile, char *infile_name)
{
	char new_line[MAX_BUFF_SIZE] = {'\0'};

	fgets(new_line, MAX_BUFF_SIZE, infile);
	do {
		choose_action(mappings, directories, infile, outfile, new_line,
			      infile_name);
		fgets(new_line, MAX_BUFF_SIZE, infile);
	} while (strncmp(new_line, "#endif", 6) &&
		 strncmp(new_line, "#else", 5) &&
		 strncmp(new_line, "#elif", 5));
	if (strncmp(new_line, "#endif", 6))
		go_to_endif(infile);
}

void check_if_cond(struct Hashmap *mappings, struct LinkedList *directories,
		   FILE *infile, FILE *outfile, char *buffer, char *infile_name)
{
	char cond[MAX_BUFF_SIZE] = {'\0'}, cond_aux[MAX_BUFF_SIZE] = {'\0'},
	     new_line[MAX_BUFF_SIZE] = {'\0'}, *aux, *ret;
	long value_cond;
	int rv;

	rv = sscanf(buffer, "#if %[^\n]s", cond);
	if (!rv)
		exit(-1);

	if (has_key(mappings, cond)) { /*daca exista cheia in mapa*/
		strncpy(cond_aux, cond, MAX_BUFF_SIZE);
		memset(cond, '\0', MAX_BUFF_SIZE);
		ret = (char *)get(mappings, cond_aux);

		if (!strlen(ret))
			strncpy(cond, "1", MAX_BUFF_SIZE);
		else
			strncpy(cond, ret, MAX_BUFF_SIZE);
	}

	value_cond = strtol(cond, &aux, 10);
	if (value_cond == 0 && cond[0] != '0') {
		value_cond = 1;
		printf("AICI\n");
	}

	strncpy(new_line, buffer, MAX_BUFF_SIZE);
	if (value_cond) {
		solve_if(mappings, directories, infile, outfile, infile_name);
	} else {
		while (strncmp(new_line, "#endif", 6) &&
		       strncmp(new_line, "#else", 5) &&
		       strncmp(new_line, "#elif", 5)) {
			fgets(new_line, MAX_BUFF_SIZE, infile);
		}

		if (!strncmp(new_line, "#else", 5)) {
			fgets(new_line, MAX_BUFF_SIZE, infile);
			do {
				choose_action(mappings, directories, infile,
					      outfile, new_line, infile_name);
				fgets(new_line, MAX_BUFF_SIZE, infile);
			} while (strncmp(new_line, "#endif", 6));
		} else if (!strncmp(new_line, "#elif", 5)) {
			replace_str(new_line, "#elif", "#if");
			check_if_cond(mappings, directories, infile, outfile,
				      new_line, infile_name);
		}
	}
}

int parse_file(struct Hashmap *mappings, struct LinkedList *directories,
	       FILE *infile, FILE *outfile, char *infile_name)
{
	char buffer[MAX_BUFF_SIZE] = {'\0'};

	while (fgets(buffer, MAX_BUFF_SIZE, infile)) {
		if (choose_action(mappings, directories, infile, outfile,
				  buffer, infile_name))
			return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int rv;
	char infile[PATH_LENGTH] = {'\0'}, outfile[PATH_LENGTH] = {'\0'};
	FILE *input_file, *output_file;

	struct LinkedList *directories =
	    (struct LinkedList *)calloc(1, sizeof(struct LinkedList));

	if (directories == NULL)
		exit(ENOMEM);

	struct Hashmap *mappings =
	    (struct Hashmap *)calloc(1, sizeof(struct Hashmap));

	if (mappings == NULL)
		exit(ENOMEM);

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

	rv = parse_file(mappings, directories, input_file, output_file, infile);

	/*Memory clean-up*/
	fclose(input_file);
	fclose(output_file);
	free_list_mem(&directories);
	free_ht(mappings);

	return rv;
}
