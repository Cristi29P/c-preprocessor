#ifndef SO_CPP_H
#define SO_CPP_H


#include "hashmap.h"

#define PATH_LENGTH 50
#define SMALL_BUFF 20
#define HT_ENTRIES 100
#define MAX_BUFF_SIZE 256

void check_if_cond(struct Hashmap *mappings, FILE *infile, FILE *outfile,
		   char *buffer);

void choose_action(struct Hashmap *mappings, FILE *infile, FILE *outfile,
		   char *buffer);

#endif