//
//  constants.h
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-09.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#ifndef PseudoCode_constants_h
#define PseudoCode_constants_h


//------------------ Constants ----------------------
const char *FILE_PATH = "program.txt";
const int LINE_MAX = 100;

const int NUM_SINGLE_OPERAND_INSTRUCTIONS = 2;
const char *SINGLE_OPERAND_INSTRUCTIONS[NUM_SINGLE_OPERAND_INSTRUCTIONS] = {"VAR", "PRINT"};

const int NUM_ASSIGNMENT_INSTRUCTIONS = 1;
const char *ASSIGNMENT_INSTRUCTIONS[NUM_ASSIGNMENT_INSTRUCTIONS] = {"SET"};

const int NUM_CONTROL_FLOW = 1;
const char *CONTROL_FLOW[NUM_CONTROL_FLOW] = {"IF"};

#endif
