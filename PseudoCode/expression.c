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
    NUM_OPERAND_TYPE,
    LOG_OPERAND_TYPE,
    NUM_OPERATOR_TYPE,
    LOG_OPERATOR_TYPE,
    COMP_OPERATOR_TYPE,
    RIGHT_PARENTHESIS_TYPE,
    LEFT_PARENTHESIS_TYPE
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
Token *createOperatorTokenFromChar(char *operator);
char *isOperator(char *operator);

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
    
    newToken->type = NUM_OPERAND_TYPE;
    newToken->value = value;
    
    tokens[numTokens] = *newToken;
    
    return i;
}

Token *createOperatorTokenFromChar(char *operation)
{
    assert(operation != NULL);
    
    // Allocate memory
    Token *newToken = malloc(sizeof(Token));
    char *tempOperation = malloc(sizeof(char) * strlen(operation));
    
    // Make sure the memory was allocated
    if (newToken == NULL || tempOperation == NULL) abort();
    strcpy(tempOperation, operation);
    
    // Select the appropriate TokenType
    if (isStringInArray(operation, (char **)NUMERICAL_OPERATORS, NUM_NUMERICAL_OPERATORS))
    {
        newToken->type = NUM_OPERATOR_TYPE;
    }
    else if (isStringInArray(operation, (char **)LOGICAL_OPERATORS, NUM_LOGICAL_OPERATORS))
    {
        newToken->type = LOG_OPERATOR_TYPE;
    }
    else if (isStringInArray(operation, (char **)COMPARISON_OPERATORS, NUM_COMPARISON_OPERATORS))
    {
        newToken->type = COMP_OPERATOR_TYPE;
    }
    else if (strcmp(operation, "(") == 0)
    {
        newToken->type = LEFT_PARENTHESIS_TYPE;
    }
    else
    {
        newToken->type = RIGHT_PARENTHESIS_TYPE;
    }
    
    // Set the value and add the token to the list
    newToken->value = tempOperation;
    
    return newToken;    
}

char *isOperator(char *c)
{
    assert(c != NULL);
    
    int result = 0;
    char *operator = NULL;
    
    for (int j = 1; j <= MAX_LENGTH_OPERATOR && !result; j++)
    {
        // Increase the length of string we're looking at here
        operator = malloc(sizeof(char) * (MAX_LENGTH_OPERATOR + 1));
        memcpy(operator, c, j);
        operator[j] = '\0';
        
        if (isStringInArray(operator, (char **)NUMERICAL_OPERATORS, NUM_NUMERICAL_OPERATORS) ||
            isStringInArray(operator, (char **)LOGICAL_OPERATORS, NUM_LOGICAL_OPERATORS) ||
            isStringInArray(operator, (char **)COMPARISON_OPERATORS, NUM_COMPARISON_OPERATORS))
        {
            result = 1;
        }
    }
    
    if (result == 0)
    {
        operator = NULL;
    }
    
    return operator;
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
        char *cc = malloc(sizeof(char));
        memcpy(cc, &infix[i], 1);
        
        // Check if it's an operand
        if (isdigit(c))
        {
            i += addNumericalOperandToTokenList(&infix[i], tokens, numTokens++);
        }
        else if (c == '(')
        {
            stackPushObject(stack, createOperatorTokenFromChar(cc));
        }
        else if (c == ')')
        {
            // While the stack is not empty, and the top object is not a left parenthesis
            while (!stackIsEmpty(stack) && ((Token *)stackReturnTopObject(stack))->type != LEFT_PARENTHESIS_TYPE)
            {
                tokens[numTokens++] = *(Token *)stackPopObject(stack);
            }
            
            stackPopObject(stack);
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
            
            while (!(isOperator(&infix[i + 1]) != NULL || infix[i+1] == '(' || infix[i+1] == ')' || infix[i+1] == ' '))
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
                newToken->type = NUM_OPERAND_TYPE;
                newToken->value = var;
                
                tokens[numTokens++] = *newToken;
            }
        }
        // Do a couple tests to see if we have an operator
        else if (c != ' ')
        {
            char *operator;
            
            // Do some things if we have an operator
            if ((operator = isOperator(&infix[i])) != NULL)
            {
                if (stackIsEmpty(stack) || ((Token *)stackReturnTopObject(stack))->type == LEFT_PARENTHESIS_TYPE)
                {
                    stackPushObject(stack, createOperatorTokenFromChar(operator));
                }
                else
                {
                    while (!stackIsEmpty(stack) && *(char *)stackReturnTopObject(stack) != '(' && operatorDoesTakePrecedence(*(char *)stackReturnTopObject(stack), c))
                    {
                        tokens[numTokens++] = *(Token *)stackPopObject(stack);
                    }
                    stackPushObject(stack, createOperatorTokenFromChar(operator));
                }
                
                // If we have already processed some extra chars we need to move the pointer ahead
                i += strlen(operator) - 1;
            }
        }
    }
    
    while (!stackIsEmpty(stack))
    {
        tokens[numTokens++] = *(Token *)stackPopObject(stack);
    }
    
    return evaluatePostfix_Int(tokens, numTokens);
}

/*
 Determines whether op1 takes precedence over op2
 
 1 = Yes
 0 = No
 */
int operatorDoesTakePrecedence(char op1, char op2)
{
    assert(op1 > 0);
    assert(op2 > 0);
    
    int result = 0;
    
    // * and / automatically take precedence over everything,
    // while the other operators are all equal
    if (op1 == '*' || op1 == '/')
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
    assert(numTokens > 0);
    
    int val = 0, operand1 = 0, operand2 = 0;
    Token *op1, *op2;
    Token *token = &postfix[0];
    Stack *stack = stackCreateNewStack();
    
    for (int i = 0; i < numTokens; i++)
    {
        if (token->type == NUM_OPERAND_TYPE || token->type == LOG_OPERAND_TYPE)
        {
            stackPushObject(stack, token);
        }
        else if (token->type == LOG_OPERATOR_TYPE)
        {
            // Get our operands as integers from the stack (with some casting/converting)
            op2 = (Token *)stackPopObject(stack);
            op1 = (Token *)stackPopObject(stack);
            
            operand2 = *(int *)op2->value;
            operand1 = *(int *)op1->value;
            
            // AND
            if (strcmp(token->value, LOGICAL_OPERATORS[0]) == 0)
            {
                val = operand1 && operand2;
            }
            // OR
            else if (strcmp(token->value, LOGICAL_OPERATORS[1]) == 0)
            {
                val = operand1 || operand2;
            }
            else
            {
                printf("ERR: Invalid logical expression.\n");
                abort();
            }
            
            // Push the result onto the stack
            Token *result = malloc(sizeof(Token));
            if (result != NULL)
            {
                result->type = LOG_OPERAND_TYPE;
                int *tempVal = malloc(sizeof(int));
                *tempVal = val;
                result->value = tempVal;
            }
            
            stackPushObject(stack, result);
        }
        else if (token->type == COMP_OPERATOR_TYPE)
        {            
            // Get our operands as integers from the stack (with some casting/converting)
            op2 = (Token *)stackPopObject(stack);
            op1 = (Token *)stackPopObject(stack);
            
            // Make sure we have numerical operands
            if (op1->type != NUM_OPERAND_TYPE || op2->type != NUM_OPERAND_TYPE)
            {
                printf("ERR: Can only do comparison operations on numerical operands.\n");
                abort();
            }
            
            operand2 = *(int *)op2->value;
            operand1 = *(int *)op1->value;
            
            // Do the comparison
            if (strcmp(token->value, COMPARISON_OPERATORS[0]) == 0)
            {
                val = operand1 > operand2 ? 1 : 0;
            }
            else if (strcmp(token->value, COMPARISON_OPERATORS[1]) == 0)
            {
                val = operand1 < operand2 ? 1 : 0;
            }
            else if (strcmp(token->value, COMPARISON_OPERATORS[2]) == 0)
            {
                val = operand1 == operand2 ? 1 : 0;
            }
            else
            {
                printf("ERR: Invalid comparison.\n");
                abort();
            }
            
            // Push the result onto the stack
            Token *result = malloc(sizeof(Token));
            if (result != NULL)
            {
                result->type = LOG_OPERAND_TYPE;
                int *tempVal = malloc(sizeof(int));
                *tempVal = val;
                result->value = tempVal;
            }

            stackPushObject(stack, result);
        }
        else if (token->type == NUM_OPERATOR_TYPE)
        {
            // Get our operands as integers from the stack (with some casting/converting)
            op2 = (Token *)stackPopObject(stack);
            op1 = (Token *)stackPopObject(stack);
            
            // Make sure we have numerical operands
            if (op1->type != NUM_OPERAND_TYPE || op2->type != NUM_OPERAND_TYPE)
            {
                printf("ERR: Can only do numerical operations on numerical operands.\n");
                abort();
            }
    
            operand2 = *(int *)op2->value;
            operand1 = *(int *)op1->value;
            
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
                result->type = NUM_OPERAND_TYPE;
                int *tempVal = malloc(sizeof(int));
                *tempVal = val;
                result->value = tempVal;
            }

            stackPushObject(stack, result);
        }
        
        // Get the next token
        token = &postfix[i + 1];
    }
    
    return *(int *)((Token *)stackPopObject(stack))->value;
}

























