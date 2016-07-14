#ifndef sql_h
#define sql_h

#include "includes.h"

void sql_find(command_line *);

void sql_insert(command_line *);

void sql_set(command_line *);

void sql_remove(command_line *);

void print_entity_projections(t_hashmap *, t_hashmap_entry *);

int is_matching(t_hashmap *, t_hashmap *);

void update_entity(t_hashmap *, t_hashmap *);

#endif /* sql_h */
