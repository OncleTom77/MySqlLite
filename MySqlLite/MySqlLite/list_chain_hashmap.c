#include "includes.h"

/*
 * Insert a hashmap in the list_chain_hashmap, accordingly to the sort_value
 */
void list_chain_hashmap_insert(list_chain_hashmap **list, t_hashmap *hashmap, t_hashmap_entry *sort) {
    
    list_chain_hashmap *element = NULL;
    int sort_value;
    
    if (!hashmap || !sort)
        return;
    
    element = malloc(sizeof(list_chain_hashmap));
    element->hashmap = hashmap;
    element->next = NULL;
    
    sort_value = *((int *)sort->value);
    while (*list != NULL) {
        if (compare_hashmap((*list)->hashmap, hashmap, sort) > 0)
            break;
        
        list = &((*list)->next);
    }
    
    element->next = *list;
    *list = element;
}

/*
 * Print the list_chain_hashmap (debug)
 */
void list_chain_hashmap_print(list_chain_hashmap *list) {
    
    while (list) {
        hashmap_print(list->hashmap);
        list = list->next;
    }
}

/*
 * Print the list_chain_hashmap's values informed in projections
 */
void list_chain_hashmap_print_projections(list_chain_hashmap *list, t_hashmap_entry *projections) {
    
    while(list) {
        print_entity_projections(list->hashmap, projections);
        list = list->next;
    }
}

/*
 * Free memory allocated for the list_chain_hashmap
 */
void list_chain_hashmap_free(list_chain_hashmap **list) {
    
    list_chain_hashmap *to_delete;
    
    while (*list) {
        to_delete = *list;
        list = &(*list)->next;
        hashmap_free(&(to_delete->hashmap));
        free(to_delete);
    }
}
