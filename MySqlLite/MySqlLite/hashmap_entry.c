#include "includes.h"

/*
 * Return an entry to insert in the hashmap
 */
t_hashmap_entry *hashmap_entry_create(char *key, void *value, type_var type) {
    
    t_hashmap_entry *entry;
    entry = malloc(sizeof(t_hashmap_entry));
    
    entry->key      = key;
    entry->value    = value;
    entry->type     = type;
    entry->next     = NULL;
    
    return entry;
}

/*
 * Append an element to the chained list pointed to list
 */
void hashmap_entry_append(t_hashmap_entry **list, t_hashmap_entry *entry) {
    
    while (*list != NULL)
        list = &(*list)->next;
    
    *list = entry;
}

/*
 * Free the memory allocated by the chained list
 */
void hashmap_entry_free(t_hashmap_entry *list) {
    
    t_hashmap_entry *to_delete;
    
    while (list != NULL) {
        to_delete   = list;
        list        = list->next;

        free(to_delete);
    }
}

/**
 * Print a hashmap entry
 */
void print_entry(t_hashmap_entry *entry) {
    
    switch (entry->type) {
        case TYPE_STRING:
            printf("%s ==> %s, ", entry->key, (char *) entry->value);
            break;
        case TYPE_INT:
            printf("%s ==> %d, ", entry->key, *((int *) entry->value));
            break;
        case TYPE_DOUBLE:
            printf("%s ==> %lf, ", entry->key, *((double *) entry->value));
            break;
        default:
            printf("%s ==> unknown type, ", entry->key);
    }
}

/*
 * Transform hashmapEntry to nom:'toto' or age:18
 */
char *get_string_from_hashmap_entry(t_hashmap_entry *entry) {
    
    char *string = NULL;
    unsigned long size;
    
    size = strlen(entry->key);
    
    switch (entry->type) {
        case TYPE_STRING:
            size += strlen((char*)entry->value) + 2;
            string = malloc(sizeof(char)*size);
            sprintf(string, "%s:'%s'", entry->key, (char *)entry->value);
            break;
            
        case TYPE_DOUBLE:
            size += 30; // number of digits of the double value
            string = malloc(sizeof(char)*size);
            sprintf(string, "%s:%lf", entry->key, *((double *)entry->value));
            break;
            
        case TYPE_INT:
            size += 20; // number of digits of the int value
            string = malloc(sizeof(char)*size);
            sprintf(string, "%s:%d", entry->key, *((int *)entry->value));
            break;
    }
    
    return string;
}

/**
 * Print the entry accordingly to the type of the value
 */
void print_type(t_hashmap_entry *entry) {
    
    switch (entry->type) {
        case TYPE_STRING:
            printf("%s\t", (char *) entry->value);
            break;
        case TYPE_INT:
            printf("%d\t", *((int *) entry->value));
            break;
        case TYPE_DOUBLE:
            printf("%lf\t", *((double *) entry->value));
            break;
        default:
            printf("unknown\t");
    }
}