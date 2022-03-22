#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"

int file_exists(const char *name)
{
	FILE *file = fopen(name, "r");

	if (file) {
		fclose(file);
		return 1;
	}
	return 0;
}

int check_param(char *argv)
{
	if (strncmp(argv, "-D", 2) || strncmp(argv, "-I", 2))
		return -1;

	return 0;
}

/**
 * Recursively expand the places where the needle occurs
 * and replace it
 */
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

/**
 * Split the haystack in 3 strings and expand the outer parts
 * so we don't have to deal with the " "" " case
 */
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
