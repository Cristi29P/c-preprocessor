#ifndef SO_CPP_H
#define SO_CPP_H

#include "hashmap.h"

void check_if_cond(struct Hashmap *mappings, struct LinkedList *directories,
		FILE * infile, FILE * outfile,
		char *buffer, char *infile_name);

int choose_action(struct Hashmap *mappings, struct LinkedList *directories,
		FILE *infile, FILE *outfile,
		char *buffer, char *infile_name);

int parse_file(struct Hashmap *mappings, struct LinkedList *directories,
	    FILE *infile, FILE *outfile,
		char *infile_name);

#endif
