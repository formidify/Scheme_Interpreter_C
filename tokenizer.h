#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <stdlib.h>
#include "value.h"
#include "talloc.h"

// function should readstdin in its entireity
// and return a linked list consisting of all
// tokens found.
Value *tokenize();


// The displayTokens function takes a linked
// list of tokens as input, and displays those 
// tokens, one per line, with each token's type.
void displayTokens(Value *list);

#endif