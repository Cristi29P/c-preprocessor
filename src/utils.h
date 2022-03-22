/**
 * https://ocw.cs.pub.ro/courses/so/laboratoare/resurse/die
 */
#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PATH_LENGTH 50
#define SMALL_BUFF 20
#define HT_ENTRIES 10
#define MAX_BUFF_SIZE 256

/* useful macro for handling error codes */
#define DIE(assertion, call_description)                                       \
	do {                                                                   \
		if (assertion) {                                               \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);     \
			perror(call_description);                              \
			exit(errno);                                           \
		}                                                              \
	} while (0)

int file_exists(const char *name);

int check_param(char *argv);

void replace_str(char *haystack, char *needle, char *replc);

void expand_str(char *haystack, char *needle, char *replc);

#endif /* UTILS_H_ */
