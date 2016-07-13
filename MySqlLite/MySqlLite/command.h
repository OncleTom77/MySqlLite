#ifndef command_h
#define command_h

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
    char *action_value;
    char *where_value;
    char *projection_value;
    char *sort_value;
    
} command_line;

command_line *analyse_arguments(int, char **);

int get_option(command_line *, char *, char *);

char *get_value(char *, unsigned long);

#endif /* command_h */
