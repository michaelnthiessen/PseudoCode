//
//  variableManager.c
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-09.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#include "variableManager.h"

const int MAX_VARS = 100;
const int DEFAULT_VAL = 0;

int varArray[MAX_VARS];
char *nameArray[MAX_VARS];
int numVars = 0;

/*
 Defines a variable by putting the name into the array
 and setting the default value.
 
 NOTE: Only works with integers right now.
 */
void variableDefineNewVariable(char *varName)
{
    assert(varName != NULL);
    char *varName_Heap;
    
    // Make sure we don't go past the end of our array
    if (numVars < MAX_VARS)
    {
        // Ensure that malloc worked
        varName_Heap = malloc(sizeof(char) * strlen(varName));
        if (varName_Heap == NULL)
        {
            printf("ERR: Could not allocate memory for new variable.\n");
            abort();
        }
        
        // Copy in the name onto the heap, and point to it
        strcpy(varName_Heap, varName);
        
        varArray[numVars] = DEFAULT_VAL;
        nameArray[numVars] = varName_Heap;
        
        numVars++;
    }
}

/*
 Returns a pointer to the variable name given
 */
void *variableReturnVariable(char *varName)
{
    assert (varName != NULL);
    int found = 0;
    
    // Linear search (for now)
    for (int i = 0; i < numVars || found; i++)
    {
        if (strcmp(varName, nameArray[i]) == 0)
        {
            found = 1;
            return &varArray[i];
        }
    }
    
    // No variable was found
    printf("ERR: Undefined variable '%s'.\n", varName);
    return NULL;
}
































