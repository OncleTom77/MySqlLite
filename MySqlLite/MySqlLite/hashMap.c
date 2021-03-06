#include "includes.h"

/* 
 * Init the hashmap
 */
t_hashmap *hashmap_create(int slots, float grow_factor, float load_factor) {
    
    int i;
    t_hashmap *hashmap;

    hashmap          = malloc(sizeof(t_hashmap)); 
    hashmap->entries = malloc(sizeof(t_hashmap_entry *) * slots);
    
    for (i = 0; i < slots; i++)
        hashmap->entries[i] = NULL;

    hashmap->slots       = slots;
    hashmap->size        = 0;
    hashmap->grow_factor = grow_factor;
    hashmap->load_factor = load_factor;
    
    return hashmap;
}

/*
 * Resize the hashmap
 */
void hashmap_resize(t_hashmap *hashmap) {
    
    int i;
    int last_nb_slots;
    t_hashmap_entry **last_entries;
    t_hashmap_entry *entry;
    t_hashmap_entry *to_delete;
    
    last_nb_slots       = hashmap->slots;
    last_entries        = hashmap->entries;
    
    hashmap->size       = 0;
    hashmap->slots      = last_nb_slots * hashmap->grow_factor;
    hashmap->entries    = malloc(sizeof(t_hashmap_entry *) * hashmap->slots);

    for (i = 0; i < hashmap->slots; i++)
        hashmap->entries[i] = NULL;
    
    for (i = 0; i < last_nb_slots; i++) {
        entry = last_entries[i];
        while (entry) {
            hashmap_put(hashmap, entry->key, entry->value, entry->type);
            
            to_delete = entry;
            entry = entry->next;
            free(to_delete);
        }
    }
    
    free(last_entries);
}

/*
 * Put a pair key/value in the hashmap
 */
void hashmap_put(t_hashmap *hashmap, char *key, void *value, type_var type) {
    
    if (hashmap) {
        int num_slot;
        t_hashmap_entry **entry;
        
        num_slot    = get_hash_code(key) % hashmap->slots;
        entry       = &(hashmap->entries[num_slot]);
        
        // If the key already exists, update its value
        if (hashmap_get(hashmap, key)) {
            
            while (*entry) {
                if (strcmp((*entry)->key, key) == 0) {
                    (*entry)->value = value;
                    return;
                }
                entry = &((*entry)->next);
            }
        } else {
            // If the size of the map is up to the number of slots * load_factor, resize the hashmap
            if (hashmap->size + 1 >= hashmap->slots * hashmap->load_factor) {
                hashmap_resize(hashmap);
                num_slot = get_hash_code(key) % hashmap->slots;
                entry = &(hashmap->entries[num_slot]);
            }
            
            while (*entry)
                entry = &((*entry)->next);
            
            *entry = hashmap_entry_create(key, value, type);
            hashmap->size++;
        }
    }
}

/*
 * Get a value by its key
 */
void *hashmap_get(t_hashmap *hashmap, char *key) {
    
    if (hashmap) {
        int num_slot;
        t_hashmap_entry **entry;
        
        num_slot    = get_hash_code(key) % hashmap->slots;
        entry       = &(hashmap->entries[num_slot]);
        
        while (*entry) {
            // Return the matching key
            if(strcmp((*entry)->key, key) == 0)
                return (*entry)->value;

            entry = &((*entry)->next);
        }
    }
    
    return NULL;
}

/*
 * Get a hashmap_entity by its key
 */
t_hashmap_entry *hashmap_get_entry(t_hashmap *hashmap, char *key) {
    
    if (hashmap) {
        int num_slot;
        t_hashmap_entry **entry;
        
        num_slot    = get_hash_code(key) % hashmap->slots;
        entry       = &(hashmap->entries[num_slot]);
        
        while (*entry) {
            // Return the matching entry
            if(strcmp((*entry)->key, key) == 0) {
                return *entry;
            }
            entry = &((*entry)->next);
        }
    }

    return NULL;
}

/*
 * Remove an element with its key
 */
void hashmap_remove(t_hashmap *hashmap, char *key) {
    
    if (hashmap) {
        int num_slot;
        t_hashmap_entry **entry;
        t_hashmap_entry *to_delete;
        
        num_slot    = get_hash_code(key) % hashmap->slots;
        entry       = &(hashmap->entries[num_slot]);
        
        while (*entry) {
            // Remove the current element if it is the good one
            if (strcmp((*entry)->key, key) == 0) {
                to_delete = *entry;
                *entry = (*entry)->next;
                
                free(to_delete);
                hashmap->size--;
                
                return;
            }

            entry = &((*entry)->next);
        }
    }
}

/*
 * Print the hashmap (debug)
 */
void hashmap_print(t_hashmap *hashmap) {
    
    int i;
    t_hashmap_entry *temp = NULL;
    
    printf("--------------- HASHMAP CONTENT ---------------\n");
    printf("Size : %d\n", hashmap->size);
    for (i = 0; i < hashmap->slots; i++) {
        printf("Slot %d : ", i);
        
        if (hashmap->entries[i] == NULL) {
            printf("NULL");
        } else {
            temp = hashmap->entries[i];
            
            while (temp != NULL) {
                print_entry(temp);
                temp = temp->next;
            }
        }
        printf("\n");
    }
    
    printf("-----------------------------------------------\n");
}

/*
 * Compare 2 hashmap by one or more key(s) defined in sort list_chain.
 * Return : -1 if the value of the key of the hashmap1 < hashmap2. Revert the result if the sort value is desc
 *          +1 if the value of the key of the hashmap1 > hashmap2. Revert the result if the sort value is desc
 *          0 if the value of the key of the hashmap1 == hashmap2
 */
int compare_hashmap(t_hashmap *hashmap1, t_hashmap *hashmap2, t_hashmap_entry *sort) {
    
    t_hashmap_entry *val1;
    t_hashmap_entry *val2;
    int result = 0;
    
    while (sort != NULL) {
        val1 = hashmap_get_entry(hashmap1, sort->key);
        val2 = hashmap_get_entry(hashmap2, sort->key);
        
        if ((result = compare_hashmap_entry(val1, val2)) != 0)
            return (*((int *)sort->value) < 0) ? -result : result;
        
        sort = sort->next;
    }
    
    return 0;
}

/*
 * Free memory allocated for the hashmap
 */
void hashmap_free(t_hashmap **hashmap) {
    
    int i;
    t_hashmap_entry *to_delete = NULL;
    t_hashmap_entry *temp = NULL;
    
    if (*hashmap == NULL)
        return;
    
    for (i = 0; i < (*hashmap)->slots; i++) {
        if ((*hashmap)->entries[i] != NULL) {
            temp = (*hashmap)->entries[i];
            
            while (temp != NULL) {
                to_delete = temp;
                temp = temp->next;
                free(to_delete);
            }
        }
    }
    
    free((*hashmap)->entries);
    free(*hashmap);

    *hashmap    = NULL;
    hashmap     = NULL;
}
