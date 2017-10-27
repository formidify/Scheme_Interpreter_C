#ifndef PARSER_H
#define PARSER_H
#include <stdlib.h>
#include "value.h"
#include "talloc.h"

/*
* Takes a linked list of tokens from a Scheme program (as in
* the output of your tokenizer), and it should return a pointer
* to a parse tree representing that program.
*/
Value *parse(Value *tokens);


/*
* display a parse tree to the screen, using parentheses to denote
* tree structure. (In other words, the output of printTree should
* visually look identical to legal Scheme code.)
*/
void printTree(Value *tree);

#endif 