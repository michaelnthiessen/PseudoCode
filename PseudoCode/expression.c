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
#include <assert.h>
#include "helperFunctions.h"
#include "expression.h"
#include "variableManager.h"
#include "constants.h"

// ----------------------------- Structs / Enums ---------------------------

typedef enum {
    NUM_OPERAND,
    LOG_OPERAND,
    NUM_OPERATOR,
    LOG_OPERATOR,
    COMP_OPERATOR
} TokenType;

typedef struct {
    TokenType type;
    void *value;
} Token;

// ----------------------------- Functions ----------------------------

int convertToPostfix_Int(char *infix);
void pushChar(char c, Stack *stack);
int evaluatePostfix_Int(Token *postfix, int numTokens);
int operatorDoesTakePrecedence(char operator1, char operator2);

int addNumericalOperandToTokenList(char *operand, Token *tokens, int numTokens);
void addNumericalOperatorToTokenList(char *operand, Token *tokens, int numTokens);

/*
 Recursively evaluates the expression, 
 resolving any variable values in the process
 */
int evaluateIntExpression (char *expression)
{
    assert(expression != NULL);
    
    return convertToPostfix_Int(expression);
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
 Takes in the pointer to a 
 */
int addNumericalOperandToTokenList(char *operand, Token *tokens, int numTokens)
{
    assert(operand != NULL);
    assert(tokens != NULL);
    assert(numTokens > -1);
    
    char tempDigits[50];
    int tempLength = 0;
    int i = 0;
    
    Token *newToken = malloc(sizeof(Token));
    int *value = malloc(sizeof(int));
    
    // Make sure the memory was allocated
    if (newToken == NULL || value == NULL) abort();
    
    // Store the digits into a temporary string
    tempDigits[tempLength++] = operand[i];
    
    while (isdigit(operand[i + 1]))
    {
        tempDigits[tempLength++] = operand[i++ + 1];
    }
    
    tempDigits[tempLength] = '\0';
    
    // Convert the string into an integer and store it in a Token
    *value = (int)strtol(tempDigits, NULL, 10);
    
    newToken->type = NUM_OPERAND;
    newToken->value = value;
    
    tokens[numTokens] = *newToken;
    
    return i;
}

void addNumericalOperatorToTokenList(char *operation, Token *tokens, int numTokens)
{
    assert(operation != NULL);
    assert(tokens != NULL);
    assert(numTokens > -1);
    
    Token *newToken = malloc(sizeof(Token));
    char *tempOperation = malloc(sizeof(char));
    
    // Make sure the memory was allocated
    if (newToken == NULL || tempOperation == NULL) abort();
    
    *tempOperation = *operation;
    
    newToken->type = NUM_OPERATOR;
    newToken->value = tempOperation;
    
    tokens[numTokens] = *newToken;
    
    return;
}

/*
 Converts an infix expression to postfix
 */
int convertToPostfix_Int(char *infix)
{
    assert(infix != NULL);
    
    int lengthInfix = (int)strlen(infix);
    Stack *stack = stackCreateNewStack();
    
    // Converting to postfix rearranges and removes parentheses,
    // so it will always be of equal or shorter length
    Token *tokens = malloc(sizeof(Token) * lengthInfix);
    int numTokens = 0;
    
    for (int i = 0; i < lengthInfix; i++)
    {
        char c = infix[i];
        
        // Check if it's an operand
        if (isdigit(c))
        {
            i += addNumericalOperandToTokenList(&infix[i], tokens, numTokens++);
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
                addNumericalOperatorToTokenList((char *)stackPopObject(stack), tokens, numTokens++);
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
                    addNumericalOperatorToTokenList((char *)stackPopObject(stack), tokens, numTokens++);
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
            Token *newToken = malloc(sizeof(Token));
            
            if (newToken == NULL) abort();
            
            varName[varLength] = infix[i];
            varLength++;
            
            while (!(isCharInArray(infix[i + 1], (char **)OPERATORS, NUM_OPERATORS) || infix[i+1] == '(' || infix[i+1] == ')' || infix[i+1] == ' '))
            {
                varName[varLength] = infix[i + 1];
                varLength++;
                i++;
            }
            
            varName[varLength] = '\0';
            
            // Get the value of the variable
            var = variableReturnVariable(varName);
            
            if (var == NULL)
            {
                abort();
            }
            else
            {
                // We don't need to copy the value of the pointer because the variable cannot change it's
                // value until after we have completely evaluated the expression anyways
                newToken->type = NUM_OPERAND;
                newToken->value = var;
                
                tokens[numTokens++] = *newToken;
            }
        }
    }
    
    while (!stackIsEmpty(stack))
    {
        addNumericalOperatorToTokenList((char *)stackPopObject(stack), tokens, numTokens++);
    }
    
    return evaluatePostfix_Int(tokens, numTokens);
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
int evaluatePostfix_Int(Token *postfix, int numTokens)
{
    assert(postfix != NULL);
    
    int val = 0, operand1 = 0, operand2 = 0;
    Token *token = &postfix[0];
    Stack *stack = stackCreateNewStack();
    
    for (int i = 0; i < numTokens; i++)
    {
        if (token->type == NUM_OPERAND)
        {
            stackPushObject(stack, token);
        }
        else
        {
            // Get our operands as integers from the stack (with some casting/converting)
            operand2 = *(int *)((Token *)stackPopObject(stack))->value;
            operand1 = *(int *)((Token *)stackPopObject(stack))->value;
            
            switch (*(char *)(token->value)) {
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
            
            // Push the result onto the stack
            Token *result = malloc(sizeof(Token));
            if (result != NULL)
            {
                result->type = NUM_OPERAND;
                int *tempVal = malloc(sizeof(int));
                *tempVal = val;
                result->value = tempVal;
            }

            stackPushObject(stack, result);
        }
    }
    
    return *(int *)((Token *)stackPopObject(stack))->value;
}

























