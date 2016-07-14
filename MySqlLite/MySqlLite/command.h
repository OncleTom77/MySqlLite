#ifndef command_h
#define command_h

#include "includes.h"

typedef enum action {
    A_NONE,
    A_FIND,
    A_INSERT,
    A_SET,
    A_REMOVE
} action;

typedef struct command_line {

    char *collection;
    action act;
    t_hashmap *action_value;
    t_hashmap *where_value;
    t_hashmap_entry *projection_value;
    t_hashmap *sort_value;
    
} command_line;

command_line *analyse_arguments(int, char **);

int get_option(command_line *, char *, char *);

char *get_value(char *, unsigned long);

void command_line_free(command_line **);

#endif /* command_h */
