#ifndef SO_CPP_H
#define SO_CPP_H

#include "hashmap.h"

#define PATH_LENGTH 50
#define SMALL_BUFF 20
#define HT_ENTRIES 10
#define MAX_BUFF_SIZE 256

void check_if_cond(struct Hashmap *mappings, struct LinkedList *directories,
		   FILE *infile, FILE *outfile, char *buffer, char *infile_name);

int choose_action(struct Hashmap *mappings, struct LinkedList *directories,
		   FILE *infile, FILE *outfile, char *buffer, char *infile_name);

int parse_file(struct Hashmap *mappings, struct LinkedList *directories,
	       FILE *infile, FILE *outfile, char *infile_name);

#endif
