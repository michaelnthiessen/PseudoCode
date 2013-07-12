//
//  helperFunctions.c
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-08.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#include "helperFunctions.h"

// Simple linear search
int isStringInArray(char* string, char** array, int numElements)
{
    assert(string != NULL);
    assert(array != NULL);
    assert(numElements > 0);
    
    for (int i = 0; i < numElements; i++)
    {
        if (strcmp(string, array[i]) == 0)
        {
            return 1;
        }
    }
    
    return 0;
}

int isCharInArray(char c, char** array, int numElements)
{
    assert(array != NULL);
    assert(numElements > 0);
    
    for (int i = 0; i < numElements; i++)
    {
        if (c == array[i][0])
        {
            return 1;
        }
    }
    
    return 0;
}

double max(double one, double two)
{
    return one > two ? one : two;
}


















