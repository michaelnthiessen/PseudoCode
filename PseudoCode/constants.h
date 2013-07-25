//
//  constants.h
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-25.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#ifndef PseudoCode_constants_h
#define PseudoCode_constants_h

static const int NUM_OPERATORS = 4;
static const char *OPERATORS[NUM_OPERATORS] = {"+", "-", "/", "*"};

static const int NUM_LOGICAL_OPERATORS = 2;
static const char *LOGICAL_OPERATORS[NUM_LOGICAL_OPERATORS] = {"AND", "OR"};

static const int NUM_COMPARISON_OPERATORS = 3;
static const char *COMPARISON_OPERATORS[NUM_COMPARISON_OPERATORS] = {">", "<", "=="};

static const int MAX_VARS = 100;
static const int DEFAULT_VAL = 0;

static const int NUM_WHITESPACE = 2;
static const char *WHITESPACE[NUM_WHITESPACE] = {" ", "\n"};

static const char *FILE_PATH = "program.txt";
static const int LINE_MAX = 100;

static const int NUM_SINGLE_OPERAND_INSTRUCTIONS = 2;
static const char *SINGLE_OPERAND_INSTRUCTIONS[NUM_SINGLE_OPERAND_INSTRUCTIONS] = {"VAR", "PRINT"};

static const int NUM_ASSIGNMENT_INSTRUCTIONS = 1;
static const char *ASSIGNMENT_INSTRUCTIONS[NUM_ASSIGNMENT_INSTRUCTIONS] = {"SET"};

static const int NUM_CONTROL_FLOW = 1;
static const char *CONTROL_FLOW[NUM_CONTROL_FLOW] = {"IF"};

#endif
