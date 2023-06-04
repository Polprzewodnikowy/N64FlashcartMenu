#ifndef STR_UTILS_H__
#define STR_UTILS_H__


#include <stdio.h>
//#include <stdlib.h> // TODO: does this work... will unlock qsort!
#include <string.h>

// e.g. if (str_endswith(cur_rom, ".z64") || str_endswith(cur_rom, ".n64"))
static bool str_endswith(const char *str, const char *suffix) {
	char *p = strstr(str, suffix);
	return p && p[strlen(suffix)] == '\0';
}

#endif