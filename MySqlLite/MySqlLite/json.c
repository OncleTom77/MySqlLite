#include "includes.h"

/*
 * Transform nom:'toto' or age:18 in hashmap_entry
 */
t_hashmap_entry *get_hashmap_entry_from_JSON(char *string) {
    
    t_hashmap_entry *entry = NULL;
    
    if (strstr(string, ":")) {
        char *key;
        key = strsep(&string, ":");
        
        if (string[0] == '\'' || string[0] == '\"') {
            char *value;

            string++;
            string[strlen(string)-1] = '\0';
            
            value = strdup(string);
            entry = hashmap_entry_create(key, value, TYPE_STRING);
        } else if (strchr(string, '.')) {
            double *value;

            value   = malloc(sizeof(double));
            *value  = strtod(string, NULL);
            entry   = hashmap_entry_create(key, value, TYPE_DOUBLE);
        } else {
            int *value;

            value   = malloc(sizeof(int));
            *value  = atoi(string);
            entry   = hashmap_entry_create(key, value, TYPE_INT);
        }
    } else
        printf("E3 - Parse error...\n");
    
    return entry;
}

/*
 * Transform a JSON string (e.g., {nom:'toto',prenom:'youyou',age:18}) in hashmap with a t_hashmap_entry for each key/value
 */
t_hashmap *JSON_parse(char *string) {
    
    t_hashmap *hashmap      = NULL;
    t_hashmap_entry *entry  = NULL;
    char *element;
    char *copy;
    
    if (string == NULL || strlen(string) < 2)
        return NULL;
    
    if (string[0] == '{' && string[strlen(string) - 1] == '}') {
        hashmap = hashmap_create(10, 2, 0.7);
        copy    = strdup(&string[1]);
        copy[strlen(copy) - 1] = '\0';

        while ((element = strsep(&copy, ",")) != NULL && strlen(element) > 0) {
            if ((entry = get_hashmap_entry_from_JSON(element)) == NULL) {
                hashmap_free(&hashmap);
                return NULL;
            }

            hashmap_put(hashmap, entry->key, entry->value, entry->type);
        }
    } else
        printf("E1 - Parse error...\n");
    
    return hashmap;
}

/*
 * Get a chained list of t_hashmap_entry from JSON string
 */
t_hashmap_entry *JSON_parse_list(char *string) {
    
    t_hashmap_entry *list   = NULL;
    t_hashmap_entry *entry  = NULL;
    char *element;
    char *copy;
    
    if (string == NULL || strlen(string) < 2)
        return NULL;
    
    if (string[0] == '{' && string[strlen(string) - 1] == '}') {
        copy = strdup(&string[1]);
        copy[strlen(copy) - 1] = '\0';

        while ((element = strsep(&copy, ",")) && strlen(element) > 0) {
            if ((entry = get_hashmap_entry_from_JSON(element)) == NULL) {
                hashmap_entry_free(list);
                return NULL;
            }

            hashmap_entry_append(&list, entry);
        }
    }
    else
        printf("E2 - Parse error...\n");
    
    return list;
}

/*
 * Transform hashmap in a JSON string (e.g, {nom:'toto',prenom:'youyou',age:18})
 */
char *JSON_stringify(t_hashmap *hashmap) {
    
    t_hashmap_entry *temp;
    char *string = NULL;
    char *element = NULL;
    int i;
    
    for (i = 0; i < hashmap->slots; i++) {
        if (hashmap->entries[i] != NULL) {
            temp = hashmap->entries[i];
            
            while (temp != NULL) {
                element = get_string_from_hashmap_entry(temp);
                
                if (!string) {
                    string = realloc(string, sizeof(char) * (strlen(element) + 2));
                    string[0] = '{';
                } else {
                    string = realloc(string, sizeof(char) * (strlen(string) + strlen(element) + 1));
                    strcat(string, ",");
                }
                strcat(string, element);
                temp = temp->next;
                free(element);
            }
        }
    }
    strcat(string, "}");
    
    return string;
}