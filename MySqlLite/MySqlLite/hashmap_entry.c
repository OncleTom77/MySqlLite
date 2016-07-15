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
            printf("%.2lf\t", *((double *) entry->value));
            break;
        default:
            printf("unknown\t");
    }
}

/*
 * Compare 2 values accordingly to their type.
 * Return : a negatif number if value1 < value2
 *          a positif number if value1 > value2
 *          0 if value1 == value2
 */
int compare_hashmap_entry(t_hashmap_entry *entry1, t_hashmap_entry *entry2) {
    
    if (entry1 && entry2) {
        if(entry1->type == entry2->type) {
            switch (entry1->type) {
                case TYPE_STRING:
                    return strcmp((char *)entry1->value, (char *)entry2->value);
                case TYPE_DOUBLE: {
                    double val1 = *((double *)entry1->value);
                    double val2 = *((double *)entry2->value);
                    if (val1 < val2)
                        return -1;
                    else if(val1 > val2)
                        return 1;
                    break;
                }
                case TYPE_INT: {
                    int val1 = *((int *)entry1->value);
                    int val2 = *((int *)entry2->value);
                    if (val1 < val2)
                        return -1;
                    else if(val1 > val2)
                        return 1;
                }
            }
        }
    } else if (entry1)
        return 1;
    else if(entry2)
        return -1;
    
    return 0;
}
