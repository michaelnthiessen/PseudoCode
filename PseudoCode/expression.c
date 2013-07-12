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
#include "variableManager.h"

const int NUM_OPERATORS = 4;
const char *OPERATORS[NUM_OPERATORS] = {"+", "-", "/", "*"};

const int NUM_LOGICAL_OPERATORS = 2;
const char *LOGICAL_OPERATORS[NUM_LOGICAL_OPERATORS] = {"AND", "OR"};

const int NUM_COMPARISON_OPERATORS = 3;
const char *COMPARISON_OPERATORS[NUM_COMPARISON_OPERATORS] = {">", "<", "=="};

char *convertToPostfix_Int(char *infix);
void pushChar(char c, Stack *stack);
int evaluatePostfix_Int(char *postfix);
int operatorDoesTakePrecedence(char operator1, char operator2);

/*
 Recursively evaluates the expression, 
 resolving any variable values in the process
 */
int evaluateIntExpression (char *expression)
{
    assert(expression != NULL);
    
    char *postfix = convertToPostfix_Int(expression);
    
    return evaluatePostfix_Int(postfix);
}

int evaluateComparison(char *expression)
{
    assert(expression != NULL);
    
    int result, op1, op2;
    int leftLength = 0, rightLength = 0, comparisonLength = 0;
    int leftHand = 1;
    int length = (int)strlen(expression);
    
    char comparison[3];
    char *left = malloc(sizeof(char) * length);
    char *right = malloc(sizeof(char) * length);
    
    for (int i = 0; i < length; i++)
    {
        // Determine the left hand side of the equation first
        if (leftHand)
        {
            left[leftLength] = expression[i];
            leftLength++;
            
            // Fill in the left hand side
            while (!isCharInArray(expression[i + 1], (char **)COMPARISON_OPERATORS, NUM_COMPARISON_OPERATORS))
            {
                left[leftLength] = expression[i + 1];
                leftLength++;
                i++;
            }
            
            // Fill in the comparison operator
            while (isCharInArray(expression[i + 1], (char **)COMPARISON_OPERATORS, NUM_COMPARISON_OPERATORS))
            {
                comparison[comparisonLength] = expression[i + 1];
                comparisonLength++;
                i++;                
            }
            
            comparison[comparisonLength] = '\0';
            left[leftLength] = '\0';
            leftHand = 0;
        }
        else
        {
            right[rightLength] = expression[i];
            rightLength++;
        }
    }
    
    op1 = evaluateIntExpression(left);
    op2 = evaluateIntExpression(right);
    
    // Greater than
    if (strcmp(comparison, COMPARISON_OPERATORS[0]) == 0)
    {
        result = op1 > op2 ? 1 : 0;
    }
    else if (strcmp(comparison, COMPARISON_OPERATORS[1]) == 0)
    {
        result = op1 < op2 ? 1 : 0;
    }
    else if (strcmp(comparison, COMPARISON_OPERATORS[2]) == 0)
    {
        result = op1 == op2 ? 1 : 0;
    }
    else
    {
        result = 0;
        printf("ERR: Invalid comparison '%s'\n", expression);
    }    
    
    return result;
}

int evaluateBoolExpression(char *expression)
{
    assert(expression != NULL);
    
    return 0;
}

/*
 Converts a boolean infix expression to postfix
 */
char *convertToPostfix_Bool(char *infix)
{
    assert(infix != NULL);
    
    int lengthInfix = (int)strlen(infix);
    int lengthPostfix = 0;
    Stack *stack = stackCreateNewStack();
    
    // Converting to postfix rearranges and removes parentheses,
    // so it will always be of equal or shorter length
    char *postfix = malloc(sizeof(char) * lengthInfix);
    
    for (int i = 0; i < lengthInfix; i++)
    {
        char c = infix[i];
        
        // Check if it's an operand
        if (isdigit(c))
        {
            postfix[lengthPostfix] = infix[i];
            lengthPostfix++;
            
            while (isdigit(infix[i + 1]))
            {
                postfix[lengthPostfix] = infix[i + 1];
                lengthPostfix++;
                i++;
            }
            
            postfix[lengthPostfix] = ' ';
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
        else if (isCharInArray(c, (char **)OPERATORS, NUM_OPERATORS))
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
        // Find the variable and replace it with it's numeric value
        else if (islower(c))
        {
            char *varName = malloc(sizeof(char) * 50);
            int varLength = 0;
            int *var;
            
            varName[varLength] = infix[i];
            varLength++;
            
            while (!(isCharInArray(infix[i + 1], (char **)OPERATORS, NUM_OPERATORS) || infix[i+1] == '(' || infix[i+1] == ')' || infix[i+1] == ' '))
            {
                varName[varLength] = infix[i + 1];
                varLength++;
                i++;
            }
            
            varName[varLength] = '\0';
            
            // Search for the variable
            var = variableReturnVariable(varName);
            
            if (var == NULL)
            {
                abort();
            }
            else
            {
                char *result;
                if (*var != 0)
                {
                    result = malloc((ceil(log10(*var))+1)*sizeof(char));
                    sprintf(result, "%d", *var);
                }
                else
                {
                    result = malloc(sizeof(char) * 2);
                    result = "0";
                }
                
                postfix = strcat(postfix, result);
                lengthPostfix += strlen(result);
                
                postfix[lengthPostfix] = ' ';
                lengthPostfix++;
            }
        }
    }
    
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
 Converts an infix expression to postfix
 */
char *convertToPostfix_Int(char *infix)
{
    assert(infix != NULL);
    
    int lengthInfix = (int)strlen(infix);
    int lengthPostfix = 0;
    Stack *stack = stackCreateNewStack();
    
    // Converting to postfix rearranges and removes parentheses,
    // so it will always be of equal or shorter length
    char *postfix = malloc(sizeof(char) * lengthInfix);
    
    for (int i = 0; i < lengthInfix; i++)
    {
        char c = infix[i];
        
        // Check if it's an operand
        if (isdigit(c))
        {
            postfix[lengthPostfix] = infix[i];
            lengthPostfix++;
            
            while (isdigit(infix[i + 1]))
            {
                postfix[lengthPostfix] = infix[i + 1];
                lengthPostfix++;
                i++;
            }
            
            postfix[lengthPostfix] = ' ';
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
        else if (isCharInArray(c, (char **)OPERATORS, NUM_OPERATORS))
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
        // Find the variable and replace it with it's numeric value
        else if (islower(c))
        {
            char *varName = malloc(sizeof(char) * 50);
            int varLength = 0;
            int *var;
            
            varName[varLength] = infix[i];
            varLength++;
            
            while (!(isCharInArray(infix[i + 1], (char **)OPERATORS, NUM_OPERATORS) || infix[i+1] == '(' || infix[i+1] == ')' || infix[i+1] == ' '))
            {
                varName[varLength] = infix[i + 1];
                varLength++;
                i++;
            }
            
            varName[varLength] = '\0';
            
            // Search for the variable
            var = variableReturnVariable(varName);
            
            if (var == NULL)
            {
                abort();
            }
            else
            {
                char *result;
                if (*var != 0)
                {
                    result = malloc((ceil(log10(*var))+1)*sizeof(char));
                    sprintf(result, "%d", *var);
                }
                else
                {
                    result = malloc(sizeof(char) * 2);
                    result = "0";
                }
                
                postfix = strcat(postfix, result);
                lengthPostfix += strlen(result);
                
                postfix[lengthPostfix] = ' ';
                lengthPostfix++;
            }
        }
    }
    
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
    assert(op1 > 0);
    assert(op2 > 0);
    
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
int evaluatePostfix_Int(char *postfix)
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
























