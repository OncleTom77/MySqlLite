
//
//  sql.c
//  MySqlLite
//
//  Created by Thomas Fouan on 22/06/2016.
//  Copyright © 2016 Thomas Fouan. All rights reserved.
//

#include "sql.h"

#define BASE_NO_SQL = "./BaseNoSql";

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
 * Transform a JSON string (e.g., {nom:'toto',prenom:'youyou',age:18}) in hashmap with a t_hashmapEntry for each key/value
 */
t_hashmap *JSON_parse(char *string) {
    
    t_hashmap *hashmap = NULL;
    t_hashmap_entry *entry = NULL;
    
    if(string == NULL || strlen(string) < 3) {
        return NULL;
    }
    
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

/*
 * Transform hashmap in a JSON string (e.g, {nom:'toto',prenom:'youyou',age:18})
 */
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

/*
 * Return 0 if value1 and value2 are equals. Else, return -1
 */
int is_equal(void *value1, void *value2, type_var type) {
    
    if(type == TYPE_STRING && strcmp((char *)value1, (char *)value2) == 0) {
        return 0;
    } else if(type == TYPE_INT && *((int *) value1) == *((int *) value2)) {
        return 0;
    } else if(type == TYPE_DOUBLE && *((double *) value1) == *((double *) value2)) {
        return 0;
    }
    
    return -1;
}

/*
 * Return 0 if the entity's values match with the constraints
 */
int is_matching(t_hashmap *entity, t_hashmap *constraints) {
    
    int i;
    t_hashmap_entry *entry;
    t_hashmap_entry *temp;
    
    // For each constraint, compare its value with the value of entity field
    for(i=0; i<constraints->slots; i++) {
        if(constraints->entries[i] != NULL) {
            temp = constraints->entries[i];
            while(temp != NULL) {
                if((entry = hashmap_get_entry(entity, temp->key)) == NULL) {
                    //printf("The '%s' key does not exist in the selected collection.\n", temp->key);
                    return -1;
                }
                if(temp->type != entry->type) {
                    //printf("Wrong type of value for the '%s' key.\n", temp->key);
                    return -1;
                }
                if(!is_equal(temp->value, entry->value, temp->type)) {
                    return -1;
                }
                temp = temp->next;
            }
        }
    }
    
    return 0;
}

/*
 * Print the entity's values informed in projections
 */
void print_entity_projections(t_hashmap *entity, t_hashmap *projections) {
    
    int i;
    t_hashmap_entry *entry;
    t_hashmap_entry *temp;
    
    // For each projection, print the value of entity fields
    for(i=0; i<projections->slots; i++) {
        if(projections->entries[i] != NULL) {
            temp = projections->entries[i];
            while(temp != NULL) {
                if((entry = hashmap_get_entry(entity, temp->key)) == NULL) {
                    return;
                }
                switch(entry->type) {
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
                
                temp = temp->next;
            }
        }
    }
}

/*
 * Search a sql entity in a collection, respecting some constraints
 */
int sql_find(command_line *input) {
    
    FILE *file = fopen(input->collection, "r");
    t_hashmap *entity;
    t_hashmap *constraints;
    t_hashmap *projections;
    
    if(file == NULL) {
        printf("The '%s' collection does not exist.\n", input->collection);
        return -1;
    }
    
    printf("Action : '%s'\n", input->action_value);
    
    entity = malloc(sizeof(t_hashmap));
    constraints = JSON_parse(input->action_value);
    projections = JSON_parse(input->projection_value);
    
    // Get each entity in the file
    while(fread(entity, sizeof(t_hashmap), 1, file) == 1) {
        hashmap_print(entity);
        // Print the entity if it respects the constraints of the -find command
        if(is_matching(entity, constraints)) {
            if(projections != NULL) {
                print_entity_projections(entity, projections);
            } else {
                hashmap_print(entity);
            }
            printf("\n");
        }
        
    }
    
    free(entity);
    if(constraints != NULL) {
        hashmap_free(&constraints);
    }
    if(projections != NULL) {
        hashmap_free(&projections);
    }
    
    fclose(file);
    return 0;
}

/*
 * Insert a sql entity in an existing collection or create one for this entity
 */
int sql_insert(command_line *input) {
    
    return 0;
}

/*
 * Update some values from existing sql entities, respecting some constraints
 */
int sql_set(command_line *input) {
    
    return 0;
}

/*
 * Remove existing sql entities respecting some constraints
 */
int sql_remove(command_line *input) {
    
    return 0;
}

