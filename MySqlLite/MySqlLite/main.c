//
//  main.c
//  MySqlLite
//
//  Created by Thomas Fouan on 22/06/2016.
//  Copyright © 2016 Thomas Fouan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "hashMap.h"

int main(int argc, const char * argv[]) {
    
    t_hashmap* hashmap = hashmap_create(10, 2, 0.7);
    
    int var = 8;
    char* var2 = "ma valeur";
    
    hashmap_put(hashmap, "toto", &var, TYPE_INT);
    hashmap_put(hashmap, "variable", var2, TYPE_STRING);
    
    hashmap_print(hashmap);
    
    
    return 0;
}
