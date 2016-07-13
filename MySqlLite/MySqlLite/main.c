#include "includes.h"

int main(int argc, char **argv) {
    
    /*t_hashmap* hashmap = hashmap_create(10, 2, 0.7);
    
    int var = 8;
    char* var2 = "ma valeur";
    double var3 = 16.75;
    int var4 = 123456789;
    char* var5 = "la la lalala";
    double var6 = 987654321.123456;
    
    hashmap_put(hashmap, "toto", &var, TYPE_INT);
    hashmap_put(hashmap, "variable", var2, TYPE_STRING);
    hashmap_put(hashmap, "coucou", &var3, TYPE_DOUBLE);
    hashmap_put(hashmap, "baba", &var4, TYPE_INT);
    hashmap_put(hashmap, "yoyo", var5, TYPE_STRING);
    hashmap_put(hashmap, "Au revoir", &var6, TYPE_DOUBLE);
    
    hashmap_print(hashmap);
    
    char* string = JSON_stringify(hashmap);
    printf("%s\n", string);
    
    t_hashmap* myHashmap = JSON_parse(string);
    hashmap_print(myHashmap);
    
    hashmap_free(&hashmap);
    hashmap_free(&myHashmap);*/
    
    //int result = 0;
    command_line *input;
    input = analyse_arguments(argc, argv);
    
    if (input == NULL)
        return EXIT_FAILURE;
    
    switch (input->act) {
        case A_FIND:
            sql_find(input);
            break;
            
        case A_INSERT:
            sql_insert(input);
            break;
            
        case A_SET:
            sql_set(input);
            break;
            
        case A_REMOVE:
            sql_remove(input);
            break;
            
        default:
            printf("Unknown action !\n");
    }
    /*
    if(result == 0) {
        printf("Operation successfuly done.\n");
    } else {
        printf("An error occurred while trying to achieve the operation.\n");
    }*/
    
    free(input);
    
    return EXIT_SUCCESS;
}
