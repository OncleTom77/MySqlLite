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
    TYPE_DOUBLE,
} type_var;

typedef struct t_hashmapEntry {
    
    char* key;
    void* value;
    type_var type;
    struct t_hashmapEntry* next;
    
} t_hashmapEntry;

typedef struct t_hashmap {
    
    t_hashmapEntry** entries;
    int slots;
    int size;
    float grow_factor;
    float load_factor;
    
} t_hashmap;

int getHashCode(char*);
t_hashmap* hashmap_create(int, float, float);
t_hashmapEntry* hashmap_create_entry(char*, void*, type_var);
void hashmap_resize(t_hashmap*);
void hashmap_put(t_hashmap*, char*, void*, type_var);
void* hashmap_get(t_hashmap*, char*);
void hashmap_remove(t_hashmap*, char*);

void hashmap_print(t_hashmap*);
void print_entry(t_hashmapEntry*);

#endif /* hashMap_h */
