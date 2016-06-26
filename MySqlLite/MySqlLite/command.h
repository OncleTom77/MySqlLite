//
//  command.h
//  MySqlLite
//
//  Created by Thomas Fouan on 26/06/2016.
//  Copyright © 2016 Thomas Fouan. All rights reserved.
//

#ifndef command_h
#define command_h

#include <stdio.h>
#include <string.h>

typedef enum action {
    A_NONE,
    A_FIND,
    A_INSERT,
    A_SET,
    A_REMOVE
} action;

#endif /* command_h */
