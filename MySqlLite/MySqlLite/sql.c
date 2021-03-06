#include "includes.h"

/*
 * Return 0 if the entity's values match with the constraints
 */
int is_matching(t_hashmap *entity, t_hashmap *constraints) {
    
    int i;
    t_hashmap_entry *entry;
    t_hashmap_entry *temp;
    
    if (entity == NULL)
        return -1;
    
    if (constraints == NULL)
        return 0;
    
    // For each constraint, compare its value with the value of entity field
    for (i = 0; i < constraints->slots; i++) {
        temp = constraints->entries[i];
        while (temp != NULL) {
            if ((entry = hashmap_get_entry(entity, temp->key)) == NULL) {
                //printf("The '%s' key does not exist in the selected collection.\n", temp->key);
                return -1;
            }

            if (temp->type != entry->type) {
                //printf("Wrong type of value for the '%s' key : %d != %d\n", temp->key, temp->type, entry->type);
                return -1;
            }

            if (is_equal(temp->value, entry->value, temp->type) != 0)
                return -1;

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
    
    if(entity == NULL || new_values == NULL)
        return;
    
    // For each member, update the value of this member in entity, if it exists
    for (i = 0; i < new_values->slots; i++) {
        temp = new_values->entries[i];
        
        while (temp != NULL) {
            
            if ((entry = hashmap_get_entry(entity, temp->key)) == NULL)
                printf("The '%s' key does not exist in the selected collection.\n", temp->key);
            else if (temp->type != entry->type)
                printf("Wrong type of value for the '%s' key : %d != %d\n", temp->key, temp->type, entry->type);
            else
                hashmap_put(entity, temp->key, temp->value, temp->type);
            
            temp = temp->next;
        }
    }
}

/*
 * Print the entity's values informed in projections
 */
void print_entity_projections(t_hashmap *entity, t_hashmap_entry *projections) {
    
    t_hashmap_entry *entry;
    int i;
    int is_print = -1;
    
    if(!entity)
        return;
    
    // If there isn't projections, print all members
    if (projections == NULL) {
        for (i = 0; i < entity->slots; i++) {
            if (entity->entries[i] != NULL) {
                entry = entity->entries[i];
                
                while (entry != NULL) {
                    is_print = 0;
                    print_type(entry);
                    entry = entry->next;
                }
            }
        }
    }
    
    // For each projection, print the value of entity fields
    while (projections != NULL) {
        if ((entry = hashmap_get_entry(entity, projections->key)) != NULL) {
            is_print = 0;
            print_type(entry);
        }
        
        projections = projections->next;
    }
    
    if(is_print == 0)
        printf("\n");
}

/*
 * Search a sql entity in a collection, respecting some constraints
 */
void sql_find(command_line *input) {
    
    t_hashmap *entity               = NULL;
    list_chain_hashmap *entities    = NULL;
    FILE *file                      = NULL;
    char *path                      = NULL;
    char *pos                       = NULL;
    char *JSON_string               = NULL;
    char *content                   = NULL;
    long length                     = 0;
    long file_length                = 0;
    
    path = malloc(sizeof(char) * (strlen(input->collection) + strlen(BASE_NO_SQL) + 6));

    sprintf(path, "%s/%s.txt", BASE_NO_SQL, input->collection);
    
    file = fopen(path, "rb");
    if (file != NULL) {
        // Get the length of the file and read it
        fseek(file, 0, SEEK_END);
        file_length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        content = malloc(sizeof(char) * file_length);

        if (fread(content, sizeof(char), file_length, file) == file_length) {
            // Get each entity in the file
            while (*content != '\0' && (pos = strchr(content, '}')) != NULL && *(pos-1) != '\\') {
                pos++;
                length = pos - content;
                
                // Get the JSON string of the current object
                JSON_string = malloc(sizeof(char) * (length + 1));
                strncpy(JSON_string, content, length);
                JSON_string[length] = '\0';
                
                // Parse the JSON string to get a hashmap of it
                entity = JSON_parse(JSON_string);
                
                // Print the entity if it respects the constraints of the -find command
                if (is_matching(entity, input->action_value) == 0) {
                    // If sort_value is not NULL, store each entity in list_chain sorted by the field of the sort_value
                    if (input->sort_value != NULL) {
                        list_chain_hashmap_insert(&entities, entity, input->sort_value);
                    } else {
                        print_entity_projections(entity, input->projection_value);
                        if (entity != NULL)
                            hashmap_free(&entity);
                    }
                }
                
                free(JSON_string);
                content = pos;
            }
            content -= file_length;
            
            // If sort_value is not NULL, print the entities sorted list
            if(input->sort_value != NULL) {
                list_chain_hashmap_print_projections(entities, input->projection_value);
                list_chain_hashmap_free(&entities);
            }
        }
        else
            printf("An error occurred while reading the file\n");
        
        free(content);
    } else {
        printf("The '%s' collection does not exist.\n", input->collection);
        //printf("The '%s' path have not could be resolved\n", path);
    }
    
    free(path);
    fclose(file);
}

/*
 * Insert a sql entity in an existing collection or create one for this entity
 */
void sql_insert(command_line *input) {
    
    FILE *file          = NULL;
    char *path          = NULL;
    char *content       = NULL;
    long length;
    
    path = malloc(sizeof(char) * (strlen(input->collection) + strlen(BASE_NO_SQL) + 6));
    
    sprintf(path, "%s/%s.txt", BASE_NO_SQL, input->collection);
    
    // Try to open the file in 'r' mode. If file is NULL, create the file with the 'w' mode
    file = fopen(path, "r+b");
    
    if (file == NULL)
        file = fopen(path, "wb");

    fseek(file, 0, SEEK_END);
    
    if (input->action_value != NULL) {
        content = JSON_stringify(input->action_value);
        length = strlen(content);

        if (fwrite(content, sizeof(char), length, file) != length)
            printf("An error occurred during the writing of the data\n");
        else
            printf("1 object has been inserted successfully !\n");
        
        free(content);
    }
    else
        printf("Syntax error in the data to insert\n");
    
    free(path);
    fclose(file);
}

/*
 * Update some values from existing sql entities, respecting some constraints
 */
void sql_set(command_line *input) {
    
    t_hashmap *entity       = NULL;
    FILE *file              = NULL;
    char *path              = NULL;
    char *pos               = NULL;
    char *JSON_string       = NULL;
    char *content           = NULL;
    long length             = 0;
    long file_length        = 0;
    long cur_position       = 0;
    int nb_entity_updated   = 0;
    
    if (input->where_value == NULL) {
        printf("The WHERE clause is missing or incorrect\n");
        return;
    }
    
    path = malloc(sizeof(char) * (strlen(input->collection) + strlen(BASE_NO_SQL) + 6));
    sprintf(path, "%s/%s.txt", BASE_NO_SQL, input->collection);
    
    file = fopen(path, "rb");
    if (file != NULL) {
        // Get the length of the file and read it
        fseek(file, 0, SEEK_END);
        file_length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        content = malloc(sizeof(char) * (file_length + 1));
        
        if (fread(content, sizeof(char), file_length, file) == file_length) {
            // Get each entity of the file
            while (content[cur_position] != '\0' && (pos = strchr(&content[cur_position], '}')) != NULL) {
                pos++;

                if (*(pos-2) == '\\')
                    continue;
                
                length = pos - &content[cur_position];
                
                // Get the JSON string of the current object
                JSON_string = malloc(sizeof(char) * (length+1));
                strncpy(JSON_string, &content[cur_position], length);
                JSON_string[length] = '\0';
                
                // Parse the JSON string to get a hashmap of it
                entity = JSON_parse(JSON_string);
                
                // Update the entity's values in the file if it respects the constraints of the -where clause
                if (is_matching(entity, input->where_value) == 0) {
                    update_entity(entity, input->action_value);
                    char *str = JSON_stringify(entity);
                    
                    content = replace_substring(content, cur_position, length, str);
                    cur_position += strlen(str);
                    nb_entity_updated++;
                    free(str);
                }
                else
                    cur_position += length;
             
                free(JSON_string);
                
                if (entity != NULL)
                    hashmap_free(&entity);
            }
            
            fclose(file);
            
            // Open the file in wb mode to replace the content of the file by the content string
            fopen(path, "wb");
            fwrite(content, sizeof(char), strlen(content), file);
            
            printf("%d objects has been updated successfully !\n", nb_entity_updated);
        }
        else
            printf("An error occurred while reading the file\n");
        
        free(content);
        fclose(file);
    } else {
        printf("The '%s' collection does not exist.\n", input->collection);
        //printf("The '%s' path have not could be resolved\n", path);
    }
    
    free(path);
}

/*
 * Remove existing sql entities respecting some constraints
 */
void sql_remove(command_line *input) {
    
    t_hashmap *entity       = NULL;
    FILE *file              = NULL;
    char *path              = NULL;
    char *pos               = NULL;
    char *JSON_string       = NULL;
    char *content           = NULL;
    long length             = 0;
    long file_length        = 0;
    long cur_position       = 0;
    int nb_entity_deleted   = 0;
    
    path = malloc(sizeof(char) * (strlen(input->collection) + strlen(BASE_NO_SQL) + 6));

    sprintf(path, "%s/%s.txt", BASE_NO_SQL, input->collection);
    
    file = fopen(path, "rb");

    if (file != NULL) {
        // Get the length of the file and read it
        fseek(file, 0, SEEK_END);
        file_length = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        content = malloc(sizeof(char) * (file_length + 1));
        
        if (fread(content, sizeof(char), file_length, file) == file_length) {
            // Get each entity of the file
            while (content[cur_position] != '\0' && (pos = strchr(&content[cur_position], '}')) != NULL && *(pos-1) != '\\') {
                pos += 1;
                length = pos - &content[cur_position];
                
                // Get the JSON string of the current object
                JSON_string = malloc(sizeof(char) * (length + 1));
                strncpy(JSON_string, &content[cur_position], length);
                JSON_string[length] = '\0';
                
                // Parse the JSON string to get a hashmap of it
                entity = JSON_parse(JSON_string);
                
                // Delete the entity in the file if it respects the constraints of the -remove command
                if (is_matching(entity, input->action_value) == 0) {
                    content = revert_substr(content, cur_position, length);
                    nb_entity_deleted++;
                }
                else
                    cur_position += length;
                
                free(JSON_string);
            }
            
            fclose(file);
            
            // Open the file in wb mode to replace the content of the file by the content string
            fopen(path, "wb");
            fwrite(content, sizeof(char), strlen(content), file);
            
            printf("%d objects has been deleted successfully !\n", nb_entity_deleted);
            
            if (entity != NULL)
                hashmap_free(&entity);
        }
        else
            printf("An error occurred while reading the file\n");
        
        free(content);
    } else {
        printf("The '%s' collection does not exist.\n", input->collection);
        //printf("The '%s' path have not could be resolved\n", path);
    }
    
    free(path);
    fclose(file);
}
