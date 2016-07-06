//
//  hashMap.h
//  cMap
//
//  Created by Thomas Fouan on 13/05/2016.
//  Copyright © 2016 Thomas Fouan. All rights reserved.
//

#ifndef hashMap_h
#define hashMap_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct t_hashmap {
    
    t_hashmap_entry **entries;
    int slots;
    int size;
    float grow_factor;
    float load_factor;
    
} t_hashmap;

int get_hash_code(char *);
t_hashmap *hashmap_create(int, float, float);
t_hashmap_entry *hashmap_create_entry(char *, void *, type_var);
void hashmap_resize(t_hashmap*);
void hashmap_put(t_hashmap *, char *, void *, type_var);
t_hashmap_entry *hashmap_get_entry(t_hashmap *, char *);
void *hashmap_get(t_hashmap *, char *);
void hashmap_remove(t_hashmap *, char *);
void hashmap_free(t_hashmap **);

void print_entry(t_hashmap_entry *);
void hashmap_print(t_hashmap *);

#endif /* hashMap_h */
