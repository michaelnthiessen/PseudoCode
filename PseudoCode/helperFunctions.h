//
//  helperFunctions.h
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-08.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#ifndef PseudoCode_helperFunctions_h
#define PseudoCode_helperFunctions_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "constants.h"

int isStringInArray(char* string, char** array, int numElements);
int isCharInArray(char c, char **array, int numElements);
double max(double one, double two);

void trimwhitespace(char *str);

#endif
