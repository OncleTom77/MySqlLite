#include "includes.h"

int main(int argc, char **argv) {
    
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
    
    command_line_free(&input);
    
    return EXIT_SUCCESS;
}
