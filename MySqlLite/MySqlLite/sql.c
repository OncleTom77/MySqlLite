
//
//  sql.c
//  MySqlLite
//
//  Created by Thomas Fouan on 22/06/2016.
//  Copyright © 2016 Thomas Fouan. All rights reserved.
//

#include "sql.h"

/*
 * Transform nom:'toto' or age:18 in hashmapEntry
 */
t_hashmap_entry *get_hashmap_entry_from_JSON(char *string) {
    
    t_hashmap_entry *entry = NULL;
    
    if(strstr(string, ":")) {
        char *key = strsep(&string, ":");
        if(string[0] == '\'') {
            string++;
            string[strlen(string)-1] = '\0';
            char *value = strdup(string);
            entry = hashmap_create_entry(key, value, TYPE_STRING);
        } else if(strchr(string, '.')) {
            double *value = malloc(sizeof(double));
            *value = strtod(string, NULL);
            entry = hashmap_create_entry(key, value, TYPE_DOUBLE);
        } else {
            int *value = malloc(sizeof(int));
            *value = atoi(string);
            entry = hashmap_create_entry(key, value, TYPE_INT);
        }
    } else {
        printf("Parse error...");
    }
    
    return entry;
}

/*
 * Transform {nom:'toto',prenom:'youyou',age:18} in hashmap with t_hashmapEntry for each key/value
 */
t_hashmap *JSON_parse(char *string) {
    
    t_hashmap *hashmap = NULL;
    t_hashmap_entry *entry = NULL;
    
    if(string[0] == '{' && string[strlen(string) - 1] == '}') {
        hashmap = hashmap_create(10, 2, 0.7);
        char *element;
        char *copy = strdup(&string[1]);
        copy[strlen(copy) - 1] = '\0';
        while((element = strsep(&copy, ","))) {
            entry = get_hashmap_entry_from_JSON(element);
            hashmap_put(hashmap, entry->key, entry->value, entry->type);
        }
    } else {
        printf("Parse error...");
    }
    
    return hashmap;
}

/*
 * Transform hashmapEntry to nom:'toto' or age:18
 */
char *get_string_from_hashmap_entry(t_hashmap_entry *entry) {
    
    char *string = NULL;
    unsigned long size = strlen(entry->key);
    switch(entry->type) {
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

char *JSON_stringify(t_hashmap *hashmap) {
    
    char *string = NULL;
    char *element = NULL;
    int i;
    for(i=0; i<hashmap->slots; i++) {
        if(hashmap->entries[i] != NULL) {
            t_hashmap_entry *temp = hashmap->entries[i];
            while(temp != NULL) {
                element = get_string_from_hashmap_entry(temp);
                
                if(!string) {
                    string = realloc(string, sizeof(char)*(strlen(element)+2));
                    string[0] = '{';
                } else {
                    string = realloc(string, sizeof(char)*(strlen(string)+strlen(element)+1));
                    strcat(string, ",");
                }
                strcat(string, element);
                temp = temp->next;
            }
        }
    }
    strcat(string, "}");
    
    return string;
}
