//
//  sql.h
//  MySqlLite
//
//  Created by Thomas Fouan on 22/06/2016.
//  Copyright © 2016 Thomas Fouan. All rights reserved.
//

#ifndef sql_h
#define sql_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashMap.h"
#include "command.h"

t_hashmap_entry *get_hashmap_entry_from_JSON(char *);
t_hashmap *JSON_parse(char *);

char *get_string_from_hashmap_entry(t_hashmap_entry *);
char *JSON_stringify(t_hashmap *);

int sql_find(command_line *);
int sql_insert(command_line *);
int sql_set(command_line *);
int sql_remove(command_line *);

#endif /* sql_h */
