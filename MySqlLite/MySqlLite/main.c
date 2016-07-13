//
//  main.c
//  MySqlLite
//
//  Created by Thomas Fouan on 22/06/2016.
//  Copyright © 2016 Thomas Fouan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "sql.h"
#include "command.h"

int main(int argc, char *argv[]) {
    
    command_line *input;
    input = analyse_arguments(argc, argv);
    
    if(input == NULL) {
        return -1;
    }
    
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
    
    free(input);
    return 0;
}
