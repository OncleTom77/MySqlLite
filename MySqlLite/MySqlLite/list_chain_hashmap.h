#ifndef list_chain_hashmap_h
#define list_chain_hashmap_h

#include "includes.h"

typedef struct list_chain_hashmap {
    
    t_hashmap *hashmap;
    struct list_chain_hashmap *next;
    
} list_chain_hashmap;

void list_chain_hashmap_insert(list_chain_hashmap **, t_hashmap *, t_hashmap_entry *);

void list_chain_hashmap_print(list_chain_hashmap *);

void list_chain_hashmap_print_projections(list_chain_hashmap *, t_hashmap_entry *);

void list_chain_hashmap_free(list_chain_hashmap **);

#endif /* list_chain_hashmap_h */
