//
//  expression.h
//  PseudoCode
//
//  Created by Michael Thiessen on 2013-07-09.
//  Copyright (c) 2013 Michael Thiessen. All rights reserved.
//

#ifndef PseudoCode_expression_h
#define PseudoCode_expression_h

#include "../Stack.h"

int evaluateIntExpression(char *expression);
int evaluateBoolExpression(char *expression);
int evaluateComparison(char *expression);

#endif
