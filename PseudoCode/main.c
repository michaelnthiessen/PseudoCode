//
//  main.c
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-08.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helperFunctions.h"
#include "constants.h"

//------------------ Functions ----------------------
void interpretProgramFile(char *programFilePath);
void parseSingleOperandInstruction(char *token);

int main(int argc, const char * argv[])
{
    // Open up a program file 
    interpretProgramFile((char *)FILE_PATH);

    printf("\n\nProcessing Completed.\n");
    return 0;
}

void interpretProgramFile(char *programFilePath)
{
    FILE *file;
    char line[LINE_MAX];
    char *token = "";
    
    // Attempt to open the file
    file = fopen(programFilePath, "r");
    if (!file)
    {
        printf("ERR: Failed to open the file '%s'\n", programFilePath);
        return;
    }
    
    // Process line by line
    while (fgets(line, LINE_MAX, file))
    {
//        printf("%s", line);
        
        // Tokenize splitting on whitespace
        token = strtok(line, " ");
        while (token != NULL)
        {
            // Check to see how many operands the current instruction requires
            if (isStringInArray(token, (char **)SINGLE_OPERAND_INSTRUCTIONS, NUM_SINGLE_OPERAND_INSTRUCTIONS))
            {
                parseSingleOperandInstruction(token);
            }
            
            token = strtok(NULL, " ");
        }
    }
}

void parseSingleOperandInstruction(char *token)
{
    char *operand = strtok(NULL, " ");
    int endChar = (int)strlen(operand) - 1;
    
    // Remove extraneous return characters
    if (operand[endChar] == '\n')
    {
        operand[endChar] = '\0';
    }
    
    // VAR - define a variable
    if (strcmp(token, SINGLE_OPERAND_INSTRUCTIONS[0]) == 0)
    {
        printf("Defining a new variable '%s'\n", operand);
    }
    // PRINT - print the contents of a variable
    else if (strcmp(token, SINGLE_OPERAND_INSTRUCTIONS[1]) == 0)
    {
        printf("Printing %s\n", operand);
    }
}






























