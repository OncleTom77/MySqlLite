#ifndef hashmap_entry_h
#define hashmap_entry_h

typedef enum type_var {
    TYPE_STRING,
    TYPE_INT,
    TYPE_DOUBLE
} type_var;

typedef struct t_hashmap_entry {
    
    char *key;
    void *value;
    type_var type;
    struct t_hashmap_entry *next;

} t_hashmap_entry;

t_hashmap_entry *hashmap_entry_create(char *, void *, type_var);

void hashmap_entry_append(t_hashmap_entry **, t_hashmap_entry *);

void hashmap_entry_free(t_hashmap_entry *);

void print_entry(t_hashmap_entry *);

char *get_string_from_hashmap_entry(t_hashmap_entry *);

void print_type(t_hashmap_entry *);

int compare_hashmap_entry(t_hashmap_entry *, t_hashmap_entry *);

#endif /* hashmap_entry_h */