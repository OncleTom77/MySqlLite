#include "includes.h"

/*
 * Store the current option in the appropriate field of command_line struct if it is valid
 */
int get_option(command_line *command_line, char *name, char *value) {
    
    int i;
    char *actions[] = {"-find", "-insert", "-set", "-remove"};
    char *options[] = {"-where", "-projection", "-sort"};
    char *parsed_value = get_value(value, strlen(value));
    t_hashmap *hashmap = NULL;
    
    if (parsed_value == NULL) {
        printf("Invalid value '%s'.\n", value);
        return -1;
    }
    
    // Check if the option is the collection
    if (strcmp(name, "-collection") == 0) {
        
        if (command_line->collection != NULL) {
            printf("The are 2 collections on the command line. Choose only one.\n");
            return -1;
        }
        command_line->collection = parsed_value;
        
        return 0;
    }
    
    if ((hashmap = JSON_parse(parsed_value)) != NULL) {
        // Check if the option is an action
        for (i = 0; i < 4; i++) {
            if (strcmp(name, actions[i]) == 0) {
                if (command_line->act != A_NONE) {
                    printf("There are 2 actions on the command line. Choose only one.\n");
                    return -1;
                }
                else if (i == 0)
                    command_line->act = A_FIND;
                else if (i == 1)
                    command_line->act = A_INSERT;
                else if (i == 2)
                    command_line->act = A_SET;
                else
                    command_line->act = A_REMOVE;
                
                command_line->action_value = hashmap;
                
                free(parsed_value);
                return 0;
            }
        }
        
        // Check if the option is an option for an action
        for (i = 0; i < 3; i++) {
            if (strcmp(name, options[i]) == 0) {
                if (i == 0 && command_line->where_value == NULL) {
                    command_line->where_value = hashmap;
                } else if (i == 1 && command_line->projection_value == NULL) {
                    // Create a chain list of t_hashmap_entry instead of t_hashmap to keep the order of the projections for printing
                    t_hashmap_entry *hashmap_entry = JSON_parse_list(parsed_value);
                    command_line->projection_value = hashmap_entry;
                    free(hashmap);
                } else if (i == 2 && command_line->sort_value == NULL) {
                    // Create a chain list of t_hashmap_entry instead of t_hashmap to keep the order of sort for printing
                    t_hashmap_entry *hashmap_entry = JSON_parse_list(parsed_value);
                    command_line->sort_value = hashmap_entry;
                    free(hashmap);
                } else {
                    printf("The '%s' option already exists on the command line.\n", options[i]);
                    return -1;
                }
                free(parsed_value);
                return 0;
            }
        }
    } else {
        printf("Syntax error for '%s' value\n", parsed_value);
        return -1;
    }
    
    
    printf("Unknown option '%s'.\n", name);

    return -1;
}

/*
 * Parse the value of the current option to get the same value if it braces by "" or not
 */
char *get_value(char *value, unsigned long length) {
    
    char *parsed_value = NULL;
    
    if (length > 2 && value[0] == '"' && value[length - 1] == '"') {
        parsed_value = malloc(sizeof(char)*length-1);
        strncpy(parsed_value, value+1, length-2);
        parsed_value[length-2] = '\0';
    }
    else if (length > 0) {
        parsed_value = malloc(sizeof(char)*length+1);
        strcpy(parsed_value, value);
        parsed_value[length] = '\0';
    }
    
    return parsed_value;
}

/*
 * Analyse the arguments passed in command line
 */
command_line *analyse_arguments(int argc, char **args) {
    
    int i;
    unsigned long length = 0;
    char *name;
    char *value;
    command_line *input = NULL;
    
    if (argc < 3) {
        printf("Not enough argument...\n");
        return NULL;
    }
    
    input = malloc(sizeof(command_line));

    input->collection       = NULL;
    input->act              = A_NONE;
    input->action_value     = NULL;
    input->where_value      = NULL;
    input->projection_value = NULL;
    input->sort_value       = NULL;
    
    for (i = 1; i < argc; i++) {
        if (strchr(args[i], '=') == NULL) {
            printf("Invalid option '%s'. Options must respect the following syntax : -option=\"value\".\n", args[i]);
            free(input);
            
            return NULL;
        }
        name    = strsep(&args[i], "=");
        value   = strsep(&args[i], "=");
        length  = strlen(value);
        
        if (get_option(input, name, value) != 0) {
            free(input);
            return NULL;
        }
    }
    
    if (input->collection == NULL) {
        printf("There is not collection on the command line. Add '-collection=\"MyCollection\"' to set the collection.\n");
        free(input);
        return NULL;
    }
    
    if (input->act == A_NONE) {
        printf("There is not action on the command line. Add :\n\t'-find={key:value[, key:value]}'\n\t'-insert={key:value[, key:value]}'\n\t'-set={key:value[, key:value]}'\n\t'-remove={key:value[, key:value]}'\n");
        free(input);
        return NULL;
    }
    
    return input;
}

/*
 * Free the command_line struct
 */
void command_line_free(command_line **input) {
    
    if((*input)->collection != NULL)
        free((*input)->collection);
    
    if((*input)->action_value != NULL)
        hashmap_free(&(*input)->action_value);
    
    if((*input)->where_value != NULL)
        hashmap_free(&(*input)->where_value);
    
    if((*input)->projection_value != NULL)
        hashmap_entry_free((*input)->projection_value);
    
    if((*input)->sort_value != NULL)
        hashmap_entry_free((*input)->sort_value);
    
    free(*input);
}
