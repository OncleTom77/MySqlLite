//
//  command.c
//  MySqlLite
//
//  Created by Thomas Fouan on 26/06/2016.
//  Copyright © 2016 Thomas Fouan. All rights reserved.
//

#include "command.h"

int check_valid_argument(char* arg) {
    
    int i;
    char* options[] = {"-collection", "-find", "-insert", "-set", "-remove"};
    
    char* pos = strchr(arg, '=');
    if(!pos) {
        return -1;
    }
    
    char* option = strsep(&arg, "=");
    char* value = strsep(&arg, "=");
    int valid_option = 0;
    
    for(i=0; i<5; i++) {
        if(strcmp(option, options[i]) == 0) {
            valid_option = 1;
            break;
        }
    }
    
    if(valid_option == 0 ||
       value[0] != '"' ||
       value[strlen(value) - 1] != '"') {
        return -1;
    }
    
    return 0;
}

int analyseArguments(int argc, char** args) {
    
    if(argc < 2) {
        printf("Not enough argument...");
    }
    int i;
    action act = A_NONE;
    char* collection;
    
    for(i=1; i<argc; i++) {
        
        if(strncmp(args[i], "-collection", 11) == 0) {
            
        }
    }
    
    return 0;
}
