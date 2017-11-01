#ifndef INTERPRETER_H
#define INTERPRETER_H

struct Frame {
   Value *bindings;
   struct Frame *parent;
};
typedef struct Frame Frame;

/*
* Handles a list of S-expressions (as in a Scheme program),
* calls eval on each S-expression in the top-level (global) environment,
* and prints each result in turn
*/
void interpret(Value *tree);

/*
* Takes a parse tree of a single S-expression and an environment frame
* in which to evaluate the expression, and returns a pointer to a Value
* representing the value
*/
Value *eval(Value *expr, Frame *frame);

#endif
