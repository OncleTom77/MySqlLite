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
#include "hashMap.h"

t_hashmap* JSON_parse(char*);
char* JSON_stringify(t_hashmap*);

#endif /* sql_h */
