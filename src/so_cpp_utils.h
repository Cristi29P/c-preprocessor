#ifndef SO_CPP_UTILS_H
#define SO_CPP_UTILS_H

#include "hashmap.h"

void define_symbol(struct Hashmap *mappings, void *infile, char *buffer);

void solve_simple_line_sub(struct Hashmap *mappings, FILE *outfile,
			   char *buffer);

void undefine_symbol(struct Hashmap *mappings, char *buffer);

int solve_include(struct Hashmap *mappings, struct LinkedList *directories,
		  FILE *infile, FILE *outfile,
		  char *buffer, char *infile_name);

void go_to_endif(FILE *infile);

void solve_if(struct Hashmap *mappings, struct LinkedList *directories,
	      FILE *infile, FILE *outfile, char *infile_name);

void check_if_cond(struct Hashmap *mappings, struct LinkedList *directories,
		FILE *infile, FILE *outfile,
		char *buffer, char *infile_name);

int choose_action(struct Hashmap *mappings, struct LinkedList *directories,
		FILE *infile, FILE *outfile,
		char *buffer, char *infile_name);

int parse_file(struct Hashmap *mappings, struct LinkedList *directories,
	    FILE *infile, FILE *outfile,
		char *infile_name);

#endif /* SO_CPP_UTILS_H */
