//
//  expression.c
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-09.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "helperFunctions.h"
#include "expression.h"

const int NUM_OPERATORS = 4;
const char *OPERATORS[NUM_OPERATORS] = {"+", "-", "/", "*"};

char *convertToPostfix(char *infix);
void pushChar(char c, Stack *stack);
int evaluatePostfix(char *postfix);
int operatorDoesTakePrecedence(char operator1, char operator2);

/*
 Recursively evaluates the expression, 
 resolving any variable values in the process
 */
int evaluateExpression (char *expression)
{
    assert(expression != NULL);
    
    char *postfix = convertToPostfix(expression);
    
    return evaluatePostfix(postfix);
}

/*
 Converts an infix expression to postfix
 */
char *convertToPostfix(char *infix)
{
    assert(infix != NULL);
    
    int lengthInfix = (int)strlen(infix);
    int lengthPostfix = 0;
    Stack *stack = stackCreateNewStack();
    
    // Converting to postfix rearranges and removes parentheses,
    // so it will always be of equal or shorter length
    char *postfix = malloc(sizeof(char) * lengthInfix);
    char *token = strtok(infix, " ");
    
    do
    {
        char c = token[0];
        char *ch = malloc(sizeof(char));
        ch = token;
        
        // Check if it's an operand
        if (isdigit(c))
        {
            strcat(postfix, ch);
            lengthPostfix += strlen(ch);
            
            strcat(postfix, " ");
            lengthPostfix++;
        }
        else if (c == '(')
        {
            pushChar('(', stack);
        }
        else if (c == ')')
        {
            // While the stack is not empty, and the top object is not a left parenthesis
            while (!stackIsEmpty(stack) && *(char *)stackReturnTopObject(stack) != '(')
            {
                postfix[lengthPostfix] = *(char *)stackPopObject(stack);
                lengthPostfix++;
                
                postfix[lengthPostfix] = ' ';
                lengthPostfix++;                
            }
            
            stackPopObject(stack);
        }
        else if (isStringInArray(ch, (char **)OPERATORS, NUM_OPERATORS))
        {
            if (stackIsEmpty(stack) || *(char *)stackReturnTopObject(stack) == '(')
            {
                pushChar(c, stack);
            }
            else
            {
                while (!stackIsEmpty(stack) && *(char *)stackReturnTopObject(stack) != '(' && operatorDoesTakePrecedence(*(char *)stackReturnTopObject(stack), c))
                {                    
                    postfix[lengthPostfix] = *(char *)stackPopObject(stack);
                    lengthPostfix++;
                    
                    postfix[lengthPostfix] = ' ';
                    lengthPostfix++;
                }
                pushChar(c, stack);
            }
        }
    } while ((token = strtok(NULL, " ")));
    
    while (!stackIsEmpty(stack))
    {
        postfix[lengthPostfix] = *(char *)stackPopObject(stack);
        lengthPostfix++;
        
        postfix[lengthPostfix] = ' ';
        lengthPostfix++;
    }
    
    postfix[lengthPostfix] = '\0';
    
    return postfix;
}

/*
 Determines whether op1 takes precedence over op2
 */
int operatorDoesTakePrecedence(char op1, char op2)
{
    int result = 0;
    
    // * and / automatically take precedence over everything
    if (op1 == '*' || op1 == '/')
    {
        result = 1;
    }
    else if (op2 == '+' || op2 == '-')
    {
        result = 1;
    }
    
    return result;
}

void pushChar(char c, Stack *stack)
{
    assert(stack != NULL);
    
    char *newChar = malloc(sizeof(char));
    *newChar = c;
    stackPushObject(stack, newChar);
}

/*
 Push each operand on the stack, and when
 an operator is encountered, pop off the correct
 number of operands needed
 */
int evaluatePostfix(char *postfix)
{
    assert(postfix != NULL);
    
    int val = 0, operand1 = 0, operand2 = 0;
    char *token = strtok(postfix, " ");
    Stack *stack = stackCreateNewStack();
    
    do
    {
        if (isdigit(token[0]))
        {
            stackPushObject(stack, token);
        }
        else if (token[0] != ' ')
        {
            // Get our operands as integers from the stack (with some casting/converting)
            operand2 = (int)strtol((char *)stackPopObject(stack), NULL, 10);
            operand1 = (int)strtol((char *)stackPopObject(stack), NULL, 10);
            
            switch (token[0]) {
                case '+':
                    val = operand1 + operand2;
                    break;
                    
                case '-':
                    val = operand1 - operand2;
                    break;
                    
                case '/':
                    val = operand1 / operand2;
                    break;
                    
                case '*':
                    val = operand1 * operand2;
                    break;
                    
                default:
                    break;
            }
            
            // Convert the int to a string and push onto the stack
            // The formula only works when val != 0
            char *result;
            if (val != 0)
            {
                result = malloc((ceil(log10(val))+1)*sizeof(char));
                sprintf(result, "%d", val);
            }
            else
            {
                result = malloc(sizeof(char) * 2);
                result = "0";
            }

            stackPushObject(stack, result);
        }
    } while ((token = strtok(NULL, " ")));
    
    return (int)strtol((char *)stackPopObject(stack), NULL, 10);
}

























