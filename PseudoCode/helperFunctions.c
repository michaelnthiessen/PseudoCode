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

/*
 Create a new string with the whitespace trimmed from it
 and then copy it back into the original location
 */
void trimwhitespace(char *str)
{
    char *newStr = malloc(sizeof(char) * strlen(str));
    int index = 0, begin = 0;
    
    // Leading whitespace
    while (isCharInArray(str[begin], (char **)WHITESPACE, NUM_WHITESPACE))
    {
        begin++;
    }
    
    // Load into the new string while the next character is not whitespace
    while (!isCharInArray(str[begin], (char **)WHITESPACE, NUM_WHITESPACE))
    {
        newStr[index++] = str[begin++];
    }
    
    newStr[index] = '\0';
    
    // Copy the new str into the old location
    memcpy(str, newStr, strlen(newStr));
    
    return;
}

double max(double one, double two)
{
    return one > two ? one : two;
}


















