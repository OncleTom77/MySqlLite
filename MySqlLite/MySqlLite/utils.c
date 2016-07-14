#include "includes.h"

int get_hash_code(char *key) {
    
    int hash;
    int i;
    
    hash = 5;
    
    for(i = 0; key && *key != '\0'; i++) {
        hash += *key * i;
        key++;
    }
    
    return hash;
}

/*
 * Return 0 if value1 and value2 are equals. Else, return -1
 */
int is_equal(void *value1, void *value2, type_var type) {
    
    if ((type == TYPE_STRING && strcmp((char *) value1, (char *) value2) == 0)
            || (type == TYPE_INT && *((int *) value1) == *((int *) value2))
            || (type == TYPE_DOUBLE && *((double *) value1) == *((double *) value2)))
        return 0;
    
    return -1;
}

/*
 * Return the content string without the characters between start_pos and start_pos + length
 */
char *revert_substr(char *content, long start_pos, long length) {
    
    char *new_string    = content;
    long old_length     = 0;
    long count          = 0;
    int i;
    
    if (content && start_pos >= 0 && length >= 0 && start_pos+length <= (old_length = strlen(content))) {
        long new_length = old_length - length;
        new_string      = malloc(sizeof(char)*(new_length+1));
        
        // Copy all characters that are not in the range between start_pos and start_pos + length
        for (i = 0; i < old_length; i++) {
            if (i >= start_pos && i < start_pos + length)
                continue;

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
    
    char *new_string = content;
    int count 		 = 0;
    long i;
    long content_length;
    long str_length;

    if (!content || !str)
    	return NULL;

    content_length 	= strlen(content);
    str_length 		= strlen(str);
    
    if (str && pos >= 0 && length >= 0 && pos + length <= content_length) {
        new_string = malloc(sizeof(char) * (content_length - length + str_length + 1));
        for (i = 0; i < pos; i++, count++)
            new_string[count] = content[i];

        for (i = 0; i < str_length; i++, count++)
            new_string[count] = str[i];

        for(i = pos + length; i < content_length; i++, count++)
            new_string[count] = content[i];
        
        new_string[count] = '\0';
        free(content);
        content = new_string;
    }
    
    return new_string;
}