#ifndef utils_h
#define utils_h

#include "includes.h"

int get_hash_code(char *);

int is_equal(void *, void *, type_var);

char *revert_substr(char *, long, long);

char *replace_substring(char *, long, long, char *);

#endif /* utils_h */