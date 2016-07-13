
//
//  sql.c
//  MySqlLite
//
//  Created by Thomas Fouan on 22/06/2016.
//  Copyright © 2016 Thomas Fouan. All rights reserved.
//

#include "sql.h"

#define BASE_NO_SQL "./BaseNoSql"

/*
 * Transform nom:'toto' or age:18 in hashmapEntry
 */
t_hashmap_entry *get_hashmap_entry_from_JSON(char *string) {
    
    t_hashmap_entry *entry = NULL;
    
    if(strstr(string, ":")) {
        char *key;
        key = strsep(&string, ":");
        
        if(string[0] == '\'' || string[0] == '\"') {
            string++;
            string[strlen(string)-1] = '\0';
            char *value;
            value = strdup(string);
            entry = hashmap_create_entry(key, value, TYPE_STRING);
        } else if(strchr(string, '.')) {
            double *value;
            value = malloc(sizeof(double));
            *value = strtod(string, NULL);
            entry = hashmap_create_entry(key, value, TYPE_DOUBLE);
        } else {
            int *value;
            value = malloc(sizeof(int));
            *value = atoi(string);
            entry = hashmap_create_entry(key, value, TYPE_INT);
        }
    } else {
        printf("Parse error...\n");
    }
    
    return entry;
}

/*
 * Transform a JSON string (e.g., {nom:'toto',prenom:'youyou',age:18}) in hashmap with a t_hashmapEntry for each key/value
 */
t_hashmap *JSON_parse(char *string) {
    
    t_hashmap *hashmap = NULL;
    t_hashmap_entry *entry = NULL;
    char *element;
    char *copy;
    
    if(string == NULL || strlen(string) < 3) {
        return NULL;
    }
    
    if(string[0] == '{' && string[strlen(string) - 1] == '}') {
        hashmap = hashmap_create(10, 2, 0.7);
        copy = strdup(&string[1]);
        copy[strlen(copy) - 1] = '\0';
        while((element = strsep(&copy, ","))) {
            if((entry = get_hashmap_entry_from_JSON(element)) == NULL) {
                hashmap_free(&hashmap);
                return NULL;
            }
            hashmap_put(hashmap, entry->key, entry->value, entry->type);
        }
    } else {
        printf("Parse error...\n");
    }
    
    return hashmap;
}

/*
 * Get a chained list of t_hashmap_entry from JSON string
 */
t_hashmap_entry *JSON_parse_list(char *string) {
    
    t_hashmap_entry *list = NULL;
    t_hashmap_entry *entry = NULL;
    char *element;
    char *copy;
    
    if(string == NULL || strlen(string) < 3) {
        return NULL;
    }
    
    if(string[0] == '{' && string[strlen(string) - 1] == '}') {
        copy = strdup(&string[1]);
        copy[strlen(copy) - 1] = '\0';
        while((element = strsep(&copy, ","))) {
            if((entry = get_hashmap_entry_from_JSON(element)) == NULL) {
                list_chain_free(list);
                return NULL;
            }
            list_chain_append(&list, entry);
        }
    } else {
        printf("Parse error...\n");
    }
    
    return list;
}

/*
 * Transform hashmapEntry to nom:'toto' or age:18
 */
char *get_string_from_hashmap_entry(t_hashmap_entry *entry) {
    
    char *string = NULL;
    unsigned long size;
    
    size = strlen(entry->key);
    
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
    
    t_hashmap_entry *temp;
    char *string = NULL;
    char *element = NULL;
    int i;
    
    for(i=0; i<hashmap->slots; i++) {
        if(hashmap->entries[i] != NULL) {
            temp = hashmap->entries[i];
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
    
    if(constraints == NULL) {
        return 0;
    }
    
    // For each constraint, compare its value with the value of entity field
    for(i=0; i<constraints->slots; i++) {
        temp = constraints->entries[i];
        while(temp != NULL) {
            if((entry = hashmap_get_entry(entity, temp->key)) == NULL) {
                printf("The '%s' key does not exist in the selected collection.\n", temp->key);
                return -1;
            }
            if(temp->type != entry->type) {
                printf("Wrong type of value for the '%s' key : %d != %d\n", temp->key, temp->type, entry->type);
                return -1;
            }
            if(is_equal(temp->value, entry->value, temp->type) != 0) {
                return -1;
            }
            temp = temp->next;
        }
    }
    
    return 0;
}

/*
 * Update entity's values with values that are in new_values
 */
void update_entity(t_hashmap *entity, t_hashmap *new_values) {
    
    int i;
    t_hashmap_entry *entry;
    t_hashmap_entry *temp;
    
    // For each member, update the value of this member in entity, if it exists
    for(i=0; i<new_values->slots; i++) {
        temp = new_values->entries[i];
        while(temp != NULL) {
            if((entry = hashmap_get_entry(entity, temp->key)) == NULL) {
                printf("The '%s' key does not exist in the selected collection.\n", temp->key);
            } else if(temp->type != entry->type) {
                printf("Wrong type of value for the '%s' key : %d != %d\n", temp->key, temp->type, entry->type);
            } else {
                hashmap_put(entity, temp->key, temp->value, temp->type);
            }
            temp = temp->next;
        }
    }
}

/**
 * Print the entry accordingly to the type of the value
 */
void print_type(t_hashmap_entry *entry) {
    
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
}

/*
 * Print the entity's values informed in projections
 */
void print_entity_projections(t_hashmap *entity, t_hashmap_entry *projections) {
    
    t_hashmap_entry *entry;
    int i;
    
    // If there isn't projections, print all members
    if(projections == NULL) {
        for(i=0; i<entity->slots; i++) {
            if(entity->entries[i] != NULL) {
                entry = entity->entries[i];
                while(entry != NULL) {
                    print_type(entry);
                    entry = entry->next;
                }
            }
        }
    }
    
    // For each projection, print the value of entity fields
    while(projections != NULL) {
        if((entry = hashmap_get_entry(entity, projections->key)) == NULL) {
            return;
        }
        print_type(entry);
        projections = projections->next;
    }
    
    printf("\n");
}

/*
 * Search a sql entity in a collection, respecting some constraints
 */
void sql_find(command_line *input) {
    
    t_hashmap *entity = NULL;
    t_hashmap *constraints = NULL;
    t_hashmap_entry *projections = NULL;
    FILE *file = NULL;
    char *path = NULL;
    char *pos = NULL;
    char *JSON_string = NULL;
    char *content = NULL;
    long length = 0;
    long file_length = 0;
    
    path = malloc(sizeof(char)*(strlen(input->collection)+strlen(BASE_NO_SQL)+6));
    sprintf(path, "%s/%s.txt", BASE_NO_SQL, input->collection);
    
    file = fopen(path, "rb");
    if(file != NULL) {
        // Get the length of the file and read it
        fseek(file, 0, SEEK_END);
        file_length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        content = malloc(sizeof(char)*(file_length));
        if(fread(content, sizeof(char), file_length, file) == file_length) {
            constraints = JSON_parse(input->action_value);
            // Create a chain list of t_hashmap_entry instead of t_hashmap to keep the order of the projections for printing
            projections = JSON_parse_list(input->projection_value);
            
            // Get each entity in the file
            while(*content != '\0' && (pos = strchr(content, '}')) != NULL && *(pos-1) != '\\') {
                pos += 1;
                length = pos-content;
                
                // Get the JSON string of the current object
                JSON_string = malloc(sizeof(char) * (length+1));
                strncpy(JSON_string, content, length);
                JSON_string[length] = '\0';
                
                // Parse the JSON string to get a hashmap of it
                entity = JSON_parse(JSON_string);
                
                // Print the entity if it respects the constraints of the -find command
                if(is_matching(entity, constraints) == 0) {
                    print_entity_projections(entity, projections);
                }
                
                free(JSON_string);
                content = pos;
            }
            content -= file_length;
            
            if(entity != NULL) {
                hashmap_free(&entity);
            }
            if(constraints != NULL) {
                hashmap_free(&constraints);
            }
            if(projections != NULL) {
                list_chain_free(projections);
            }
        } else {
            printf("An error occurred while reading the file\n");
        }
        
        free(content);
    } else {
        printf("The '%s' collection does not exist.\n", input->collection);
        printf("The '%s' path have not could be resolved\n", path);
    }
    
    free(path);
    fclose(file);
}

/*
 * Insert a sql entity in an existing collection or create one for this entity
 */
void sql_insert(command_line *input) {
    
    t_hashmap *entity = NULL;
    FILE *file = NULL;
    long length;
    char *path = NULL;
    
    path = malloc(sizeof(char)*(strlen(input->collection)+strlen(BASE_NO_SQL)+6));
    sprintf(path, "%s/%s.txt", BASE_NO_SQL, input->collection);
    
    // Try to open the file in 'r' mode. If file is NULL, create the file with the 'w' mode
    file = fopen(path, "r+b");
    if(file == NULL) {
        file = fopen(path, "wb");
    }
    fseek(file, 0, SEEK_END);
    
    entity = JSON_parse(input->action_value);
    if(entity != NULL) {
        length = strlen(input->action_value);
        if(fwrite(input->action_value, sizeof(char), length, file) != length) {
            printf("An error occurred during the writing of the data.\n");
        }
        
        free(entity);
    } else {
        printf("Syntax error in the data to insert.\n");
    }
    
    free(path);
    fclose(file);
}

/*
 * Update some values from existing sql entities, respecting some constraints
 */
void sql_set(command_line *input) {
    
    t_hashmap *entity = NULL;
    t_hashmap *values = NULL;
    t_hashmap *constraints = NULL;
    FILE *file = NULL;
    char *path = NULL;
    char *pos = NULL;
    char *JSON_string = NULL;
    char *content = NULL;
    long length = 0;
    long file_length = 0;
    long cur_position = 0;
    int nb_entity_updated = 0;
    
    if((constraints = JSON_parse(input->where_value)) == NULL) {
        printf("The WHERE clause is missing or incorrect\n");
        return;
    }
    
    path = malloc(sizeof(char)*(strlen(input->collection)+strlen(BASE_NO_SQL)+6));
    sprintf(path, "%s/%s.txt", BASE_NO_SQL, input->collection);
    
    file = fopen(path, "rb");
    if(file != NULL) {
        // Get the length of the file and read it
        fseek(file, 0, SEEK_END);
        file_length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        content = malloc(sizeof(char)*(file_length+1));
        
        if(fread(content, sizeof(char), file_length, file) == file_length) {
            values = JSON_parse(input->action_value);
            
            // Get each entity of the file
            while(content[cur_position] != '\0' && (pos = strchr(&content[cur_position], '}')) != NULL) {
                pos++;
                if(*(pos-2) == '\\') {
                    continue;
                }
                
                length = pos - &content[cur_position];
                
                // Get the JSON string of the current object
                JSON_string = malloc(sizeof(char) * (length+1));
                strncpy(JSON_string, &content[cur_position], length);
                JSON_string[length] = '\0';
                
                // Parse the JSON string to get a hashmap of it
                entity = JSON_parse(JSON_string);
                
                // Update the entity's values in the file if it respects the constraints of the -where clause
                if(is_matching(entity, constraints) == 0) {
                    update_entity(entity, values);
                    char *str = JSON_stringify(entity);
                    
                    content = replace_substring(content, cur_position, length, str);
                    cur_position += strlen(str);
                    nb_entity_updated++;
                    free(str);
                } else {
                    cur_position += length;
                }
                
                free(JSON_string);
            }
            
            fclose(file);
            
            // Open the file in wb mode to replace the content of the file by the content string
            fopen(path, "wb");
            fwrite(content, sizeof(char), strlen(content), file);
            
            printf("%d objects has been deleted successfully !\n", nb_entity_updated);
            
            if(entity != NULL) {
                hashmap_free(&entity);
            }
            if(constraints != NULL) {
                hashmap_free(&constraints);
            }
        } else {
            printf("An error occurred while reading the file\n");
        }
        
        free(content);
    } else {
        printf("The '%s' collection does not exist.\n", input->collection);
        printf("The '%s' path have not could be resolved\n", path);
    }
    
    free(path);
    fclose(file);
}

/*
 * Remove existing sql entities respecting some constraints
 */
void sql_remove(command_line *input) {
    
    t_hashmap *entity = NULL;
    t_hashmap *constraints = NULL;
    FILE *file = NULL;
    char *path = NULL;
    char *pos = NULL;
    char *JSON_string = NULL;
    char *content = NULL;
    long length = 0;
    long file_length = 0;
    long cur_position = 0;
    int nb_entity_deleted = 0;
    
    path = malloc(sizeof(char)*(strlen(input->collection)+strlen(BASE_NO_SQL)+6));
    sprintf(path, "%s/%s.txt", BASE_NO_SQL, input->collection);
    
    file = fopen(path, "rb");
    if(file != NULL) {
        // Get the length of the file and read it
        fseek(file, 0, SEEK_END);
        file_length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        content = malloc(sizeof(char)*(file_length+1));
        
        if(fread(content, sizeof(char), file_length, file) == file_length) {
            constraints = JSON_parse(input->action_value);
            
            // Get each entity of the file
            while(content[cur_position] != '\0' && (pos = strchr(&content[cur_position], '}')) != NULL && *(pos-1) != '\\') {
                pos += 1;
                length = pos - &content[cur_position];
                
                // Get the JSON string of the current object
                JSON_string = malloc(sizeof(char) * (length+1));
                strncpy(JSON_string, &content[cur_position], length);
                JSON_string[length] = '\0';
                
                // Parse the JSON string to get a hashmap of it
                entity = JSON_parse(JSON_string);
                
                // Delete the entity in the file if it respects the constraints of the -remove command
                if(is_matching(entity, constraints) == 0) {
                    content = revert_substr(content, cur_position, length);
                    nb_entity_deleted++;
                } else {
                    cur_position += length;
                }
                
                free(JSON_string);
            }
            
            fclose(file);
            
            // Open the file in wb mode to replace the content of the file by the content string
            fopen(path, "wb");
            fwrite(content, sizeof(char), strlen(content), file);
            
            printf("%d objects has been deleted successfully !\n", nb_entity_deleted);
            
            if(entity != NULL) {
                hashmap_free(&entity);
            }
            if(constraints != NULL) {
                hashmap_free(&constraints);
            }
        } else {
            printf("An error occurred while reading the file\n");
        }
        
        free(content);
    } else {
        printf("The '%s' collection does not exist.\n", input->collection);
        printf("The '%s' path have not could be resolved\n", path);
    }
    
    free(path);
    fclose(file);
}

/*
 * Return the content string without the characters between start_pos and start_pos + length
 */
char *revert_substr(char *content, long start_pos, long length) {
    
    char *new_string = content;
    long old_length = 0;
    long count = 0;
    int i;
    
    if(content && start_pos >= 0 && length >= 0 && start_pos+length <= (old_length = strlen(content))) {
        long new_length = old_length - length;
        new_string = malloc(sizeof(char)*(new_length+1));
        
        // Copy all characters that are not in the range between start_pos and start_pos + length
        for(i=0; i<old_length; i++) {
            if(i >= start_pos && i < start_pos + length) {
                continue;
            }
            new_string[count++] = content[i];
        }
        
        new_string[new_length] = '\0';
        
        free(content);
        content = new_string;
    }
    
    return new_string;
}

/*
 * Return the content string replacing the characters between pos and pos + length by str
 */
char *replace_substring(char *content, long pos, long length, char *str) {
    
    long content_length = strlen(content);
    long str_length = strlen(str);
    char *new_string = content;
    int count = 0;
    long i;
    
    if(str && pos >= 0 && length >= 0 && pos + length <= content_length) {
        new_string = malloc(sizeof(char) * (content_length - length + str_length + 1));
        for(i=0; i<pos; i++, count++) {
            new_string[count] = content[i];
        }
        for(i=0; i<str_length; i++, count++) {
            new_string[count] = str[i];
        }
        for(i=pos+length; i<content_length; i++, count++) {
            new_string[count] = content[i];
        }
        
        new_string[count] = '\0';
        free(content);
        content = new_string;
    }
    
    return new_string;
}

/*
 * Append an element to the chained list pointed to list
 */
void list_chain_append(t_hashmap_entry **list, t_hashmap_entry *entry) {
    
    while(*list != NULL) {
        list = &(*list)->next;
    }
    
    *list = entry;
}

/*
 * Free the memory allocated by the chained list
 */
void list_chain_free(t_hashmap_entry *list) {
    
    t_hashmap_entry *to_delete;
    
    while(list != NULL) {
        to_delete = list;
        list = list->next;
        free(to_delete);
    }
}
