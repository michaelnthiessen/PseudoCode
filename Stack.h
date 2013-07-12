//
//  Stack.h
//  Stack
//
//  Created by Michael Thiessen on 2013-06-20.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#ifndef Stack_Stack_h
#define Stack_Stack_h

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

// Define our struct here
struct Stack {
    void **pointers;
    int size, numItems;
};

typedef struct Stack Stack;

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

Stack *stackCreateNewStack();

void stackPushObject(Stack *stack, void *object);
void *stackPopObject(Stack *stack);
void *stackReturnTopObject(Stack *stack);
int stackIsEmpty(Stack *stack);

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

#endif
