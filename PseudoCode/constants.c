//
//  constants.c
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-26.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#include <stdio.h>
#include "constants.h"

const int NUM_OPERATORS = 4;
const char *OPERATORS[NUM_OPERATORS] = {"+", "-", "/", "*"};

const int NUM_LOGICAL_OPERATORS = 2;
const char *LOGICAL_OPERATORS[NUM_LOGICAL_OPERATORS] = {"AND", "OR"};

const int NUM_COMPARISON_OPERATORS = 3;
const char *COMPARISON_OPERATORS[NUM_COMPARISON_OPERATORS] = {">", "<", "=="};

const int NUM_WHITESPACE = 2;
const char *WHITESPACE[NUM_WHITESPACE] = {" ", "\n"};

const char *FILE_PATH = "program.txt";
const int LINE_MAX = 100;

const int NUM_SINGLE_OPERAND_INSTRUCTIONS = 2;
const char *SINGLE_OPERAND_INSTRUCTIONS[NUM_SINGLE_OPERAND_INSTRUCTIONS] = {"VAR", "PRINT"};

const int NUM_ASSIGNMENT_INSTRUCTIONS = 1;
const char *ASSIGNMENT_INSTRUCTIONS[NUM_ASSIGNMENT_INSTRUCTIONS] = {"SET"};

const int NUM_CONTROL_FLOW = 1;
const char *CONTROL_FLOW[NUM_CONTROL_FLOW] = {"IF"};
