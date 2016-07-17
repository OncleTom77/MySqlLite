#ifndef json_h
#define json_h

#include "includes.h"

t_hashmap_entry *get_hashmap_entry_from_JSON(char *);

t_hashmap *JSON_parse(char *);

t_hashmap_entry *JSON_parse_list(char *);

char *JSON_stringify(t_hashmap *);

#endif /* json_h */