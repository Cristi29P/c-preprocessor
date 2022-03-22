#ifndef CMD_LINE_
#define CMD_LINE_

#include "linkedlist.h"
#include "hashmap.h"

void add_cmd_directory(struct LinkedList *directories, char *argv);

int parse_cmd_arguments(struct Hashmap *mappings,
			struct LinkedList *directories, char *infile,
			char *outfile, char *argv[], int argc);

int parse_cmd_arguments(struct Hashmap *mappings,
			struct LinkedList *directories, char *infile,
			char *outfile, char *argv[], int argc);

#endif /* CMD_LINE_ */
