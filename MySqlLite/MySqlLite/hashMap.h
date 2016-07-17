#ifndef hashMap_h
#define hashMap_h

typedef struct t_hashmap {

    t_hashmap_entry **entries;
    int slots;
    int size;
    float grow_factor;
    float load_factor;
    
} t_hashmap;

t_hashmap *hashmap_create(int, float, float);

void hashmap_resize(t_hashmap*);

void hashmap_put(t_hashmap *, char *, void *, type_var);

void *hashmap_get(t_hashmap *, char *);

t_hashmap_entry *hashmap_get_entry(t_hashmap *, char *);

void hashmap_remove(t_hashmap *, char *);

void hashmap_free(t_hashmap **);

void hashmap_print(t_hashmap *);

int compare_hashmap(t_hashmap *, t_hashmap *, t_hashmap_entry *);

#endif /* hashMap_h */
