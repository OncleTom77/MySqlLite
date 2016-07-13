#ifndef sql_h
#define sql_h

#include "includes.h"

t_hashmap_entry *get_hashmap_entry_from_JSON(char *);

t_hashmap *JSON_parse(char *);

char *get_string_from_hashmap_entry(t_hashmap_entry *);

char *JSON_stringify(t_hashmap *);

void sql_find(command_line *);

void sql_insert(command_line *);

void sql_set(command_line *);

void sql_remove(command_line *);

void list_chain_append(t_hashmap_entry **, t_hashmap_entry *);

void list_chain_free(t_hashmap_entry *);

void print_type(t_hashmap_entry *);
void print_entity_projections(t_hashmap *, t_hashmap_entry *);

int is_equal(void *, void *, type_var);
int is_matching(t_hashmap *, t_hashmap *);
void update_entity(t_hashmap *, t_hashmap *);

char *revert_substr(char *, long, long);
char *replace_substring(char *, long, long, char *);

#endif /* sql_h */
