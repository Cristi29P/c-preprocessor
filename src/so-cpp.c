#include "utils.h"
#include "so_cpp_utils.h"
#include "cmd_line.h"
#include "hashmap.h"
#include "linkedlist.h"

int main(int argc, char *argv[])
{
	/*Variable definitions*/
	int rv;
	char infile[PATH_LENGTH] = {'\0'}, outfile[PATH_LENGTH] = {'\0'};
	FILE *input_file, *output_file;

	/*Allocate memory for the DS's*/
	struct LinkedList *directories = calloc(1, sizeof(struct LinkedList));
	struct Hashmap *mappings = calloc(1, sizeof(struct Hashmap));

	/*Check if the memory for the DS's was successfully allocated*/
	if (directories == NULL)
		exit(ENOMEM);

	if (mappings == NULL)
		exit(ENOMEM);

	/*Init the DS's*/
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
		/*We read from stdin if there is no input file*/
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
		/*We write to stdout if there is no output file*/
		output_file = stdout;
	}
	/*Finished the checking phase and we proceed to parsing the file*/
	rv = parse_file(mappings, directories, input_file, output_file, infile);

	/*Memory clean-up*/
	fclose(input_file);
	fclose(output_file);
	free_list_mem(&directories);
	free_ht(mappings);

	return rv;
}
