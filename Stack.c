//
//  Stack.c
//  Stack
//
//  Created by Michael Thiessen on 2013-06-20.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Stack.h"

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------
const float SIZE_INCREMENT_FACTOR = 2;

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------
void checkResize(Stack *stack);

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

// Initialize our Stack
Stack *stackCreateNewStack()
{
    // Create our stack
    Stack *newStack = malloc(sizeof(Stack));
    
    if (newStack != NULL)
    {
        newStack->size = 0;
        newStack->numItems = 0;
    }
    else
    {
        printf("ERR: Failed to allocate memory for new Stack\n");
    }
    
    return newStack;
}

int stackIsEmpty(Stack *stack)
{
    assert(stack != NULL);
    return stack->numItems ? 0 : 1;
}

void *stackPopObject(Stack *stack)
{
    assert(stack != NULL);
    assert(stack->numItems > 0);
    
    void *returnVal = stack->pointers[stack->numItems - 1];
    
    // Decrease size of the array
    stack->numItems--;
    
    // Resize the array smaller
    checkResize(stack);
    
    return returnVal;
}

void *stackReturnTopObject(Stack *stack)
{
    assert(stack != NULL);
    return stack->pointers[stack->numItems - 1];
}

// Add the object after the specified index
// If the index is too large, simply add to the end
void stackPushObject(Stack *stack, void *object)
{
    assert(stack != NULL);
    assert(object != NULL);
    
    // Check to see if we need to allocate more memory
    checkResize(stack);
    
    int index = stack->numItems;
    
    // Place in the new value
    stack->pointers[index] = object;
    stack->numItems++;    
}

// Check whether we need to resize the memory allocated
// to the stack or not
void checkResize(Stack *stack)
{
    assert(stack != NULL);
    
    int newSize = -1;
    
    // Check if we need to increase or decrease
    // OR the special case of the first item
    if (stack->numItems == 0)
    {
        newSize = 1;
    }
    else if (stack->numItems < (stack->size / SIZE_INCREMENT_FACTOR))
    {
        newSize = stack->size / SIZE_INCREMENT_FACTOR;
    }
    else if (stack->numItems == stack->size)
    {
        newSize = stack->size * SIZE_INCREMENT_FACTOR;
    }
    
    if (newSize > 0)
    {
        // If it is the first one we must use malloc
        if (newSize == 1)
        {
            stack->pointers = malloc(newSize * sizeof(void *));
        }
        else
        {
            stack->pointers = realloc(stack->pointers, newSize * sizeof(void *));
        }
        
        // Update our new size
        stack->size = newSize;
    
        // Check to make sure everything was allocated properly
        if (stack->pointers == NULL)
        {
            printf("Re-sizing of Stack has failed\n");
            abort();
        }
    }
}





















