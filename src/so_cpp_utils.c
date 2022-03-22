#define _CRT_SECURE_NO_WARNINGS
#include "so_cpp_utils.h"

void define_symbol(struct Hashmap *mappings, void *infile, char *buffer)
{
	char symbol[MAX_BUFF_SIZE] = {'\0'}, value[MAX_BUFF_SIZE] = {'\0'},
	     value_copy[MAX_BUFF_SIZE] = {'\0'}, final[MAX_BUFF_SIZE] = {'\0'},
	     *delim = "\t []{}<>=+-*/%!&|^.,:;()\\", *token;
	int rt;

	rt = sscanf(buffer, "#define %s %[^\n]s", symbol, value);
	if (!rt)
		exit(-1);

	strncpy(value_copy, value, MAX_BUFF_SIZE);

	/*If the define has no associated value we add it to the map*/
	/*with the empty null as value for the key*/
	if (strlen(value) == 0) {
		put(mappings, symbol, strnlen(symbol, MAX_BUFF_SIZE) + 1, "",
		    1);
		return;
	}

	/*We check for multiline defines by comparing the last character*/
	/*of each line with '\\'*/
	while (value[strlen(value) - 1] == '\\') {
		token = strtok(value, delim);
		/*We tokenize the line so we make sure*/
		/* there is no macro that needs to be expanded*/
		/* inside another macro*/
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

		fscanf((FILE *)infile, " %[^\n]s", value);
		strncpy(value_copy, value, MAX_BUFF_SIZE);
	}

	/*If no multiline define we do the basic macro expanding*/
	/*and substitution*/
	token = strtok(value, delim);
	while (token != NULL) {
		if (has_key(mappings, token))
			replace_str(value_copy, token,
				    (char *)get(mappings, token));

		token = strtok(NULL, delim);
	}

	/*We add the final value expanded value to the hashmap*/
	strncat(final, value_copy, MAX_BUFF_SIZE);
	put(mappings, symbol, strnlen(symbol, MAX_BUFF_SIZE) + 1, final,
	    strnlen(final, MAX_BUFF_SIZE) + 1);
}

void solve_simple_line_sub(struct Hashmap *mappings, FILE *outfile,
			   char *buffer)
{
	char value_copy[MAX_BUFF_SIZE] = {'\0'};
	char *delim = "\t []{}<>=+-*/%!&|^.,:;()\\", *token;

	/*We expand all the available tokens that are registered*/
	/*as macros inside the hashtable*/
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
	/*Remove key_value entry from hashmap*/
	if (has_key(mappings, symbol))
		remove_ht_entry(mappings, symbol);
}

/**
* The functions follows the logic: firstly check if we read from stdin
* (that means that we search for the header file in the current dir).
* If we read from an input file we search recursively in all the
* included headers and the given directories
*/
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
		sprintf(full_path, "./%s", file_name);

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
/**
 * Parse each type of action and choose the requiered function
 * The function can make recursive calls to parse other files
 */
int choose_action(struct Hashmap *mappings, struct LinkedList *directories,
		  FILE *infile, FILE *outfile, char *buffer, char *infile_name)
{
	char aux[MAX_BUFF_SIZE] = {'\0'};
	int rv;
	/*Parse each type of action*/
	if (!strncmp(buffer, "#define", 7))
		define_symbol(mappings, infile, buffer);
	else if (!strncmp(buffer, "#undef", 6))
		undefine_symbol(mappings, buffer);
	else if (!strncmp(buffer, "#ifdef", 6)) {
		rv = sscanf(buffer, "#ifdef %[^\n]s", aux);
		if (!rv)
			return -1;
		if (has_key(mappings, aux))
			sprintf(buffer, "#if 1");
		else
			sprintf(buffer, "#if 0");

		check_if_cond(mappings, directories, infile, outfile, buffer,
			      infile_name);
	} else if (!strncmp(buffer, "#ifndef", 7)) {
		rv = sscanf(buffer, "#ifndef %[^\n]s", aux);
		if (!rv)
			return -1;
		if (has_key(mappings, aux))
			sprintf(buffer, "#if 0");
		else
			sprintf(buffer, "#if 1");

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

/*If we have finished parsing an #if condition, we skip
 * the rest of the body and go straight to the end
 */
void go_to_endif(FILE *infile)
{
	char buffer[MAX_BUFF_SIZE] = {'\0'};

	while (strncmp(buffer, "#endif", 6))
		fgets(buffer, MAX_BUFF_SIZE, infile);
}

/*Solves the #if, #ifdef, #ifndef conditions*/
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

	/*Extract the condition*/
	if (has_key(mappings, cond)) {
		strncpy(cond_aux, cond, MAX_BUFF_SIZE);
		memset(cond, '\0', MAX_BUFF_SIZE);
		ret = (char *)get(mappings, cond_aux);

		if (!strlen(ret))
			strncpy(cond, "1", MAX_BUFF_SIZE);
		else
			strncpy(cond, ret, MAX_BUFF_SIZE);
	}
	/*Check if the condition is a string or number*/
	value_cond = strtol(cond, &aux, 10);
	if (value_cond == 0 && cond[0] != '0')
		value_cond = 1;

	/*Solve the if-else case and call again the function*/
	/*If you hit an #elif*/
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

/*Parse the input file and feed the lines to the function
 * which handles the parsing
 */
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
